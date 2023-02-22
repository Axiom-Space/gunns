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
                                                    GunnsElectConverterInput* bmsDownIn,
                                                    GunnsElectConverterOutput* bmsDownOut)
    :
    GunnsNetworkSpotterConfigData(name),
    mBattery(battery),
    mBmsUpIn(bmsUpIn),
    mBmsUpOut(bmsUpOut),
    mBmsDownIn(bmsDownIn),
    mBmsDownOut(bmsDownOut)
{
    // nothing to do
}

GunnsBMSSpotterInputData::GunnsBMSSpotterInputData()
    :
    GunnsNetworkSpotterInputData()
{
    // nothing to do
}

GunnsBMSSpotter::GunnsBMSSpotter()
    : GunnsNetworkSpotter()
{

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
    mBmsDownIn = config->mBmsDownIn;
    mBmsDownOut = config->mBmsDownOut;

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
    /// - Do your other data validation as appropriate.
    if ((result->mBmsUpIn == result->mBmsDownIn) || (result->mBmsUpOut == result->mBmsDownOut)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Input or Output Converter pair not unique.");
    }
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
    if ((mBmsUpIn->getEnabled() && mBmsDownIn->getEnabled()) || 
        (mBmsUpOut->getEnabled() && mBmsDownOut->getEnabled()))
    {
        std::cerr << "Both Up or Down Conv pairs enabled. Disabling down-conv" << std::endl;
        mBmsDownIn->setEnabled(false);
        mBmsDownOut->setEnabled(false);
    }

}

void GunnsBMSSpotter::stepPostSolver(const double dt) {
    if (((mBmsUpIn->getEnabled() && mBmsDownIn->getEnabled()) || (mBmsUpOut->getEnabled() && mBmsDownOut->getEnabled())))
    {
        std::cerr << "Both Up or Down Conv pairs enabled. Disabling down-conv" << std::endl;
        mBmsDownIn->setEnabled(false);
        mBmsDownOut->setEnabled(false);
    }

}

    void GunnsBMSSpotter::enableCharging() {
        disableDischarging();
        mBmsDownIn->setEnabled(true);
        mBmsDownOut->setEnabled(true);
    }
    void GunnsBMSSpotter::disableCharging() {
        mBmsDownIn->setEnabled(false);
        mBmsDownOut->setEnabled(false);
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
        return (mBmsDownIn->getEnabled() && mBmsDownOut->getEnabled());
    }
    bool GunnsBMSSpotter::isDischarging() {
        return (mBmsUpIn->getEnabled() && mBmsUpOut->getEnabled());
    }
    bool GunnsBMSSpotter::isInvalidBoth() {
        return (
            (mBmsUpIn->getEnabled() && mBmsUpOut->getEnabled()) &&
            (mBmsDownIn->getEnabled() && mBmsDownOut->getEnabled())
        );
    }
    bool GunnsBMSSpotter::isInvalidExclusive() {
        return (
            (mBmsUpIn->getEnabled() ^ mBmsUpOut->getEnabled()) ||
            (mBmsDownIn->getEnabled() ^ mBmsDownOut->getEnabled())
        );
    }
    bool GunnsBMSSpotter::isInvalid() {
        return isInvalidBoth() || isInvalidExclusive();
    }