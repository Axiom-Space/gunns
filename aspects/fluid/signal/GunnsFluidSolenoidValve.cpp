/************************** TRICK HEADER ***********************************************************
LIBRARY DEPENDENCY:
 ((aspects/fluid/conductor/GunnsFluidValve.o))
***************************************************************************************************/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <algorithm>

#include "GunnsFluidSolenoidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 (--)    Name of object.
/// @param[in]  nodes                (--)    Pointer to nodes.
/// @param[in]  maxConductivity      (m2)    Maximum conductivity.
/// @param[in]  expansionScaleFactor (--)    Scale factor for isentropic gas cooling.
/// @param[in]  thermalLength        (m)     Tube length for thermal convection
/// @param[in]  thermalDiameter      (m)     Tube inner diameter for thermal convection
/// @param[in]  surfaceRoughness     (m)     Tube wall surface roughness for thermal convection
/// @param[in]  latching             (--)    Boolean denoting whether the solenoid valve is latching
/// @param[in]  openVoltage          (V)     Voltage threshold at which valve opens, aka pull in voltage
/// @param[in]  openTime             (s)     Maximum time for solenoid valve to open, aka response time
/// @param[in]  closeVoltage         (V)     Voltage threshold at which valve closes, aka dropout voltage
/// @param[in]  closeTime            (s)     Maximum time for solenoid valve to close, aka response time
///
/// @details    Default constructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveConfigData::GunnsFluidSolenoidValveConfigData(const std::string& name,
                                                                     GunnsNodeList*     nodes,
                                                                     const double       maxConductivity,
                                                                     const double       expansionScaleFactor,
                                                                     const double       thermalLength,
                                                                     const double       thermalDiameter,
                                                                     const double       surfaceRoughness,
                                                                     const bool         latching,
                                                                     const double       openVoltage,
                                                                     const double       openTime,
                                                                     const double       closeVoltage,
                                                                     const double       closeTime)
    :
    GunnsFluidValveConfigData(name,
                              nodes,
                              maxConductivity,
                              expansionScaleFactor,
                              thermalLength,
                              thermalDiameter,
                              surfaceRoughness),
    mLatching(latching),
    mOpenVoltage(openVoltage),
    mOpenTime(openTime),
    mCloseVoltage(closeVoltage),
    mCloseTime(closeTime)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveConfigData::GunnsFluidSolenoidValveConfigData(const GunnsFluidSolenoidValveConfigData& that)
    :
    GunnsFluidValveConfigData(that),
    mLatching(that.mLatching),
    mOpenVoltage(that.mOpenVoltage),
    mOpenTime(that.mOpenTime),
    mCloseVoltage(that.mCloseVoltage),
    mCloseTime(that.mCloseTime)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveConfigData::~GunnsFluidSolenoidValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag       (--)    Blockage malfunction flag.
/// @param[in]  malfBlockageValue      (--)    Blockage malfunction fractional value (0-1).
/// @param[in]  position               (--)    Fractional position.
/// @param[in]  malfLeakThruFlag       (--)    Leak rate malfunction flag.
/// @param[in]  malfLeakThruValue      (kg/s)  Leak rate malfunction value.
/// @param[in]  wallTemperature        (K)     Tube wall temperature for thermal convection.
/// @param[in]  flux                   (A)     Current through the solenoid coil.
/// @param[in]  voltage                (V)     Voltage across the solenoid coil.
/// @param[in]  malfStuckFlag          (--)    Stuck at current position malfunction flag.
/// @param[in]  malfFailToFlag         (--)    Fail to position malfunction flag.
/// @param[in]  malfFailToValue        (--)    Fail to position malfunction value
///
/// @details    Default constructs this Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveInputData::GunnsFluidSolenoidValveInputData(const bool   malfBlockageFlag,
                                                                   const double malfBlockageValue,
                                                                   const double position,
                                                                   const bool   malfLeakThruFlag,
                                                                   const double malfLeakThruValue,
                                                                   const double wallTemperature,
                                                                   const double flux,
                                                                   const double voltage,
                                                                   const bool   malfStuckFlag,
                                                                   const bool   malfFailToFlag,
                                                                   const double malfFailToValue)
    :
    GunnsFluidValveInputData(malfBlockageFlag,
                             malfBlockageValue,
                             position,
                             malfLeakThruFlag,
                             malfLeakThruValue,
                             wallTemperature),
    mFlux(flux),
    mVoltage(voltage),
    mMalfStuckFlag(malfStuckFlag),
    mMalfFailToFlag(malfFailToFlag),
    mMalfFailToValue(malfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveInputData::GunnsFluidSolenoidValveInputData(const GunnsFluidSolenoidValveInputData& that)
    :
    GunnsFluidValveInputData(that),
    mFlux(that.mFlux),
    mVoltage(that.mVoltage),
    mMalfStuckFlag(that.mMalfStuckFlag),
    mMalfFailToFlag(that.mMalfFailToFlag),
    mMalfFailToValue(that.mMalfFailToValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Valve Controller model Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveInputData::~GunnsFluidSolenoidValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValve::GunnsFluidSolenoidValve()
    :
    GunnsFluidValve(),
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
/// @details    Default destructs this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValve::~GunnsFluidSolenoidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
/// @param[in,out]  links        --  Link vector.
/// @param[in]      port0        --  Nominal inlet port map index.
/// @param[in]      port1        --  Nominal outlet port map index.
///
/// @return     voidunused
///
/// @throws     TsInitializationException
///
/// @details    Initializes this Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::initialize(const GunnsFluidSolenoidValveConfigData& configData,
                                         const GunnsFluidSolenoidValveInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            links,
                                         const int                                port0,
                                         const int                                port1)
{
    /// - Reset the initialization complete flag.
    mInitFlag               = false;
    
    /// - First initialize & validate parent.
    GunnsFluidValve::initialize(configData, inputData, links, port0, port1);

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
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates this Valve Controller model initialization data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::validate() const
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
void GunnsFluidSolenoidValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidValve::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt  (s)  Time step.
///
/// @return         void
///
/// @details        Updates this GUNNS Fluid Solenoid Valve Link Model state (valve position and
///                 effective conductivity).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::updateState(const double dt)
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
    GunnsFluidValve::updateState(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] latching  (--) latching state 
///
/// @details  Sets whether the valve is latching
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::setLatching(const bool latching)
{
    mLatching = latching;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flux  (--) Flux through the coil
///
/// @details  Sets the flux to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::setFlux(const double flux)
{
    mFlux = flux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage  (--) Voltage over the coil
///
/// @details  Sets the voltage to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::setVoltage(const double voltage)
{
    mVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
///
/// @details  Sets the stuck malf flag to given the value.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::setMalfStuck(const bool flag)
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
void GunnsFluidSolenoidValve::setMalfFailTo(const bool flag, const double value)
{
    mMalfFailToFlag  = flag;
    mMalfFailToValue = value;
}

