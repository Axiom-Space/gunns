/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/GunnsBMSSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

GunnsBMSSpotterConfigData::GunnsBMSSpotterConfigData(const std::string& name,
                                                    const GunnsElectConverterInput* bmsUpIn,
                                                    const GunnsElectConverterOutput* bmsUpOut,
                                                    const GunnsElectConverterInput* bmsDownIn,
                                                    const GunnsElectConverterOutput* bmsDownOut)
    :
    GunnsNetworkSpotterConfigData(name),
    mBmsUpIn(bmsUpIn),
    mBmsUpOut(bmsUpOut),
    mBmsDownIn(bmsDownIn),
    mBmsDownOut(bmsDownOut)
{
    // nothing to do
}

GunnsBMSSpotterInputData::GunnsBMSSpotterInputData(const int postStepCounter)
    :
    mPostStepCounter(postStepCounter)
{
    // nothing to do
}

GunnsBMSSpotter::GunnsBMSSpotter()
    :
    GunnsNetworkSpotter(),

    mPreStepCounter(0),
    mPostStepCounter(0)
{
    // nothing to do
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
    mPreStepCounter  = config->mPreStepCounter;
    mPostStepCounter = input->mPostStepCounter;

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

void GunnsBMSSpotter::stepPreSolver(const double dt __attribute__((unused))) {
    mPreStepCounter++;
}

void GunnsBMSSpotter::stepPostSolver(const double dt __attribute__((unused))) {
    mPostStepCounter++;
}
