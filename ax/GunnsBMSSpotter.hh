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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:

        const GunnsElectConverterInput*   mBmsUpIn;
        const GunnsElectConverterOutput*  mBmsUpOut;
        const GunnsElectConverterInput*   mBmsDownIn;
        const GunnsElectConverterOutput*  mBmsDownOut;

        GunnsBMSSpotterConfigData(const std::string& name,
                                    const GunnsElectConverterInput* bmsUpIn,
                                    const GunnsElectConverterOutput* bmsUpOut,
                                    const GunnsElectConverterInput* bmsDownIn,
                                    const GunnsElectConverterOutput* bmsDownOut);
        virtual ~GunnsBMSSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        int mPostStepCounter;                     /**< (--) initial counter to support unit tests */
        GunnsBMSSpotterInputData(const int postStepCounter = 0);
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
            DISCHARGING     = 0,
            CHARGING        = 1,
            TRIPPED         = 2,
        };
        GunnsBMSSpotter();
        virtual     ~GunnsBMSSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        virtual void stepPreSolver(const double dt);
        virtual void stepPostSolver(const double dt);

    protected:
        int                  mPreStepCounter;       /**< (--) counter to support unit tests */
        int                  mPostStepCounter;      /**< (--) counter to support unit tests */
        const GunnsBMSSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const GunnsBMSSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
    private:
        GunnsElectConverterInput*   mBmsUpIn;
        GunnsElectConverterOutput*  mBmsUpOut;
        GunnsElectConverterInput*   mBmsDownIn;
        GunnsElectConverterOutput*  mBmsDownOut;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBMSSpotter and befriend UtGunnsNetworkSpotter.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
//class FriendlyGunnsBMSSpotter : public GunnsBMSSpotter
//{
//    public:
//        FriendlyGunnsBMSSpotter(GunnsBasicConductor& linkRef) : GunnsBMSSpotter(linkRef) {};
//        virtual ~FriendlyGunnsBMSSpotter() {;}
//        friend class UtGunnsNetworkSpotter;
//};



///@}

#endif
