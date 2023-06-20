#ifndef GunnsFluidHiFiSolenoidValve_EXISTS
#define GunnsFluidHiFiSolenoidValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_Solenoid_VALVE    Solenoid Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Solenoid Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidHiFiSolenoidValve.o))

PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-05))

@{
*/

#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/fluid/hi-fi/GunnsFluidHiFiValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model configuration data
///
/// @note:    A derived class may add position scale, bias to support other units or a convert
///           function pointer to support non-linearity between valve cmd/position/conductivity.
///
/// @details  The sole purpose of this class is to provide a data structure for the Solenoid Valve
///           model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiSolenoidValveConfigData : public GunnsFluidHiFiValveConfigData
{
    public:
        bool          mLatching;        /**< (--)  trick_chkpnt_io(**) Boolean denoting whether the solenoid valve is latching. */
        double        mOpenVoltage;    /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve opens, aka pull in voltage. */
        double        mOpenTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        double        mCloseVoltage;  /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve closes, aka dropout voltage. */
        double        mCloseTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        /// @brief    Default constructs this Solenoid Valve model configuration data.
        GunnsFluidHiFiSolenoidValveConfigData(const std::string& name                 = "",
                                              GunnsNodeList*     nodes                = 0,
                                              const CoeffTypes   coefficientType      = DISCHARGE_COEFF,
                                              const double       coefficientValue     = 0.0,
                                              const double       throatDiameter       = 0.0,
                                              const double       criticalReynolds     = 0.0,
                                              const double       expansionScaleFactor = 0.0,
                                              const double       flowTuningFactor     = 1.0,
                                              const bool         latching             = false,
                                              const double       openVoltage          = 0.0,
                                              const double       openTime             = 0.0,
                                              const double       closeVoltage         = 0.0,
                                              const double       closeTime            = 0.0);
        /// @brief    Copy constructs this Solenoid Valve model configuration data.
        GunnsFluidHiFiSolenoidValveConfigData(const GunnsFluidHiFiSolenoidValveConfigData& that);
        /// @brief    Default destructs this Solenoid Valve model configuration data.
        virtual ~GunnsFluidHiFiSolenoidValveConfigData();
    private:
        /// @brief    Assigns this Solenoid Valve model configuration data.
        GunnsFluidHiFiSolenoidValveConfigData& operator=(const GunnsFluidHiFiSolenoidValveConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model input data
///
/// @details  The sole purpose of this class is to provide a data structure for the Solenoid Valve
///           model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiSolenoidValveInputData : public GunnsFluidHiFiValveInputData
{
    public:
        double        mFlux;            /**< (--)   Current through the coil. Updated through simbus */
        double        mVoltage;         /**< (--)   Current voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Solenoid Valve model input data.
        GunnsFluidHiFiSolenoidValveInputData(const bool   malfBlockageFlag  = false,
                                             const double malfBlockageValue = 0.0,
                                             const double position          = 0.0,
                                             const bool   malfLeakThruFlag  = false,
                                             const double malfLeakThruValue = 0.0,
                                             const double flux              = 0.0,
                                             const double voltage           = 0.0,
                                             const bool   malfStuckFlag     = false,
                                             const bool   malfFailToFlag    = false,
                                             const double malfFailToValue   = 0.0);
        /// @brief    Copy constructs this Solenoid Valve model input data.
        GunnsFluidHiFiSolenoidValveInputData(const GunnsFluidHiFiSolenoidValveInputData& that);
        /// @brief    Default destructs this Solenoid Valve model input data.
        virtual ~GunnsFluidHiFiSolenoidValveInputData();
    private:
        /// @brief    Assigns this Solenoid Valve model input data.
        GunnsFluidHiFiSolenoidValveInputData& operator =(const GunnsFluidHiFiSolenoidValveInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model
///
/// @details  Provides the base class for a Solenoid Valve with manual position and malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiSolenoidValve : public GunnsFluidHiFiValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHiFiSolenoidValve);
    public:
        double        mFlux;            /**< (--)   Current through the coil. Updated through simbus */
        double        mVoltage;         /**< (--)   Current voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Solenoid Valve model.
        GunnsFluidHiFiSolenoidValve();
        /// @brief    Default destructs this Solenoid Valve model.
        virtual ~GunnsFluidHiFiSolenoidValve();
        /// @brief    Initializes this Solenoid Valve model.
        /// @callgraph
        void initialize(const GunnsFluidHiFiSolenoidValveConfigData&    configData,
                        const GunnsFluidHiFiSolenoidValveInputData&     initData,
                        std::vector<GunnsBasicLink*>&                   networkLinks,
                        const int                                       port0,
                        const int                                       port1);
        /// @brief Gets latching
        bool getLatching() const;
        /// @brief Sets latching
        void setLatching(const bool latching = false);
        /// @brief Gets the flux
        double getFlux() const;
        /// @brief Sets the flux
        void setFlux(const double flux = 0.0);
        /// @brief Gets the voltage
        double getVoltage() const;
        /// @brief Sets the voltage
        void setVoltage(const double voltage = 0.0);
        /// @brief Sets and resets the stuck malfunction
        void setMalfStuck(const bool flag = false);
        /// @brief Sets and resets the fail to position malfunction
        void setMalfFailTo(const bool flag = false, const double value = 0.0);
    protected:
        bool          mLatching;        /**< (--)  trick_chkpnt_io(**) Boolean denoting whether the solenoid valve is latching. */
        double        mOpenVoltage;    /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve opens, aka pull in voltage. */
        double        mOpenTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        double        mCloseVoltage;  /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve closes, aka dropout voltage. */
        double        mCloseTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        /// @brief    Validates this Solenoid Valve model initialization data.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates this Solenoid Valve model fractional position.
        void updateState(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiSolenoidValve(const GunnsFluidHiFiSolenoidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiSolenoidValve operator= (const GunnsFluidHiFiSolenoidValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Latching state.
///
/// @details  Gets whether this GUNNS Fluid Solenoid Valve is latching.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidHiFiSolenoidValve::getLatching() const
{
    return mLatching;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Coil current.
///
/// @details  Gets the current through the coil of this GUNNS Fluid Solenoid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHiFiSolenoidValve::getFlux() const
{
    return mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Coil voltage.
///
/// @details  Gets the voltage across the coil of this GUNNS Fluid Solenoid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHiFiSolenoidValve::getVoltage() const
{
    return mVoltage;
}

#endif
