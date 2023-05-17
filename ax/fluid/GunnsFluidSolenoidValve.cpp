/**
@file
@brief    TS21 Fluid Controller Fluid (Valve, Fan, Pump) Controllers implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((simulation/hs/TsHsMsg.o)
  (software/exceptions/TsInitializationException.o))
*/

#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidSolenoidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 (--)    Name of object.
/// @param[in]  nodes                (--)    Pointer to nodes.
/// @param[in]  maxConductivity      (m2)    Maximum conductivity.
/// @param[in]  expansionScaleFactor (--)    Scale factor for isentropic gas cooling.
/// @param[in]  thermalLength        (m)     Tube length for thermal convection
/// @param[in]  thermalDiameter      (m)     Tube inner diameter for thermal convection
/// @param[in]  surfaceRoughness     (m)     Tube wall surface roughness for thermal convection
/// @param[in]  minCmdPosition       (--)    Minimum valid valve position.
/// @param[in]  maxCmdPosition       (--)    Maximum valid valve position.
/// @param[in]  minFluidPosition     (--)    Minimum valid valve flow area fraction.
/// @param[in]  maxFluidPosition     (--)    Maximum valid valve flow area fraction.
///
/// @details    Default constructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveConfigData::GunnsFluidSolenoidValveConfigData(const std::string& name,
                                                                     GunnsNodeList*     nodes,
                                                                     const double       maxConductivity,
                                                                     const double       expansionScaleFactor,
                                                                     const double       thermalLength,
                                                                     const double       thermalDiameter,
                                                                     const double       surfaceRoughnessconst,
                                                                     const double       minCmdPosition,
                                                                     const double       maxCmdPosition,
                                                                     const double       minFluidPosition,
                                                                     const double       maxFluidPosition)
    :
    GunnsFluidValveConfigData(name,
                              nodes,
                              maxConductivity,
                              expansionScaleFactor,
                              thermalLength,
                              thermalDiameter,
                              surfaceRoughness),
    mMinCmdPosition(minCmdPosition),
    mMaxCmdPosition(maxCmdPosition),
    mMinFluidPosition(minFluidPosition),
    mMaxFluidPosition(maxFluidPosition)
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
    mMinCmdPosition(that.mMinCmdPosition),
    mMaxCmdPosition(that.mMaxCmdPosition),
    mMinFluidPosition(that.mMinFluidPosition),
    mMaxFluidPosition(that.mMaxFluidPosition)
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
/// @param[in]  wallTemperature        (K)     Tube wall temperature for thermal convection
/// @param[in]  malfStuckFlag          (--)    Stuck at current position malfunction flag.
/// @param[in]  malfFailToFlag         (--)    Fail to position malfunction flag.
/// @param[in]  malfFailToValue        (--)    Fail to position malfunction value
/// @param[in]  cmdPosition            (--)    Valve position.
/// @param[in]  manualPositionFlag     (--)    Manual override valve position flag.
/// @param[in]  manualPositionValue    (--)    Manual override valve position value.
///
/// @details    Default constructs this Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSolenoidValveInputData::GunnsFluidSolenoidValveInputData(const bool   malfBlockageFlag,
                                                                       const double malfBlockageValue,
                                                                       const double position,
                                                                       const bool   malfLeakThruFlag,
                                                                       const double malfLeakThruValue,
                                                                       const double wallTemperature,
                                                                       const double cmdPosition,
                                                                       const bool   manualPositionFlag,
                                                                       const double manualPositionValue)
    :
    GunnsFluidValveInputData(malfBlockageFlag,
                             malfBlockageValue,
                             position,
                             malfLeakThruFlag,
                             malfLeakThruValue,
                             wallTemperature),
    mCmdPosition(cmdPosition),
    mManualPositionFlag(manualPositionFlag),
    mManualPositionValue(manualPositionValue)
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
    mCmdPosition(that.mCmdPosition),
    mManualPositionFlag(that.mManualPositionFlag),
    mManualPositionValue(that.mManualPositionValue)
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
    mMalfValveStuckFlag(false),
    mMalfValveFailToFlag(false),
    mMalfValveFailToValue(0.0),
    mMalfManualFlag(false),
    mMinCmdPosition(0.0),
    mMaxCmdPosition(0.0),
    mFluidBias(0.0),
    mFluidScale(0.0),
    mCmdPosition(0.0),
    mManualPositionFlag(false),
    mManualPositionValue(0.0),
    mStuckFlag(false),
    mLowerLimitFlag(false),
    mUpperLimitFlag(false),
    mFluidPosition(0.0),
    mInitFlag(false)
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
                                           std::vector<GunnsBasicLink*>&              links,
                                           const int                                  port0,
                                           const int                                  port1)
{
    /// - Reset the initialization complete flag.
    mInitFlag            = false;
    
    /// - First initialize & validate parent.
    GunnsFluidValve::initialize(configData, inputData, links, port0, port1);

    /// - Validate the configuration and input data.
    validate(config, input);

    /// - Initialize from the configuration data.
    mMinCmdPosition         = config.mMinCmdPosition;
    mMaxCmdPosition         = config.mMaxCmdPosition;
    mFluidScale             = (config.mMaxFluidPosition - config.mMinFluidPosition) / (mMaxCmdPosition - mMinCmdPosition);
    mFluidBias              = config.mMaxFluidPosition - mFluidScale * config.mMaxCmdPosition;

    /// - Initialize from the input data.
    mCmdPosition            = input.mCmdPosition;
    mManualPositionFlag     = input.mManualPositionFlag;
    mManualPositionValue    = input.mManualPositionValue;

    /// - Initialize malfunctions off..
    mMalfValveStuckFlag     = false;
    mMalfValveFailToFlag    = false;
    mMalfValveFailToValue   = 0.0;
    mMalfManualFlag         = false;

    /// - Initialize the outputs (position) consistent with the inputs.
    GunnsFluidSolenoidValve::update(0.0);

    /// - Set the initialization complete flag.
    mInitFlag            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  Configuration data.
/// @param[in]  input   (--)  Input data.
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates this Valve Controller model initialization data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::validate(const GunnsFluidSolenoidValveConfigData& config,
                                       const GunnsFluidSolenoidValveInputData&  input) const
{
    /// - Throw a TsInitializationException exception on valve maximum flow area fraction <= valve minimum flow area fraction.
    TS_GENERIC_IF_ERREX((config.mMaxFluidPosition <= config.mMinFluidPosition),
                        TsInitializationException, "Invalid Configuration Data",  "Valve maximum flow area fraction <= valve minimum flow area fraction.");

    /// - Throw a TsInitializationException exception on valve position out of range.
    TS_GENERIC_IF_ERREX((!MsMath::isInRange(config.mMinCmdPosition, input.mCmdPosition, config.mMaxCmdPosition)),
                        TsInitializationException, "Invalid Input Data",          "Valve position out of range.");

    /// - Throw a TsInitializationException exception on manual position out of range.
    TS_GENERIC_IF_ERREX(input.mManualPositionFlag && (!MsMath::isInRange(config.mMinCmdPosition, input.mManualPositionValue, config.mMaxCmdPosition)),
                        TsInitializationException, "Invalid Input Data",          "Manual position out of range.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCheckValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidValve::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step (not used).
///
/// @note       This method is intended to by called by a manager which has the responsibility
///             for ensuring that this instance has been initialized, hence the lack of an internal
///             initialization check.
///
/// @details    Updates this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::update(const double dt __attribute__((unused)))
{
    updatePosition(mCmdPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  Fractional valve position.
///
/// @details    Updates the fractional valve position of this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSolenoidValve::updatePosition(const double position)
{
    /// - Skip the position update on a stuck valve malfunction.
    if (!mMalfValveStuckFlag) {
        if (mMalfValveFailToFlag) {
            /// - Set the position to the fail-to value on a valve fail-to position malfunction.
            mCmdPosition = mMalfValveFailToValue;
        } else if (mManualPositionFlag && !mMalfManualFlag) {
            /// - Compute the desired position based on the manual command, if any, and subject to malfunction.
            mCmdPosition = mManualPositionValue;
        } else {
            /// - Otherwise, use the input desired position.
            mCmdPosition = position;
        }
    }

    /// - Update status flags (stuck, lower limit, upper limit).
    mStuckFlag                 = mMalfValveStuckFlag || mMalfValveFailToFlag;
    mLowerLimitFlag            = mCmdPosition <= mMinCmdPosition;
    mUpperLimitFlag            = mCmdPosition >= mMaxCmdPosition;

    /// - In all cases limit the position to the valid range.
    mCmdPosition               = MsMath::limitRange(mMinCmdPosition, mCmdPosition, mMaxCmdPosition);

    /// - Compute the valve flow area fraction from the position and limit range to 0 to 1.
    const double fluidPosition = mFluidBias + mFluidScale * mCmdPosition;
    mFluidPosition             = MsMath::limitRange(0.0, fluidPosition, 1.0);
}

