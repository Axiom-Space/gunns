#ifndef GunnsFluid3WaySolenoidValve_EXISTS
#define GunnsFluid3WaySolenoidValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_THREE_WAY_SOLENOID_VALVE    Three Way Solenoid Valve Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Three Way Solenoid Valve.  This consists of two normal valves that share the same exit
   port.  Their positions are coordinated by this parent class to sum to 1.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluid3WaySolenoidValve.o))

PROGRAMMERS:
- (
   (Kyle Fondon)   (L3) (2011-02) (Added to GUNNS Fluid Conductors)
  )

@{
*/

#include "aspects/fluid/conductor/GunnsFluid3WayValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Solenoid Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Three Way Solenoid Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WaySolenoidValveConfigData : public GunnsFluid3WayValveConfigData
{
    public:
        bool   mLatching;              /**< (--) trick_chkpnt_io(**) Boolean denoting whether the solenoid valve is latching. */
        double mOpenVoltage;           /**< (--) trick_chkpnt_io(**) Voltage threshold at which valve opens, aka pull in voltage. */
        double mOpenTime;              /**< (--) trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        double mCloseVoltage;          /**< (--) trick_chkpnt_io(**) Voltage threshold at which valve closes, aka dropout voltage. */
        double mCloseTime;             /**< (--) trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        /// @brief Default constructs this Three Way Solenoid Valve configuration data.
        GunnsFluid3WaySolenoidValveConfigData(const std::string& name                  = "",
                                              GunnsNodeList*     nodes                 = 0,
                                              const bool         positionPathB         = false,
                                              const double       maxConductivityA      = 0.0,
                                              const double       maxConductivityB      = 0.0,
                                              const double       expansionScaleFactorA = 0.0,
                                              const double       expansionScaleFactorB = 0.0,
                                              const double       thermalLengthA        = 0.0,
                                              const double       thermalLengthB        = 0.0,
                                              const double       thermalDiameterA      = 0.0,
                                              const double       thermalDiameterB      = 0.0,
                                              const double       surfaceRoughnessA     = 0.0,
                                              const double       surfaceRoughnessB     = 0.0,
                                              const double       nonMixingBand         = 0.0,
                                              const bool         latching              = false,
                                              const double       openVoltage           = 0.0,
                                              const double       openTime              = 0.0,
                                              const double       closeVoltage          = 0.0,
                                              const double       closeTime             = 0.0);
        /// @brief Copy constructs this Three Way Solenoid Valve configuration data.
        GunnsFluid3WaySolenoidValveConfigData(const GunnsFluid3WaySolenoidValveConfigData& that);
        /// @brief Default destructs this Three Way Solenoid Valve configuration data.
        virtual ~GunnsFluid3WaySolenoidValveConfigData();
    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WaySolenoidValveConfigData& operator =(const GunnsFluid3WaySolenoidValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Solenoid Valve Input Data
///
/// @details  This provides a data structure for the Three Way Solenoid Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WaySolenoidValveInputData : public GunnsFluid3WayValveInputData
{
    public:
        double        mFlux;            /**< (--)   Current through the coil. Updated through simbus */
        double        mVoltage;         /**< (--)   Voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief Default constructs this Three Way Solenoid Valve input data.
        GunnsFluid3WaySolenoidValveInputData(const double position         = 0.0,
                                             const double wallTemperatureA = 0.0,
                                             const double wallTemperatureB = 0.0,
                                             const double flux             = 0.0,
                                             const double voltage          = 0.0,
                                             const bool   malfStuckFlag    = false,
                                             const bool   malfFailToFlag   = false,
                                             const double malfFailToValue  = 0.0);
        /// @brief Copy constructs this Three Way Solenoid Valve input data.
        GunnsFluid3WaySolenoidValveInputData(const GunnsFluid3WaySolenoidValveInputData& that);
        /// @brief Default destructs this Three Way Solenoid Valve input data.
        virtual ~GunnsFluid3WaySolenoidValveInputData();
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WaySolenoidValveInputData& operator =(const GunnsFluid3WaySolenoidValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Three Way Solenoid Valve
///
/// @details  This class models the fluid aspect of a three-way valve.  It has 3 ports - two inputs
///           and an exit.  The link models the two flow paths of the inputs to the exit like a "V"
///           shape, where the tops of the V are the input ports and the vertex is the exit port.
///           Inputs A and B each flow to the exit, but not directly between each other.  The node
///           attached to the exit port is where the A & B flow paths mix.  Flow can go in either
///           direction in each path independently of the other, based on the relative pressures at
///           all three ports.  Flow is defined positive going from an input port to the exit, and
///           negative going from the exit port to an input port.
///
///           This class embeds two normal two-way valves to represent these flow paths.  A single
///           overall 3-way valve position governs the relative positions of both 2-ways.  The 3-way
///           position can be defined relative to either path A or B (default A), and the other
///           path's position is calculated such that (path A position) + (path B position) = 1.
///           Intermediate positions partially open both paths, allowing both paths to flow and mix
///           to or split from the exit.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluid3WaySolenoidValve : public GunnsFluid3WayValve
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluid3WaySolenoidValve);

    public:

        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        double        mFlux;            /**< (--)   Current through the coil. Updated through simbus */
        double        mVoltage;         /**< (--)   Voltage over the coil. Updated through simbus */
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @}

        /// @brief Default constructor.
        GunnsFluid3WaySolenoidValve();
        /// @brief Default destructor.
        virtual ~GunnsFluid3WaySolenoidValve();
        /// @brief Initialize method.
        void initialize(const GunnsFluid3WaySolenoidValveConfigData& configData,
                        const GunnsFluid3WaySolenoidValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                networkLinks,
                        const int                                    port0,
                        const int                                    port1,
                        const int                                    port2);
        /// @brief Steps the 3-way valve.
        virtual void step(const double dt);
        /// @brief Computes flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Updates the State of the 3-way valve.
        virtual void updateState(const double dt);
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
        double        mOpenVoltage;     /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve opens, aka pull in voltage. */
        double        mOpenTime;        /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        double        mCloseVoltage;    /**< (--)  trick_chkpnt_io(**) Voltage threshold at which valve closes, aka dropout voltage. */
        double        mCloseTime;       /**< (--)  trick_chkpnt_io(**) Maximum time for solenoid valve to open, aka response time. */
        /// @brief Validates the initialization of this Gunns Three Way Solenoid Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 3};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluid3WaySolenoidValve(const GunnsFluid3WaySolenoidValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluid3WaySolenoidValve& operator =(const GunnsFluid3WaySolenoidValve& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Latching state.
///
/// @details  Gets whether this GUNNS Fluid 3Way Solenoid Valve is latching.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluid3WaySolenoidValve::getLatching() const
{
    return mLatching;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Coil current.
///
/// @details  Gets the current through the coil of this GUNNS Fluid 3Way Solenoid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluid3WaySolenoidValve::getFlux() const
{
    return mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- Coil voltage.
///
/// @details  Gets the voltage across the coil of this GUNNS Fluid 3Way Solenoid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluid3WaySolenoidValve::getVoltage() const
{
    return mVoltage;
}

#endif