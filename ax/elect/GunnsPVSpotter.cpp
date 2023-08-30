/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/elect/GunnsPVSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

GunnsPVSpotterConfigData::GunnsPVSpotterConfigData(const std::string& name,
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

GunnsPVSpotterInputData::GunnsPVSpotterInputData(double startingFluxFromBatt, 
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

GunnsPVSpotter::GunnsPVSpotter()
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

void GunnsPVSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                       const GunnsNetworkSpotterInputData*  inputData)
{
  /// - Initialize the base class.
  GunnsNetworkSpotter::initialize(configData, inputData);

  /// - Reset the init flag.
  mInitFlag = false;

  /// - Validate & type-cast config & input data.
  const GunnsPVSpotterConfigData* config = validateConfig(configData);
  const GunnsPVSpotterInputData*  input  = validateInput(inputData);

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

const GunnsPVSpotterConfigData* GunnsPVSpotter::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
  const GunnsPVSpotterConfigData* result = dynamic_cast<const GunnsPVSpotterConfigData*>(config);
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

const GunnsPVSpotterInputData* GunnsPVSpotter::validateInput(const GunnsNetworkSpotterInputData* input)
{
  const GunnsPVSpotterInputData* result = dynamic_cast<const GunnsPVSpotterInputData*>(input);
  if (!result) {
      GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                  "Bad input data pointer type.");
  }

  /// - Do your other data validation as appropriate.

  return result;
}

void GunnsPVSpotter::stepPreSolver(const double dt) {

  /// - Update spotter mode from user input
  if (mOverrideStatus) {
    switch(mNextCommandedStatus) {
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
    updateStatus();
    /// - NOTE_ There should be like, a 'no change' to reset to?
    mOverrideStatus = false;
  }

  // HACK_ If both channels are on, disable charging
  if ((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
    && mBatterySource->getFluxDemand() > 0.0)
  {
    std::cerr << "Both Charging and Discharging enabled on battery '" << mBattery->getName() << "' . Disabling charging" << std::endl;
    disableCharging();
  }

  // Basic Hysteresis here -- automatically charge/discharge based on SoC
  if (mAutoThresholdsEnabled) {
    if ((mBattery->getSoc() <= mLowSocCutoff) && (mStatus != BmsStatus::CHARGING)) {
      disableDischarging();
      enableCharging();
      updateStatus(); // FIXME_ This doesn't _necessarily_ make it mStatus == Charging
      std::cerr << "Battery '" << mBattery->getName() << "' hit low SoC threshold, switching status to: " << returnStatus() << std::endl;
    } else if ((mBattery->getSoc() >= mHighSocCutoff) && mStatus != BmsStatus::DISCHARGING) {
      disableCharging();
      enableDischarging();
      updateStatus(); // FIXME_ This doesn't _necessarily_ make it mStatus == Discharging
      std::cerr << "Battery '" << mBattery->getName() << "' hit high SoC threshold, switching status to: " << returnStatus() << std::endl;
    }
  }
}

void GunnsPVSpotter::stepPostSolver(const double dt) {
  // HACK_ If both channels are on, disable charging
  if (((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
    && mBatterySource->getFluxDemand() > 0.0))
  {
    std::cerr << "Both Charging and Discharging enabled on battery '" << mBattery->getName() << "' . Disabling charging" << std::endl;
    disableCharging();
  }

  addFlux(dt);
}

void GunnsPVSpotter::enableCharging() {
  disableDischarging();
  mBatterySource->setFluxDemand(mDefaultChargeCurrent);

}
void GunnsPVSpotter::disableCharging() {
  mBatterySource->setFluxDemand(0.0);
}

void GunnsPVSpotter::enableDischarging() {
  disableCharging();
  mBmsUpIn->setEnabled(true);
  mBmsUpOut->setEnabled(true);
}
void GunnsPVSpotter::disableDischarging() {
  mBmsUpIn->setEnabled(false);
  mBmsUpOut->setEnabled(false);
}

bool GunnsPVSpotter::isCharging() {
  return (mBatterySource->getFluxDemand() > 0.0);
}
bool GunnsPVSpotter::isDischarging() {
  return (mBmsUpIn->getEnabled() && mBmsUpOut->getEnabled());
}

bool GunnsPVSpotter::isInvalid() {
  return (mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) && mBatterySource->getFluxDemand() > 0.0;
}

void GunnsPVSpotter::updateStatus() {
  if (isInvalid()) {
    mStatus = BmsStatus::INVALID;
  } else if (isCharging()) {
    mStatus = BmsStatus::CHARGING;
  } else if (isDischarging()) {
    mStatus = BmsStatus::DISCHARGING;
  }
}

void GunnsPVSpotter::addFlux(const double dt) {
  mNetFluxFromBatt += (dt*mBattery->getFlux()); // mFlux should already account for direction
}

void GunnsPVSpotter::updateChargeCurrent(const double newCurrent) {
  mDefaultChargeCurrent = newCurrent;
}