#ifndef GunnsPVSpotter_EXISTS
#define GunnsPVSpotter_EXISTS

/**
@file
@brief     GUNNS PV Spotter declarations

@defgroup  TSM_GUNNS_ELECT_SPOTTER_PV_SPOTTER    GUNNS Elect PV Spotter
@ingroup   TSM_GUNNS_ELECT_SPOTTER

@copyright 

PURPOSE:   (Provides the classes for the GUNNS PV Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsPVSpotter.o))

PROGRAMMERS:
- (
  (Tristan Mansfield) (Axiom Space) (2023-09) (Initial)
  )

@{
*/


#include "core/GunnsNetworkSpotter.hh"
#include "core/GunnsBasicConductor.hh"
#include "aspects/electrical/SolarArray/GunnsElectPvArray.hh"
#include "aspects/electrical/SolarArray/GunnsElectPvRegConv.hh"
#include "aspects/electrical/Switch/SwitchElect.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"

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
    SwitchElect*           mSwitch;
    GunnsLosslessSource*   mSource;
    GunnsElectConverterOutput* mConvOut;
    

    GunnsPVSpotterConfigData(const std::string& name,
                              GunnsElectPvArray*    array,
                              GunnsElectPvRegConv*  reg,
                              GunnsLosslessSource*  source,
                              SwitchElect*          sswitch,
                              GunnsElectConverterOutput* convOut);
    virtual ~GunnsPVSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PV Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO_ Tristan Mansfield Change this one
class GunnsPVSpotterInputData : public GunnsNetworkSpotterInputData
{
  public:

    GunnsPVSpotterInputData();
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

    void enableManualControl();
    void disableManualControl();
    void enableAutomaticControl();
    void disableAutomaticControl();
    void disable();

    bool isAutomatic();
    bool isManual();
    bool isDisabled();

    void updateStatus();

    double getConvOutEff();

    GunnsElectPvArray*         mArray;      /**< (--) trick_chkpnt_io(**) Pointer to PV array */
    GunnsElectPvRegConv*       mReg;        /**< (--) trick_chkpnt_io(**) Pointer to PV Regulator */
    SwitchElect*               mSwitch;     /**< (--) trick_chkpnt_io(**) Pointer to PV Switch */
    GunnsLosslessSource*       mSource;     /**< (--) trick_chkpnt_io(**) Pointer to PV Lossless Source */
    GunnsElectConverterOutput* mConvOut;    /**< (--) trick_chkpnt_io(**) Pointer to PV Converter Output */

    PVStatus mNextCommandedStatus;          /**< (--) trick_chkpnt_io(**) If mOverrideStatus is true, mStatus will change to this status */
    bool     mOverrideStatus;               /**< (--) trick_chkpnt_io(**) Bool, if true, updates mStatus and clears */

    /// @brief Changes mPVCalcCurrent to be equal to this, assuming it's positive
    /// @param newSourceCurrent : The new current to source from the PVs, regardless of if it's possible
    void updateSourceCurrent(double newSourceCurrent);


  protected:
    const GunnsPVSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
    const GunnsPVSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

  private:

    /// @brief  This needs to be updated constantly to weigh the max available power from this PV and the power demand from on high
    double mPVCalcCurrent;  /**< (--) trick_chkpnt_io(*io) current sourced from PV when in MANUAL mode */

    PVStatus   mStatus;     /**< (--) trick_chkpnt_io(*io) Mode of PV operation {DISABLED, MANUAL, AUTOMATIC} */

    bool mBothEnabledLastStep; /**< (--) trick_chkpnt_io(**) Internal flag */


};

///@}

#endif
