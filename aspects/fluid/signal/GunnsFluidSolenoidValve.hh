#ifndef GunnsFluidSolenoidValve_EXISTS
#define GunnsFluidSolenoidValve_EXISTS

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
- ((GunnsFluidSolenoidValve.o))

PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-05))

@{
*/

#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/fluid/conductor/GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Solenoid Valve Model configuration data
///
/// @note:    A derived class may add position scale, bias to support other units or a convert
///           function pointer to support non-linearity between valve cmd/position/conductivity.
///
/// @details  The sole purpose of this class is to provide a data structure for the Solenoid Valve
///           model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSolenoidValveConfigData : public GunnsFluidValveConfigData
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
                                          const double       openVoltage          = 0.0,
                                          const double       openTime             = 0.0,
                                          const double       closeVoltage         = 0.0,
                                          const double       closeTime            = 0.0);
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
class GunnsFluidSolenoidValveInputData : public GunnsFluidValveInputData
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
                                         const double voltage             = 0.0,
                                         const bool   malfStuckFlag       = false,
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
class GunnsFluidSolenoidValve : public GunnsFluidValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSolenoidValve);
    public:
        double        mVoltage;         /**< (--)   Current voltage over the coil. Updated through simbus */
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
inline double GunnsFluidSolenoidValve::getVoltage() const
{
    return mVoltage;
}

#endif
