#ifndef GunnsBMSSpotter_EXISTS
#define GunnsBMSSpotter_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_NETWORK_SPOTTER    Gunns Network Spotter Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Network Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "core/GunnsNetworkSpotter.hh"
#include "core/GunnsBasicConductor.hh"
#include "aspects/electrical/Converter/GunnsElectConverterInput.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:

        GunnsElectConverterInput*   mBmsUpIn;
        GunnsElectConverterOutput*  mBmsUpOut;
        GunnsElectConverterInput*   mBmsDownIn;
        GunnsElectConverterOutput*  mBmsDownOut;

        GunnsBMSSpotterConfigData(const std::string& name,
                                    GunnsElectConverterInput* bmsUpIn,
                                    GunnsElectConverterOutput* bmsUpOut,
                                    GunnsElectConverterInput* bmsDownIn,
                                    GunnsElectConverterOutput* bmsDownOut);
        virtual ~GunnsBMSSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        GunnsBMSSpotterInputData();
        virtual ~GunnsBMSSpotterInputData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Derived BMS Spotter Class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotter : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBMSSpotter);
    public:
        /// @brief  Enumeration of BMS States
        enum BmsStatus {
            DISABLED        = 0,
            DISCHARGING     = 1,
            CHARGING        = 2,
            TRIPPED         = 3,
        };
        GunnsBMSSpotter();
        virtual     ~GunnsBMSSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        virtual void stepPreSolver(const double dt);
        virtual void stepPostSolver(const double dt);


    protected:
        const GunnsBMSSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const GunnsBMSSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
    private:
        GunnsElectConverterInput*   mBmsUpIn;
        GunnsElectConverterOutput*  mBmsUpOut;
        GunnsElectConverterInput*   mBmsDownIn;
        GunnsElectConverterOutput*  mBmsDownOut;
};

///@}

#endif
