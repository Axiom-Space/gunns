#ifndef GunnsFluidSolenoidValve_EXISTS
#define GunnsFluidSolenoidValve_EXISTS

/**
@file
@brief     TS21 Fluid Controller Fluid (Valve, Fan, Pump) Controllers declarations.

@defgroup  TSM_CONTROLLER_FLUID   Fluid (Valve, Fan, Pump) Controllers
@ingroup   TSM_CONTROLLER
@defgroup  TSM_CONTROLLER_FLUID_VALVE_CONTROLLER   Base (Manual) Solenoid Valve
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: ((Base class for TS21 Fluid Controller Solenoid Valves with position and malfunction.)

@details
   (The simbus will update the GUNNS fluid valve position with the mFluidPosition attribute from this class.
    Malfunctions support latching to the current position and failing to a specified position.))

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ((GunnsFluidSolenoidValve.o))

 PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-05)

@{
*/

#include <string>

#include "aspects/fluid/GunnsFluidValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model configuration data
///
/// @note:    A derived class may add position scale, bias to support other units or a convert
///           function pointer to support non-linearity between valve cmd/position/conductivity.
///
/// @details  The sole purpose of this class is to provide a data structure for the Solenoid Valve
///           model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSolenoidValveConfigData : GunnsFluidValveConfigData
{
    public:
        double        mOpenVoltage;    /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve opens, aka pull in voltage. */
        double        mOpenTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        double        mCloseVoltage;  /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve closes, aka dropout voltage. */
        double        mCloseTime;  /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        /// @brief    Default constructs this Solenoid Valve model configuration data.
        GunnsFluidSolenoidValveConfigData(const std::string& name                 = "",
                                          GunnsNodeList*     nodes                = 0,
                                          const double       maxConductivity      = 0.0,
                                          const double       expansionScaleFactor = 0.0,
                                          const double       thermalLength        = 0.0,
                                          const double       thermalDiameter      = 0.0,
                                          const double       surfaceRoughness     = 0.0,
                                          const double       mOpenVoltage         = 0.0,
                                          const double       mOpenTime            = 0.0,
                                          const double       mCloseVoltage        = 0.0,
                                          const double       mCloseTime           = 0.0);
        /// @brief    Copy constructs this Solenoid Valve model configuration data.
        GunnsFluidSolenoidValveConfigData(const GunnsFluidSolenoidValveConfigData& that);
        /// @brief    Default destructs this Solenoid Valve model configuration data.
        virtual ~GunnsFluidSolenoidValveConfigData();
    private:
        /// @brief    Assigns this Solenoid Valve model configuration data.
        GunnsFluidSolenoidValveConfigData& operator=(const GunnsFluidSolenoidValveConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model input data
///
/// @details  The sole purpose of this class is to provide a data structure for the Solenoid Valve
///           model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSolenoidValveInputData : GunnsFluidValveInputData
{
    public:
        double        mVoltage;         /**< (--)   Current voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Solenoid Valve model input data.
        GunnsFluidSolenoidValveInputData(const bool   malfBlockageFlag    = false,
                                         const double malfBlockageValue   = 0.0,
                                         const double position            = 0.0,
                                         const bool   malfLeakThruFlag    = false,
                                         const double malfLeakThruValue   = 0.0,
                                         const double wallTemperature     = 0.0,
                                         const double mVoltage            = 0.0,
                                         const bool   mMalfStuckFlag      = false,
                                         const bool   malfFailToFlag      = false,
                                         const double malfFailToValue     = 0.0);
        /// @brief    Copy constructs this Solenoid Valve model input data.
        GunnsFluidSolenoidValveInputData(const GunnsFluidSolenoidValveInputData& that);
        /// @brief    Default destructs this Solenoid Valve model input data.
        virtual ~GunnsFluidSolenoidValveInputData();
    private:
        /// @brief    Assigns this Solenoid Valve model input data.
        GunnsFluidSolenoidValveInputData& operator =(const GunnsFluidSolenoidValveInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model
///
/// @details  Provides the base class for a Solenoid Valve with manual position and malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSolenoidValve : GunnsFluidValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSolenoidValve);
    public:
        double        mVoltage;          /**< (--)   Current voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Solenoid Valve model.
        GunnsFluidSolenoidValve();
        /// @brief    Default destructs this Solenoid Valve model.
        virtual ~GunnsFluidSolenoidValve();
        /// @brief    Initializes this Solenoid Valve model.
        /// @callgraph
        void initialize(const GunnsFluidSolenoidValveConfigData&   configData,
                        const GunnsFluidSolenoidValveInputData&    initData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0,
                        const int                                  port1);
        /// @brief    Updates this Solenoid Valve model.
        /// @callgraph
        virtual void update(const double dt);
        /// @brief Gets the voltage
        double getVoltage() const;
        /// @brief Sets the voltage
        void setVoltage(const double voltage = 0.0);
        /// @brief Sets and resets the stuck malfunction
        void setMalfStuck(const bool flag = false);
        /// @brief Sets and resets the fail to position malfunction
        void setMalfFailTo(const bool flag = false, const double value = 0.0);
    protected:
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
        GunnsFluidSolenoidValve(const GunnsFluidSolenoidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSolenoidValve operator= (const GunnsFluidSolenoidValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Coil voltage.
///
/// @details  Gets the voltage across the coil of this GUNNS Fluid Solenoid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidValve::getVoltage() const
{
    return mVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage  (--) Voltage over the coil
///
/// @details  Sets the voltage to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::setVoltage(const bool voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
///
/// @details  Sets the stuck malf flag to given the value.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::setMalfStuck(const bool flag)
{
    mMalfStuckFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) Fail to position malfunction value
///
/// @details  Sets the fail to position malf parameters to given the values.  Calling this method
///           with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}

#endif
