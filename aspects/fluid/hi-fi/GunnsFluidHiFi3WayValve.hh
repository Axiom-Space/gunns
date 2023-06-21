#ifndef GunnsFluidHiFi3WayValve_EXISTS
#define GunnsFluidHiFi3WayValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_HIFI_THREE_WAY_VALVE    Three Way Valve Model
@ingroup   TSM_GUNNS_FLUID_HIFI

@details
PURPOSE:
- (Class for the GUNNS Three Way Valve.  This consists of two normal valves that share the same exit
   port.  Their positions are coordinated by this parent class to sum to 1.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidHiFi3WayValve.o))

PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-06))

@{
*/

#include "aspects/fluid/hi-fi/GunnsFluidHiFiValve.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Three Way Valve
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFi3WayValveConfigData : public GunnsFluidLinkConfigData
{
    public:
        bool       mPositionPathB;                                      /**< (--) trick_chkpnt_io(**) Valve position is relative to flow path B (false = A) */
        GunnsFluidHiFiOrificeConfigData::CoeffTypes mCoefficientTypeA;  /**< (--) trick_chkpnt_io(**) Path A type of coefficient to be specified. */
        GunnsFluidHiFiOrificeConfigData::CoeffTypes mCoefficientTypeB;  /**< (--) trick_chkpnt_io(**) Path B type of coefficient to be specified. */
        double     mCoefficientValueA;                                  /**< (--) trick_chkpnt_io(**) Path A coefficient value of the specified type. */
        double     mCoefficientValueB;                                  /**< (--) trick_chkpnt_io(**) Path B coefficient value of the specified type. */
        double     mThroatDiameterA;                                    /**< (m)  trick_chkpnt_io(**) Path A throat diameter. */
        double     mThroatDiameterB;                                    /**< (m)  trick_chkpnt_io(**) Path B throat diameter. */
        double     mCriticalReynoldsA;                                  /**< (--) trick_chkpnt_io(**) Path A optional Reynolds number at laminar/turbulent transition. */
        double     mCriticalReynoldsB;                                  /**< (--) trick_chkpnt_io(**) Path B optional Reynolds number at laminar/turbulent transition. */
        double     mExpansionScaleFactorA;                              /**< (--) trick_chkpnt_io(**) Path A optional scaling for expansion gas cooling. */
        double     mExpansionScaleFactorB;                              /**< (--) trick_chkpnt_io(**) Path B optional scaling for expansion gas cooling. */
        double     mFlowTuningFactorA;                                  /**< (--) trick_chkpnt_io(**) Path A optional factor for flow tuning. */
        double     mFlowTuningFactorB;                                  /**< (--) trick_chkpnt_io(**) Path B optional factor for flow tuning. */
        double     mNonMixingBand;                                      /**< (--) trick_chkpnt_io(**) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut. */
        /// @brief Default constructs this Three Way Valve configuration data.
        GunnsFluidHiFi3WayValveConfigData(const std::string&                                name                  = "",
                                          GunnsNodeList*                                    nodes                 = 0,
                                          const bool                                        positionPathB         = false,
                                          const GunnsFluidHiFiOrificeConfigData::CoeffTypes coefficientTypeA      = GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF,
                                          const GunnsFluidHiFiOrificeConfigData::CoeffTypes coefficientTypeB      = GunnsFluidHiFiOrificeConfigData::DISCHARGE_COEFF,
                                          const double                                      coefficientValueA     = 0.0,
                                          const double                                      coefficientValueB     = 0.0,
                                          const double                                      throatDiameterA       = 0.0,
                                          const double                                      throatDiameterB       = 0.0,
                                          const double                                      criticalReynoldsA     = 0.0,
                                          const double                                      criticalReynoldsB     = 0.0,
                                          const double                                      expansionScaleFactorA = 0.0,
                                          const double                                      expansionScaleFactorB = 0.0,
                                          const double                                      flowTuningFactorA     = 1.0,
                                          const double                                      flowTuningFactorB     = 1.0,
                                          const double                                      nonMixingBand         = 0.0);
        /// @brief Copy constructs this Three Way Valve configuration data.
        GunnsFluidHiFi3WayValveConfigData(const GunnsFluidHiFi3WayValveConfigData& that);
        /// @brief Default destructs this Three Way Valve configuration data.
        virtual ~GunnsFluidHiFi3WayValveConfigData();
    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFi3WayValveConfigData& operator =(const GunnsFluidHiFi3WayValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Three Way Valve Input Data
///
/// @details  This provides a data structure for the Three Way Valve input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFi3WayValveInputData : public GunnsFluidLinkInputData
{
    public:
        double mPosition;         /**< (--) (0-1) Valve position relative to config data path */
        /// @brief Default constructs this Three Way Valve input data.
        GunnsFluidHiFi3WayValveInputData(const double position = 0.0);
        /// @brief Copy constructs this Three Way Valve input data.
        GunnsFluidHiFi3WayValveInputData(const GunnsFluidHiFi3WayValveInputData& that);
        /// @brief Default destructs this Three Way Valve input data.
        virtual ~GunnsFluidHiFi3WayValveInputData();
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFi3WayValveInputData& operator =(const GunnsFluidHiFi3WayValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Three Way Valve
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
class GunnsFluidHiFi3WayValve : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidHiFi3WayValve);

    public:

        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsFluidHiFiValve mPathA;                     /**< (--) Path A valve */
        GunnsFluidHiFiValve mPathB;                     /**< (--) Path B valve */
        /// @}

        /// @brief Default constructor.
        GunnsFluidHiFi3WayValve();
        /// @brief Default destructor.
        virtual ~GunnsFluidHiFi3WayValve();
        /// @brief Initialize method.
        void initialize(const GunnsFluidHiFi3WayValveConfigData& configData,
                        const GunnsFluidHiFi3WayValveInputData&  inputData,
                        std::vector<GunnsBasicLink*>&            networkLinks,
                        const int                                port0,
                        const int                                port1,
                        const int                                port2);
        /// @brief Steps the 3-way valve.
        virtual void step(const double dt);
        /// @brief Computes flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Transports flows through the link.
        virtual void transportFlows(const double dt);
        /// @brief Updates the State of the 3-way valve.
        virtual void updateState(const double dt);
        /// @brief Gets the 3-way valve position.
        virtual double getPosition();
        /// @brief Sets the 3-way valve position.
        virtual void setPosition(const double position);
        /// @brief Gets a pointer to the path A valve object.
        GunnsFluidHiFiValve* getPathA();
        /// @brief Gets a pointer to the path B valve object.
        GunnsFluidHiFiValve* getPathB();

    protected:
        bool            mPositionPathB;             /**< (--) trick_chkpnt_io(**) Valve position is relative to flow path B (false = A) */
        double          mNonMixingBand;             /**< (--) trick_chkpnt_io(**) Fraction (0-1] of valve range centered on 0.5 in which both paths are shut. */
        double          mPosition;                  /**< (--)                     Three way valve position. Initialized, then read from sim-bus */
        /// @brief Validates the initialization of this Gunns Three Way Valve.
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
        GunnsFluidHiFi3WayValve(const GunnsFluidHiFi3WayValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFi3WayValve& operator =(const GunnsFluidHiFi3WayValve& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  (0-1) 3-way valve fluid position.
///
/// @details  Sets the 3-way valve position.  The given position should be 0-1, 0 representing fully
///           closed and 1 representing fully open of the path given by the config parameter
///           mPositionPathB (false = path A).  The path's 2-way valve positions always sum to 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFi3WayValve::setPosition(const double position)
{
    mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  position  (--)  (0-1) 3-way valve fluid position.
///
/// @details  Gets the 3-way valve position.  The position should be 0-1, 0 representing fully
///           closed and 1 representing fully open of the path given by the config parameter
///           mPositionPathB (false = path A).  The path's 2-way valve positions always sum to 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHiFi3WayValve::getPosition()
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidHiFiValve*  (--)  Pointer to the path A valve object.
///
/// @details  Returns a pointer to GunnsFluidHiFiValve mPathA.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidHiFiValve* GunnsFluidHiFi3WayValve::getPathA()
{
    return &mPathA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidHiFiValve*  (--)  Pointer to the path B valve object.
///
/// @details  Returns a pointer to GunnsFluidHiFiValve mPathB.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidHiFiValve* GunnsFluidHiFi3WayValve::getPathB()
{
    return &mPathB;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @details  The path A & B sub-valve links perform their own transprt.  The base class requires
///           this to be overriden, so we leave this empty.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFi3WayValve::transportFlows(const double dt __attribute__((unused)))
{
    // nothing to do
}

#endif
