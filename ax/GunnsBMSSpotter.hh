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

/* FORWARD WORK: -> 
 * 1.
 */


#include "core/GunnsNetworkSpotter.hh"
#include "core/GunnsBasicConductor.hh"
#include "aspects/electrical/Converter/GunnsElectConverterInput.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"
#include "aspects/electrical/Batt/GunnsElectBattery.hh"

#include "ax/elect/GunnsLosslessSource.hh"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    BMS Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:

        GunnsElectBattery*          mBattery;
        GunnsElectConverterInput*   mBmsUpIn;
        GunnsElectConverterOutput*  mBmsUpOut;
        GunnsLosslessSource*        mBatterySource;
        

        GunnsBMSSpotterConfigData(const std::string& name,
                                    GunnsElectBattery*         battery,
                                    GunnsElectConverterInput*  bmsUpIn,
                                    GunnsElectConverterOutput* bmsUpOut,
                                    GunnsLosslessSource*       battSource);
        virtual ~GunnsBMSSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    BMS Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBMSSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        double mStartingNetFluxFromBatt;
        double      mLowSocCutoff;
        double      mHighSocCutoff;
        double      mDefaultChargeCurrent;
        bool        mAutoThresholdsEnabled;

        GunnsBMSSpotterInputData(double startingFluxFromBatt, double lowSocCutoff, double highSocCutoff, double defaultChargeCurrent, bool autoThresholdsEnabled);
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
            INVALID         = 4,
        };

        std::string returnStatus() {
            switch (this->mStatus)
            {
            case BmsStatus::DISABLED:
                return "DISABLED";
                break;
            case BmsStatus::DISCHARGING:
                return "DISCHARGING";
                break;
            case BmsStatus::CHARGING:
                return "CHARGING";
                break;
            case BmsStatus::TRIPPED:
                return "TRIPPED";
                break;
            case BmsStatus::INVALID:
                return "INVALID";
                break;
            default:
                break;
            }
        }
        
        GunnsBMSSpotter();
        virtual     ~GunnsBMSSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        virtual void stepPreSolver(const double dt);
        virtual void stepPostSolver(const double dt);

        void enableCharging();
        void disableCharging();
        void enableDischarging();
        void disableDischarging();

        bool isCharging();
        bool isDischarging();
        bool isInvalid();

        void updateStatus();
        void updateChargeCurrent(const double newCurrent);

        GunnsElectBattery* mBattery;

        BmsStatus mNextCommandedStatus;
        bool      mOverrideStatus;


    protected:
        const GunnsBMSSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const GunnsBMSSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
        
        void applyUpdatedStatus();

    private:
        
        GunnsElectConverterInput*   mBmsUpIn;
        GunnsElectConverterOutput*  mBmsUpOut;
        GunnsLosslessSource*        mBatterySource;

        double      mNetFluxFromBatt;
        double      mLowSocCutoff;
        double      mHighSocCutoff;
        double      mDefaultChargeCurrent;

        double      mTotalDischargeTime;
        double      mTotalChargeTime;
        double      mCurrentStateTime;

        bool        mAutoThresholdsEnabled;             /**< *o (--) trick_chkpnt_io(**) if bool -> auto enable charging/discharging based on battery SoC */

        BmsStatus   mStatus;

        void addFlux(const double dt);

};

///@}

#endif
