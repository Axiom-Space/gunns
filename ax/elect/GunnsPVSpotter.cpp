/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/elect/GunnsPVSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

GunnsPVSpotterConfigData::GunnsPVSpotterConfigData(const std::string& name,
                                                  GunnsElectPvArray*    array,
                                                  GunnsElectPvRegConv*  reg,
                                                  GunnsLosslessSource*  source,
                                                  SwitchElect*          sswitch)
  : GunnsNetworkSpotterConfigData(name)
  , mArray(array)
  , mReg(reg)
  , mSwitch(sswitch)
  , mSource(source)
{
    // nothing to do
}

GunnsPVSpotterInputData::GunnsPVSpotterInputData() // 
  : GunnsNetworkSpotterInputData()
{
    // nothing to do
}

GunnsPVSpotter::GunnsPVSpotter()
  : GunnsNetworkSpotter()
  , mStatus(PVStatus::DISABLED)
  , mPVCalcCurrent(0.0)
  , mNextCommandedStatus(PVStatus::DISABLED)
  , mOverrideStatus(false)
  , mBothEnabledLastStep(false)
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
  mArray = config->mArray;
  mReg = config->mReg;
  mSwitch = config->mSwitch;
  mSource = config->mSource;

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
        disableAutomaticControl();
        disableManualControl();
        break;
      case MANUAL:
      /// FIXME_: TristanMansfieldRight now, disableAuto sets "switch commanded closed" to false, which doesn't take effect until the next timestep, so you have to toggle manual twice :(
        disableAutomaticControl();
        enableManualControl();
        break;
      case AUTOMATIC:
        disableManualControl();
        enableAutomaticControl();
        break;
      default:
        throw "How did you set mNextCommandedStatus to something invalid?";
    }
    // updateStatus(); // <- Moved to end of stepPreSolver
    /// - NOTE_ There should be like, a 'no change' to reset to?
    mOverrideStatus = false;
  }

  // HACK_ If both channels are on, disable MANUAL
  if (isAutomatic() && isManual())
  {
    if (mBothEnabledLastStep) {
      std::cerr << "Both Switch and Source enabled for PV: '" << mArray->getName() << "' . Disabling Source" << std::endl;
      disableManualControl();
      mBothEnabledLastStep = false;
    } else {
      mBothEnabledLastStep = true;
    }
  } else {mBothEnabledLastStep = false;}

  /// - Make mStatus reflect changes
  updateStatus();
}

void GunnsPVSpotter::stepPostSolver(const double dt) {
  /// Nothing to do for now
}

void GunnsPVSpotter::enableAutomaticControl() {
  mSwitch->setSwitchCommandedClosed(true);

}
void GunnsPVSpotter::disableAutomaticControl() {
  mSwitch->setSwitchCommandedClosed(false);
}

void GunnsPVSpotter::enableManualControl() {
  mSource->setFluxDemand(mPVCalcCurrent);
}
void GunnsPVSpotter::disableManualControl() {
  mSource->setFluxDemand(0.0);
}

void GunnsPVSpotter::disable() {
  disableManualControl();
  disableAutomaticControl();
}

void GunnsPVSpotter::updateStatus() {
  if (isDisabled()) {
    mStatus = PVStatus::DISABLED;
  } else if (isManual()) {
    mStatus = PVStatus::MANUAL;
  } else if (isAutomatic()) {
    mStatus = PVStatus::AUTOMATIC;
  }
}

void GunnsPVSpotter::updateSourceCurrent(double newSourceCurrent) {
  if (newSourceCurrent < 0.0) {
    return;
  }
  mPVCalcCurrent = newSourceCurrent;
  /// If we're currently in automatic mode we don't want to change this
  if (isManual()) {
    mSource->setFluxDemand(mPVCalcCurrent);
  }
  return;
}

bool GunnsPVSpotter::isAutomatic() {
  return mSwitch->isSwitchClosed();
}

bool GunnsPVSpotter::isManual() {
  return mSource->getFluxDemand() != 0.0;
}

bool GunnsPVSpotter::isDisabled() {
  return !(isAutomatic() || isManual());
}

