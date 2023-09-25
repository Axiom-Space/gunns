#ifndef GunnsBMSSpotter_EXISTS
#define GunnsBMSSpotter_EXISTS

/**
@file
@brief     GUNNS BMS Spotter declarations

@defgroup  TSM_GUNNS_ELECT_SPOTTER_BMS_SPOTTER    GUNNS Elect BMS Spotter
@ingroup   TSM_GUNNS_ELECT_SPOTTER

@copyright 

PURPOSE:   (Provides the classes for the GUNNS BMS Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBMSSpotter.o))

PROGRAMMERS:
- (
  (Tristan Mansfield) (Axiom Space) (2023-09) (Initial)
  )

@{
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
    double  mStartingNetFluxFromBatt;
    double  mLowSocCutoff;
    double  mHighSocCutoff;
    double  mDefaultChargeCurrent;
    bool    mAutoThresholdsEnabled;

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
      switch (this->mStatus){
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

    void updateStatusVar();
    void updateChargeCurrent(const double newCurrent);
    
    void updateStatus(BmsStatus mode);
    BmsStatus getStatus();

    GunnsElectBattery* mBattery;

    BmsStatus mNextCommandedStatus;
    bool      mOverrideStatus;

    
    double      mLowSocCutoff;
    double      mHighSocCutoff;

    /**
     * @brief Returns this GunnsBMSSpotter's `mDefaultChargeCurrent` term
     * @return This GunnsBMSSpotter's `mDefaultChargeCurrent` term
    */
    double getChargeCurrent();


  protected:
    const GunnsBMSSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
    const GunnsBMSSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

  private:
    GunnsElectConverterInput*   mBmsUpIn;           /**< (--) trick_chkpnt_io(**) Pointer to Converter Input */
    GunnsElectConverterOutput*  mBmsUpOut;          /**< (--) trick_chkpnt_io(**) Pointer to Converter Output */
    GunnsLosslessSource*        mBatterySource;     /**< (--) trick_chkpnt_io(**) Pointer to Lossless Source */

    double      mNetFluxFromBatt;                   /**< (--) trick_chkpnt_io(*io) Tracks the net flux into/out of the battery */
    double      mDefaultChargeCurrent;              /**< (--) trick_chkpnt_io(*io) The charge current when charging */

    double      mTotalDischargeTime;                /**< (--) trick_chkpnt_io(*io) Tracks the sim time spent discharging */
    double      mTotalChargeTime;                   /**< (--) trick_chkpnt_io(*io) Tracks the sim time spent charging */
    double      mCurrentStateTime;                  /**< (--) trick_chkpnt_io(*io) Tracks sim time since last state change */

    bool        mAutoThresholdsEnabled;             /**< *o (--) trick_chkpnt_io(**) if true -> auto enable charging/discharging based on battery SoC */

    BmsStatus   mStatus;                            /**< (--) trick_chkpnt_io(*io) Mode of BMS Operation {DISABLED, CHARGING, DISCHARGING} */

    void addFlux(const double dt);

};

///@}

#endif
