/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Three Way Solenoid Valve.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    (
     (aspects/fluid/conductor/GunnsFluid3WayValve.o)
    )

PROGRAMMERS:
   ((Mike Moore) (L3) (2012-04) (Addition to GUNNS Fluid Conductors))
**************************************************************************************************/

#include "aspects/fluid/signal/GunnsFluid3WaySolenoidValve.hh"
#include "core/GunnsFluidUtils.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--) Link name.
/// @param[in] nodes                 (--) Network nodes array.
/// @param[in] positionPathB         (--) Valve position is relative to flow path B (false = A).
/// @param[in] maxConductivityA      (m2) Path A maximum flow conductivity.
/// @param[in] maxConductivityB      (m2) Path B maximum flow conductivity.
/// @param[in] expansionScaleFactorA (--) Path A thermal expansion scale factor.
/// @param[in] expansionScaleFactorB (--) Path B thermal expansion scale factor.
/// @param[in] thermalLengthA        (m)  Path A thermal convection length.
/// @param[in] thermalLengthB        (m)  Path B thermal convection length.
/// @param[in] thermalDiameterA      (m)  Path A thermal convection diameter.
/// @param[in] thermalDiameterB      (m)  Path B thermal convection diameter.
/// @param[in] surfaceRoughnessA     (m)  Path A thermal surface roughness.
/// @param[in] surfaceRoughnessB     (m)  Path B thermal surface roughness.
/// @param[in] nonMixingBand         (--) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut.
/// @param[in] latching              (--) Boolean denoting whether the solenoid valve is latching
/// @param[in] openVoltage           (V)  Voltage threshold at which valve opens, aka pull in voltage
/// @param[in] openTime              (s)  Maximum time for solenoid valve to open, aka response time
/// @param[in] closeVoltage          (V)  Voltage threshold at which valve closes, aka dropout voltage
/// @param[in] closeTime             (s)  Maximum time for solenoid valve to close, aka response time
///
/// @details  Default GUNNS Three Way Solenoid Valve Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveConfigData::GunnsFluid3WaySolenoidValveConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const bool         positionPathB,
                                                             const double       maxConductivityA,
                                                             const double       maxConductivityB,
                                                             const double       expansionScaleFactorA,
                                                             const double       expansionScaleFactorB,
                                                             const double       thermalLengthA,
                                                             const double       thermalLengthB,
                                                             const double       thermalDiameterA,
                                                             const double       thermalDiameterB,
                                                             const double       surfaceRoughnessA,
                                                             const double       surfaceRoughnessB,
                                                             const double       nonMixingBand,
                                                             const bool         latching,
                                                             const double       openVoltage,
                                                             const double       openTime,
                                                             const double       closeVoltage,
                                                             const double       closeTime)
    :
    GunnsFluid3WayValveConfigData(name,
                                  nodes,
                                  positionPathB,
                                  maxConductivityA,
                                  maxConductivityB,
                                  expansionScaleFactorA,
                                  expansionScaleFactorB,
                                  thermalLengthA,
                                  thermalLengthB,
                                  thermalDiameterA,
                                  thermalDiameterB,
                                  surfaceRoughnessA,
                                  surfaceRoughnessB,
                                  nonMixingBand),
    mLatching(latching),
    mOpenVoltage(openVoltage),
    mOpenTime(openTime),
    mCloseVoltage(closeVoltage),
    mCloseTime(closeTime)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Gunns Three Way Solenoid Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveConfigData::GunnsFluid3WaySolenoidValveConfigData(
        const GunnsFluid3WaySolenoidValveConfigData& that)
    :
    GunnsFluid3WayValveConfigData(that),
    mLatching(that.mLatching),
    mOpenVoltage(that.mOpenVoltage),
    mOpenTime(that.mOpenTime),
    mCloseVoltage(that.mCloseVoltage),
    mCloseTime(that.mCloseTime)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Solenoid Valve Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveConfigData::~GunnsFluid3WaySolenoidValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position         (--) (0-1) Valve position relative to config data path.
/// @param[in] wallTemperatureA (K)  Path A wall temperature for thermal convection.
/// @param[in] wallTemperatureB (K)  Path B wall temperature for thermal convection.
/// @param[in] flux             (A)  Current through the solenoid coil.
/// @param[in] voltage          (V)  Voltage across the solenoid coil.
/// @param[in] malfStuckFlag    (--) Stuck at current position malfunction flag.
/// @param[in] malfFailToFlag   (--) Fail to position malfunction flag.
/// @param[in] malfFailToValue  (--) Fail to position malfunction value
///
/// @details  Default GUNNS Three Way Solenoid Valve Input Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveInputData::GunnsFluid3WaySolenoidValveInputData(const double position,
                                                                           const double wallTemperatureA,
                                                                           const double wallTemperatureB,
                                                                           const double flux,
                                                                           const double voltage,
                                                                           const bool   malfStuckFlag,
                                                                           const bool   malfFailToFlag,
                                                                           const double malfFailToValue)
    :
    GunnsFluid3WayValveInputData(position,
                                 wallTemperatureA,
                                 wallTemperatureB),
    mFlux(flux),
    mVoltage(voltage),
    mMalfStuckFlag(malfStuckFlag),
    mMalfFailToFlag(malfFailToFlag),
    mMalfFailToValue(malfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Three Way Solenoid Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveInputData::GunnsFluid3WaySolenoidValveInputData(const GunnsFluid3WaySolenoidValveInputData& that)
    :
    GunnsFluid3WayValveInputData(that),
    mFlux(that.mFlux),
    mVoltage(that.mVoltage),
    mMalfStuckFlag(that.mMalfStuckFlag),
    mMalfFailToFlag(that.mMalfFailToFlag),
    mMalfFailToValue(that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Solenoid Valve Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValveInputData::~GunnsFluid3WaySolenoidValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Three Way Solenoid Valve Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValve::GunnsFluid3WaySolenoidValve()
    :
    GunnsFluid3WayValve(),
    mFlux(0.0),
    mVoltage(0.0),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mLatching(false),
    mOpenVoltage(0.0),
    mOpenTime(0.0),
    mCloseVoltage(0.0),
    mCloseTime(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Three Way Solenoid Valve
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluid3WaySolenoidValve::~GunnsFluid3WaySolenoidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Three Way Solenoid Valve Config Data
/// @param[in]     inputData    (--) GUNNS Three Way Solenoid Valve Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
/// @param[in]     port2        (--) Network port 2
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Three Way Solenoid Valve with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::initialize(const GunnsFluid3WaySolenoidValveConfigData& configData,
                                             const GunnsFluid3WaySolenoidValveInputData&  inputData,
                                             std::vector<GunnsBasicLink*>&                networkLinks,
                                             const int                                    port0,
                                             const int                                    port1,
                                             const int                                    port2)
{
    /// - Reset the initialization complete flag.
    mInitFlag               = false;
    
    /// - First initialize & validate parent.
    GunnsFluid3WayValve::initialize(configData, inputData, networkLinks, port0, port1, port2);

    /// - Initialize from the configuration data.
    mLatching               = configData.mLatching;
    mOpenVoltage            = configData.mOpenVoltage;
    mOpenTime               = configData.mOpenTime;
    mCloseVoltage           = configData.mCloseVoltage;
    mCloseTime              = configData.mCloseTime;

    /// - Initialize from the input data.
    mFlux                   = inputData.mFlux;
    mVoltage                = inputData.mVoltage;

    /// - Initialize malfunctions off..
    mMalfStuckFlag     = false;
    mMalfFailToFlag    = false;
    mMalfFailToValue   = 0.0;

    /// - Validate the configuration and input data.
    validate();

    /// - Set the initialization complete flag.
    mInitFlag               = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Three Way Solenoid Valve initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::validate() const
{
    /// - Throw an exception if open voltage < 0.
    if (mOpenVoltage < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Open voltage < 0.");
    }
    
    /// - Throw an exception if close voltage < 0.
    if (mCloseVoltage < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Close voltage < 0.");
    }

    /// - Throw an exception if open pressure less than close pressure.
    if (mOpenVoltage < mCloseVoltage) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Open voltage < close voltage.");
    }

    /// - Throw an exception if open time < 0.
    if (mOpenTime <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Open time <= 0.");
    }
    
    /// - Throw an exception if close time < 0.
    if (mCloseTime <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Close time <= 0.");
    }
    
    /// - Throw an exception if voltage < 0.
    if (mVoltage < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Voltage < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @throws   TsOutOfBoundsException
///
/// @details  Calls step on the Three Way Solenoid Valve to update the state and compute a new conductivity
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Update the flow path port maps to match the parent link.
    if (mPathA.setPort(0, mNodeMap[0]) and mPathA.setPort(1, mNodeMap[2]) and
        mPathB.setPort(0, mNodeMap[1]) and mPathB.setPort(1, mNodeMap[2])) {

        /// - Update the flow paths.
        updateState(dt);

    /// - This link should prevent port assignment violations from getting as far as the fluid valve
    ///   objects, but just in case any reject their assignments, throw an exception.
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "child valve rejected port assignment.");
    }

    /// - This link has no direct contributions to the network system of equations, since the path
    ///   A & B sub valves are orchestrated directly by the solver.  Force this link's contributions
    ///   to the system of equations to zero.
    for (int i=0; i<9; ++i) {
        mAdmittanceMatrix[i] = 0.0;
    }
    mSourceVector[0] = 0.0;
    mSourceVector[1] = 0.0;
    mSourceVector[2] = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  Drives the path A & B sub-valves positions from this 3-way position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluid3WaySolenoidValve::updateState(const double dt)
{
    /// - Handle stuck malfunction
    if (!mMalfStuckFlag) {
        if (mMalfFailToFlag) {
            /// - Handle fail to position malfunction with range limiting.
            mPosition                     = MsMath::limitRange(0.0, mMalfFailToValue, 1.0);
        } else {
            const double previousPosition = mPosition;
            if (!mLatching) {
                if (mVoltage >= mOpenVoltage) {
                    /// - The position is fully open (1.0) if delta V across the valve is large enough.
                    mPosition                 = 1.0;
                } else if (mVoltage <= mCloseVoltage) {
                    /// - The position is fully closed (0.0) if delta V across the valve is small enough.
                    mPosition                 = 0.0;
                } else {
                    /// - Otherwise the position transitions (0.0 to 1.0) linearly in delta V.
                    mPosition                 = (mVoltage - mCloseVoltage) / (mOpenVoltage - mCloseVoltage);
                }

                /// - Apply range and rate limiting to the computed position.
                mPosition                     = MsMath::limitRange(std::max(0.0, previousPosition - dt / mCloseTime),
                                                                   mPosition,
                                                                   std::min(1.0, previousPosition + dt / mOpenTime));
            } else {
                if (mVoltage >= mOpenVoltage && mFlux > 0) {
                    /// - The position is fully open (1.0) if delta V across the valve is large enough and current is positive.
                    mPosition                 = 1.0;
                } else if (mVoltage >= mOpenVoltage && mFlux < 0) {
                    /// - The position is fully closed (0.0) if delta V across the valve is large enough and current is positive.
                    mPosition                 = 0.0;
                } else {
                    /// - Otherwise the position transitions (0.0 to 1.0) linearly in delta V.
                    mPosition                 = (mVoltage - mCloseVoltage) / (mOpenVoltage - mCloseVoltage);
                }

                /// - Apply range and rate limiting to the computed position.
                mPosition                     = MsMath::limitRange(std::max(0.0, previousPosition - dt / mCloseTime),
                                                                   mPosition,
                                                                   std::min(1.0, previousPosition + dt / mOpenTime));
            }
        }
    }
    /// - Call parent updateState to apply valve malfunctions and update the effective conductivity.
    GunnsFluid3WayValve::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  The path A & B sub-valve links perform their own functions.  This parent link has no
///           flows itself, so we set all port directions to indicate no flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::computeFlows(const double dt __attribute__((unused)))
{
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    mPortDirections[2] = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] latching  (--) latching state 
///
/// @details  Sets whether the valve is latching
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::setLatching(const bool latching)
{
    mLatching = latching;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flux  (--) Flux through the coil
///
/// @details  Sets the flux to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::setFlux(const double flux)
{
    mFlux = flux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage  (--) Voltage over the coil
///
/// @details  Sets the voltage to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::setVoltage(const double voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
///
/// @details  Sets the stuck malf flag to given the value.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluid3WaySolenoidValve::setMalfStuck(const bool flag)
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
void GunnsFluid3WaySolenoidValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}


