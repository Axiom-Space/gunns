/************************** TRICK HEADER **********************************************************
LIBRARY DEPENDENCY:
    ((core/GunnsFluidLink.o))

PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-06))
**************************************************************************************************/

#include "GunnsFluidHiFi3WayValve.hh"
#include "core/GunnsFluidUtils.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--) Link name.
/// @param[in] nodes                 (--) Network nodes array.
/// @param[in] positionPathB         (--) Valve position is relative to flow path B (false = A)
/// @param[in] coefficientTypeA      (--) Path A type of coefficient to be specified.
/// @param[in] coefficientTypeB      (--) Path B type of coefficient to be specified.
/// @param[in] coefficientValueA     (--) Path A coefficient value of the specified type.
/// @param[in] coefficientValueB     (--) Path B coefficient value of the specified type.
/// @param[in] throatDiameterA       (m)  Path A throat diameter.
/// @param[in] throatDiameterB       (m)  Path B throat diameter.
/// @param[in] criticalReynoldsA     (--) Path A optional Reynolds number at laminar/turbulent transition.
/// @param[in] criticalReynoldsB     (--) Path B optional Reynolds number at laminar/turbulent transition.
/// @param[in] expansionScaleFactorA (--) Path A optional scaling for expansion gas cooling.
/// @param[in] expansionScaleFactorB (--) Path B optional scaling for expansion gas cooling.
/// @param[in] flowTuningFactorA     (--) Path A optional factor for flow tuning.
/// @param[in] flowTuningFactorB     (--) Path B optional factor for flow tuning.
/// @param[in] nonMixingBand         (--) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut.
///
/// @details  Default GUNNS Three Way Valve Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveConfigData::GunnsFluidHiFi3WayValveConfigData(const std::string&                             name,
                                                                     GunnsNodeList*                                 nodes,
                                                                     bool                                           positionPathB,
                                                                     GunnsFluidHiFiOrificeConfigData::CoeffTypes    coefficientTypeA,
                                                                     GunnsFluidHiFiOrificeConfigData::CoeffTypes    coefficientTypeB,
                                                                     double                                         coefficientValueA,
                                                                     double                                         coefficientValueB,
                                                                     double                                         throatDiameterA,
                                                                     double                                         throatDiameterB,
                                                                     double                                         criticalReynoldsA,
                                                                     double                                         criticalReynoldsB,
                                                                     double                                         expansionScaleFactorA,
                                                                     double                                         expansionScaleFactorB,
                                                                     double                                         flowTuningFactorA,
                                                                     double                                         flowTuningFactorB,
                                                                     double                                         nonMixingBand)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mPositionPathB(positionPathB),
    mCoefficientTypeA(coefficientTypeA),
    mCoefficientTypeB(coefficientTypeB),
    mCoefficientValueA(coefficientValueA),
    mCoefficientValueB(coefficientValueB),
    mThroatDiameterA(throatDiameterA),
    mThroatDiameterB(throatDiameterB),
    mCriticalReynoldsA(criticalReynoldsA),
    mCriticalReynoldsB(criticalReynoldsB),
    mExpansionScaleFactorA(expansionScaleFactorA),
    mExpansionScaleFactorB(expansionScaleFactorB),
    mFlowTuningFactorA(flowTuningFactorA),
    mFlowTuningFactorB(flowTuningFactorB),
    mNonMixingBand(nonMixingBand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Gunns Three Way Valve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveConfigData::GunnsFluidHiFi3WayValveConfigData(const GunnsFluidHiFi3WayValveConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mPositionPathB(that.mPositionPathB),
    mCoefficientTypeA(that.mCoefficientTypeA),
    mCoefficientTypeB(that.mCoefficientTypeB),
    mCoefficientValueA(that.mCoefficientValueA),
    mCoefficientValueB(that.mCoefficientValueB),
    mThroatDiameterA(that.mThroatDiameterA),
    mThroatDiameterB(that.mThroatDiameterB),
    mCriticalReynoldsA(that.mCriticalReynoldsA),
    mCriticalReynoldsB(that.mCriticalReynoldsB),
    mExpansionScaleFactorA(that.mExpansionScaleFactorA),
    mExpansionScaleFactorB(that.mExpansionScaleFactorB),
    mFlowTuningFactorA(that.mFlowTuningFactorA),
    mFlowTuningFactorB(that.mFlowTuningFactorB),
    mNonMixingBand(that.mNonMixingBand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Valve Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveConfigData::~GunnsFluidHiFi3WayValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position         (--) (0-1) Valve position relative to config data path.
/// @param[in] wallTemperatureA (K)  Path A wall temperature for thermal convection.
/// @param[in] wallTemperatureB (K)  Path B wall temperature for thermal convection.
///
/// @details  Default GUNNS Three Way Valve Input Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveInputData::GunnsFluidHiFi3WayValveInputData(const double position)
    :
    GunnsFluidLinkInputData(false, 0.0),
    mPosition              (position)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Three Way Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveInputData::GunnsFluidHiFi3WayValveInputData(const GunnsFluidHiFi3WayValveInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mPosition              (that.mPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Three Way Valve Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValveInputData::~GunnsFluidHiFi3WayValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Three Way Valve Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValve::GunnsFluidHiFi3WayValve()
    :
    GunnsFluidLink(NPORTS),
    mPathA(),
    mPathB(),
    mPositionPathB(false),
    mNonMixingBand(0.0),
    mPosition(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Three Way Valve
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHiFi3WayValve::~GunnsFluidHiFi3WayValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Three Way Valve Config Data
/// @param[in]     inputData    (--) GUNNS Three Way Valve Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
/// @param[in]     port2        (--) Network port 2
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Three Way Valve with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFi3WayValve::initialize(const GunnsFluidHiFi3WayValveConfigData& configData,
                                         const GunnsFluidHiFi3WayValveInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            networkLinks,
                                         const int                                port0,
                                         const int                                port1,
                                         const int                                port2)
{
    /// - Initialize the parent class.
    int ports[3] = {port0, port1, port2};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag      = false;

    /// - Initialize class attributes.
    mPositionPathB = configData.mPositionPathB;
    mNonMixingBand = configData.mNonMixingBand;
    mPosition      = inputData.mPosition;

    /// - Build fluid valve config & input data and initialize the valves.
    ///   The input data malfunctions are all forced off, and their
    ///   positions are ignored here since they will get set during update.
    mPathA.initialize(GunnsFluidHiFiValveConfigData(configData.mName + ".mPathA",
                                                    configData.mNodeList,
                                                    configData.mCoefficientTypeA,
                                                    configData.mCoefficientValueA,
                                                    configData.mThroatDiameterA,
                                                    configData.mCriticalReynoldsA,
                                                    configData.mExpansionScaleFactorA,
                                                    configData.mFlowTuningFactorA),
                      GunnsFluidHiFiValveInputData(false, 0.0, 0.0, false, 0.0),
                      networkLinks, port0, port2);

    mPathB.initialize(GunnsFluidHiFiValveConfigData(configData.mName + ".mPathB",
                                                    configData.mNodeList,
                                                    configData.mCoefficientTypeB,
                                                    configData.mCoefficientValueB,
                                                    configData.mThroatDiameterB,
                                                    configData.mCriticalReynoldsB,
                                                    configData.mExpansionScaleFactorB,
                                                    configData.mFlowTuningFactorB),
                      GunnsFluidHiFiValveInputData(false, 0.0, 0.0, false, 0.0),
                      networkLinks, port1, port2);

    /// - Validate and set init flag on successful validation.
    validate();
    mInitFlag = mPathA.isInitialized() and mPathB.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Three Way Valve initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFi3WayValve::validate() const
{
    /// - Throw an exception on non-mixing band not in (0-1].
    if (!MsMath::isInRange(0.0, mNonMixingBand, 1.0-DBL_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Non-mixing band not (0-1].");
    }

    /// - Throw an exception on valve position not (0-1).
    if (!MsMath::isInRange(0.0, mPosition, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Valve position not (0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFi3WayValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @throws   TsOutOfBoundsException
///
/// @details  Calls step on the Three Way Valve to update the state and compute a new conductivity
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFi3WayValve::step(const double dt)
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
inline void GunnsFluidHiFi3WayValve::updateState(const double dt)
{
    /// - Calculate path valve positions from main valve position and set them.
    if (mNonMixingBand > 0.0) {
        const double valveWidth = std::max(DBL_EPSILON, 0.5 - 0.5 * mNonMixingBand);
        if (mPositionPathB) {
            mPathA.setPosition(std::max(0.0, (valveWidth - mPosition) / valveWidth));
            mPathB.setPosition(std::max(0.0, (mPosition - (1.0 - valveWidth)) / valveWidth));
        } else {
            mPathA.setPosition(std::max(0.0, (mPosition - (1.0 - valveWidth)) / valveWidth));
            mPathB.setPosition(std::max(0.0, (valveWidth - mPosition) / valveWidth));
        }
    } else {
        if (mPositionPathB) {
            mPathA.setPosition(1.0 - mPosition);
            mPathB.setPosition(mPosition);
        } else {
            mPathA.setPosition(mPosition);
            mPathB.setPosition(1.0 - mPosition);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  The path A & B sub-valve links perform their own functions.  This parent link has no
///           flows itself, so we set all port directions to indicate no flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHiFi3WayValve::computeFlows(const double dt __attribute__((unused)))
{
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    mPortDirections[2] = NONE;
}
