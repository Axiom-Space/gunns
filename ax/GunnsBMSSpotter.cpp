/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/GunnsBMSSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

GunnsBMSSpotterConfigData::GunnsBMSSpotterConfigData(const std::string& name,
                                                    GunnsElectBattery* battery,
                                                    GunnsElectConverterInput* bmsUpIn,
                                                    GunnsElectConverterOutput* bmsUpOut,
                                                    GunnsLosslessSource*       battSource)
    :
    GunnsNetworkSpotterConfigData(name),
    mBattery(battery),
    mBmsUpIn(bmsUpIn),
    mBmsUpOut(bmsUpOut),
    mBattSource(battSource)
{
    // nothing to do
}

GunnsBMSSpotterInputData::GunnsBMSSpotterInputData(double startingFluxFromBatt, 
        double lowSocCutoff, double highSocCutoff, double defaultChargeCurrent) // 
    : GunnsNetworkSpotterInputData()
    , mStartingNetFluxFromBatt(startingFluxFromBatt)
    , mLowSocCutoff(lowSocCutoff)
    , mHighSocCutoff(highSocCutoff)
    , mDefaultChargeCurrent(defaultChargeCurrent)
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
    mBattSource = config->mBattSource;

    mNetFluxFromBatt = input->mStartingNetFluxFromBatt;
    mLowSocCutoff = input->mLowSocCutoff;
    mHighSocCutoff = input->mHighSocCutoff;

    /// - Set the init flag.
    mInitFlag = true;
}

const GunnsBMSSpotterConfigData* GunnsBMSSpotter::validateConfig(const GunnsNetworkSpotterConfigData* config)
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
    // 1. Make sure not both channels
    // 2. Decide if we should be charging or discharging
    //   a. Status of battery (safety wrt over/under charging most important)
    //   b. How long have we been charging/discharging (Can't switch back and forth too much) -> 
    //   c. Status of other batteries (e.g. if only one battery pack is low, rely on others more and charge the one)
    //   d. 
    // 3. Enable whichever channel
    // 4. Set mStatus last thing in PreSolver
    // 5. Profit ???

    // FIXME_ Check if both channels are on
    if ((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
        && mBattSource->getFluxDemand() > 0.0)
    {
        std::cerr << "Both Up or Down Conv pairs enabled. Disabling charging" << std::endl;
        disableCharging();
    }

    // Bad Hysteresis here
    if ((mBattery->getSoc() <= 0.30) && (mStatus != BmsStatus::CHARGING)) {
        disableDischarging();
        enableCharging();
        updateStatus(); // FIXME_ This doesn't _necessarily_ make it mStatus == Charging
        std::cerr << returnStatus() << std::endl;
    } else if ((mBattery->getSoc() >= 0.80) && mStatus != BmsStatus::DISCHARGING) {
        disableCharging();
        enableDischarging();
        updateStatus(); // FIXME_ This doesn't _necessarily_ make it mStatus == Discharging
        std::cerr << returnStatus() << std::endl;
    }

    // // try to keep transformer down out voltage equal to battery voltage?
    // if (mStatus == BmsStatus::CHARGING) { // mSourcePotential is set at end of GunnsElectBattery::updateState()
    //     mBmsDownOut->setSetpoint(mBattery->getSourcePotential());
    // }

}

void GunnsBMSSpotter::stepPostSolver(const double dt) {
    // 
    if (((mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) 
        && mBattSource->getFluxDemand() > 0.0))
    {
        std::cerr << "Both Up or Down Conv pairs enabled. Disabling charging" << std::endl;
        disableCharging();
    }

    addFlux(dt);
}

void GunnsBMSSpotter::enableCharging() {
    disableDischarging();
    mBattSource->setFluxDemand(mDefaultChargeCurrent);

}
void GunnsBMSSpotter::disableCharging() {
   mBattSource->setFluxDemand(0.0);
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
    return (mBattSource->getFluxDemand() > 0.0);
}
bool GunnsBMSSpotter::isDischarging() {
    return (mBmsUpIn->getEnabled() && mBmsUpOut->getEnabled());
}

bool GunnsBMSSpotter::isInvalid() {
    return (mBmsUpIn->getEnabled() || mBmsUpOut->getEnabled()) && mBattSource->getFluxDemand() > 0.0;
}

void GunnsBMSSpotter::updateStatus() {
    if (isInvalid()) {
        mStatus = BmsStatus::INVALID;
    } else if (isCharging()) {
        mStatus = BmsStatus::CHARGING;
    } else if (isDischarging()) {
        mStatus = BmsStatus::DISCHARGING;
    }
}

void GunnsBMSSpotter::addFlux(const double dt) {
    mNetFluxFromBatt += (dt*mBattery->getFlux()); // mFlux should already account for direction
}

void GunnsBMSSpotter::updateChargeCurrent(const double newCurrent) {
    mDefaultChargeCurrent = newCurrent;
}