#ifndef GunnsPVSpotter_EXISTS
#define GunnsPVSpotter_EXISTS

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
#include "aspects/electrical/SolarArray/GunnsElectPvArray.hh"
#include "aspects/electrical/SolarArray/GunnsElectPvRegConv.hh"
#include "aspects/electrical/Switch/SwitchElect.hh"

#include "ax/elect/GunnsLosslessSource.hh"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PV Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsPVSpotterConfigData : public GunnsNetworkSpotterConfigData
{
  public:

    GunnsElectPvArray*     mArray;
    GunnsElectPvRegConv*   mReg;
    SwitchElect*                   mSwitch;
    GunnsLosslessSource*   mSource;
    

    GunnsPVSpotterConfigData(const std::string& name,
                              GunnsElectPvArray*    array,
                              GunnsElectPvRegConv*  reg,
                              SwitchElect*          sswitch,
                              GunnsLosslessSource*  source);
    virtual ~GunnsPVSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PV Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsPVSpotterInputData : public GunnsNetworkSpotterInputData
{
  public:
    double  mStartingNetFluxFromBatt;
    double  mLowSocCutoff;
    double  mHighSocCutoff;
    double  mDefaultChargeCurrent;
    bool    mAutoThresholdsEnabled;

    GunnsPVSpotterInputData(double startingFluxFromBatt, double lowSocCutoff, double highSocCutoff, double defaultChargeCurrent, bool autoThresholdsEnabled);
    virtual ~GunnsPVSpotterInputData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Derived PV Spotter Class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsPVSpotter : public GunnsNetworkSpotter
{
  TS_MAKE_SIM_COMPATIBLE(GunnsPVSpotter);
  public:
    /// @brief  Enumeration of BMS States
    enum PVStatus {
      DISABLED    = 0,
      MANUAL      = 1,
      AUTOMATIC   = 2,
    };

    std::string returnStatus() {
      switch (this->mStatus){
      case PVStatus::DISABLED:
        return "DISABLED";
        break;
      case PVStatus::MANUAL:
        return "MANUAL";
        break;
      case PVStatus::AUTOMATIC:
        return "AUTOMATIC";
        break;
      default:
        break;
      }
    }
    
    GunnsPVSpotter();
    virtual     ~GunnsPVSpotter() {;}
    virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                            const GunnsNetworkSpotterInputData*  inputData);
    virtual void stepPreSolver(const double dt);
    virtual void stepPostSolver(const double dt);

    void enableCharging();
    void disableCharging();
    void enableDischarging();
    void disableDischarging();

    bool isAutomatic();
    bool isManual();
    PVStatus isInvalid();

    void updateStatus();
    void updateChargeCurrent(const double newCurrent);

    GunnsElectPvArray* mArray;

    PVStatus mNextCommandedStatus;
    bool      mOverrideStatus;


  protected:
    const GunnsPVSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
    const GunnsPVSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

  private:
    GunnsElectPvRegConv*   mReg;
    SwitchElect*           mSwitch;
    GunnsLosslessSource*   mSource;

    double      mNetFluxFromBatt;
    double      mLowSocCutoff;
    double      mHighSocCutoff;
    double      mDefaultChargeCurrent;

    double      mTotalDischargeTime;
    double      mTotalChargeTime;
    double      mCurrentStateTime;

    bool        mAutoThresholdsEnabled;             /**< *o (--) trick_chkpnt_io(**) if bool -> auto enable charging/discharging based on array SoC */

    PVStatus   mStatus;

    void addFlux(const double dt);

};

///@}

#endif
