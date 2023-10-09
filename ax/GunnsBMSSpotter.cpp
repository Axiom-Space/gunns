/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/GunnsBMSSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GunnsBMSSpotter.hh"

GunnsBMSSpotterConfigData::GunnsBMSSpotterConfigData(const std::string& name,
                                                    GunnsElectBattery* battery,
                                                    GunnsElectConverterInput* bmsUpIn,
                                                    GunnsElectConverterOutput* bmsUpOut,
                                                    GunnsLosslessSource*       batterySource)
  : GunnsNetworkSpotterConfigData(name)
  , mBattery(battery)
  , mBmsUpIn(bmsUpIn)
  , mBmsUpOut(bmsUpOut)
  , mBatterySource(batterySource)
{
    // nothing to do
}

GunnsBMSSpotterInputData::GunnsBMSSpotterInputData(double startingFluxFromBatt, 
        double lowSocCutoff, double highSocCutoff, double defaultChargeCurrent,
        bool autoThresholdsEnabled) // 
  : GunnsNetworkSpotterInputData()
  , mStartingNetFluxFromBatt(startingFluxFromBatt)
  , mLowSocCutoff(lowSocCutoff)
  , mHighSocCutoff(highSocCutoff)
  , mDefaultChargeCurrent(defaultChargeCurrent)
  , mAutoThresholdsEnabled(autoThresholdsEnabled)
{
    // nothing to do
}

GunnsBMSSpotter::GunnsBMSSpotter()
  : GunnsNetworkSpotter()
  , mDefaultChargeCurrent(0.0)
  , mTotalDischargeTime(0.0)
  , mTotalChargeTime(0.0)
  , mCurrentStateTime(0.0)
  , mStatus(BmsStatus::DISCHARGING)
  , mAutoThresholdsEnabled(true)
{
    // Nothing to do
}

void GunnsBMSSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                       const GunnsNetworkSpotterInputData*  inputData)
{
  /// - Initialize the base class.
  GunnsNetworkSpotter::initialize(configData, inputData);

  /// - Reset the init flag.
  mInitFlag = false;

  /// - Validate & type-cast config & input data.
  const GunnsBMSSpotterConfigData* config = validateConfig(configData);
  const GunnsBMSSpotterInputData*  input  = validateInput(inputData);

  /// - Initialize with validated config & input data.
  mBattery = config->mBattery;
  mBmsUpIn = config->mBmsUpIn;
  mBmsUpOut = config->mBmsUpOut;
  mBatterySource = config->mBatterySource;

  mNetFluxFromBatt = input->mStartingNetFluxFromBatt;
  mLowSocCutoff = input->mLowSocCutoff;
  mHighSocCutoff = input->mHighSocCutoff;
  mDefaultChargeCurrent = input->mDefaultChargeCurrent;
  mAutoThresholdsEnabled = input->mAutoThresholdsEnabled;

  /// - Set the init flag.
  mInitFlag = true;
}

double GunnsBMSSpotter::getChargeCurrent()
{
  return mDefaultChargeCurrent;
}

double GunnsBMSSpotter::getBmsEfficiency()
{
  return mBmsUpOut->getConverterEfficiency();
}

double GunnsBMSSpotter::getOutputVoltage()
{
  return mBmsUpOut->getPotentialVector()[0];
}

const GunnsBMSSpotterConfigData *GunnsBMSSpotter::validateConfig(const GunnsNetworkSpotterConfigData *config)
{
  const GunnsBMSSpotterConfigData* result = dynamic_cast<const GunnsBMSSpotterConfigData*>(config);
  if (!result) {
      GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                  "Bad config data pointer type.");
  }
  // /// - Do your other data validation as appropriate.
  // if ((result->mBmsUpIn == result->mBmsDownIn) || (result->mBmsUpOut == result->mBmsDownOut)) {
  //     GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
  //                 "Input or Output Converter pair not unique.");
  // }
  return result;
}

const GunnsBMSSpotterInputData* GunnsBMSSpotter::validateInput(const GunnsNetworkSpotterInputData* input)
{
  const GunnsBMSSpotterInputData* result = dynamic_cast<const GunnsBMSSpotterInputData*>(input);
  if (!result) {
      GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                  "Bad input data pointer type.");
  }

  /// - Do your other data validation as appropriate.

  return result;
}

void GunnsBMSSpotter::stepPreSolver(const double dt) {

  /// - Update spotter mode from user input
  if (mOverrideStatus) {
    updateStatus(mNextCommandedStatus);
    /// - NOTE_ There should be like, a 'no change' to reset to?
    mOverrideStatus = false;
  }

  // HACK_ If both channels are on, disable charging
  if ((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
    && mBatterySource->getFluxDemand() > 0.0)
  {
    message_publish(MESSAGE_TYPE::MSG_INFO, "Both Charging and Discharging enabled on battery '%s', Disabling charging.\n", mBattery->getName());
    disableCharging();
  }

  // Basic Hysteresis here -- automatically charge/discharge based on SoC
  if (mAutoThresholdsEnabled) {
    if ((mBattery->getSoc() <= mLowSocCutoff) && (mStatus != BmsStatus::CHARGING)) {
      disableDischarging();
      enableCharging();
      updateStatusVar(); // FIXME_ This doesn't _necessarily_ make it mStatus == Charging
      message_publish(MESSAGE_TYPE::MSG_INFO, "Battery '%s' hit low SoC threshold of %1.4f, switching status to: %s\n", mBattery->getName(), mLowSocCutoff, returnStatus().data());
    } else if ((mBattery->getSoc() >= mHighSocCutoff) && mStatus != BmsStatus::DISCHARGING) {
      disableCharging();
      enableDischarging();
      updateStatusVar(); // FIXME_ This doesn't _necessarily_ make it mStatus == Discharging
      message_publish(MESSAGE_TYPE::MSG_INFO, "Battery '%s' hit high SoC threshold of %1.4f, switching status to: %s\n", mBattery->getName(), mHighSocCutoff, returnStatus().data());
    }
  }
}

void GunnsBMSSpotter::stepPostSolver(const double dt) {
  // HACK_ If both channels are on, disable charging
  if (((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
    && mBatterySource->getFluxDemand() > 0.0))
  {
    message_publish(MESSAGE_TYPE::MSG_INFO, "Both Charging and Discharging enabled on battery '%s', Disabling charging.\n", mBattery->getName());
    disableCharging();
  }

  addFlux(dt);
}

void GunnsBMSSpotter::enableCharging() {
  disableDischarging();
  mBatterySource->setFluxDemand(mDefaultChargeCurrent);

}
void GunnsBMSSpotter::disableCharging() {
  mBatterySource->setFluxDemand(0.0);
}

void GunnsBMSSpotter::enableDischarging() {
  disableCharging();
  mBmsUpIn->setEnabled(true);
  mBmsUpOut->setEnabled(true);
}
void GunnsBMSSpotter::disableDischarging() {
  mBmsUpIn->setEnabled(false);
  mBmsUpOut->setEnabled(false);
}

bool GunnsBMSSpotter::isCharging() {
  return (mBatterySource->getFluxDemand() > 0.0);
}

bool GunnsBMSSpotter::isDischarging() {
  return (mBmsUpIn->getEnabled() && mBmsUpOut->getEnabled());
}

bool GunnsBMSSpotter::isInvalid() {
  return (mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) && mBatterySource->getFluxDemand() > 0.0;
}

void GunnsBMSSpotter::updateStatusVar() {
  if (isInvalid()) {
    mStatus = BmsStatus::INVALID;
  } else if (isCharging()) {
    mStatus = BmsStatus::CHARGING;
  } else if (isDischarging()) {
    mStatus = BmsStatus::DISCHARGING;
  } else {
    mStatus = BmsStatus::DISABLED;
  }
}

void GunnsBMSSpotter::addFlux(const double dt) {
  mNetFluxFromBatt += (dt*mBattery->getFlux()); // mFlux should already account for direction
}

void GunnsBMSSpotter::updateChargeCurrent(const double newCurrent) {
  mDefaultChargeCurrent = newCurrent;
}

void GunnsBMSSpotter::updateDischargeCurrentLimit(const double newCurrentLimit)
{
  /**
   * If the ConvOut link has limiting enabled, this changes the limited-to current.
   * If it has OverCurrent trip enabled, this changes the trip threshold.
  */

  mBmsUpOut->getOutputOverCurrentTrip()->setLimit(newCurrentLimit);
}

void GunnsBMSSpotter::updateStatus(BmsStatus mode)
{
  switch(mode) {
      case DISABLED:
        disableDischarging();
        disableCharging();
        break;
      case DISCHARGING:
        enableDischarging();
        break;
      case CHARGING:
        enableCharging();
        break;
      case TRIPPED:
        throw "Not yet Implemented mNextCommandedStatus = TRIPPED";
        break;
      case INVALID:
        throw "Not yet Implemented mNextCommandedStatus = INVALID";
        break;
    }
    updateStatusVar();
}

GunnsBMSSpotter::BmsStatus GunnsBMSSpotter::getStatus()
{
  return mStatus;
}