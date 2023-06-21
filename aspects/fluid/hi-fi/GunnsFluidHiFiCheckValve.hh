#ifndef GunnsFluidHiFiCheckValve_EXISTS
#define GunnsFluidHiFiCheckValve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_HIFI_CHECK_VALVE    Check Valve Model
@ingroup   TSM_GUNNS_FLUID_HIFI

@details
PURPOSE:
- (Classes for the GUNNS Fluid Check Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidHiFiCheckValve.o))

PROGRAMMERS:
- ((Kyle Fondon) (Axiom Space) (Initial) (2023-06))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

#include "aspects/fluid/hi-fi/GunnsFluidHiFiValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Check Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiCheckValveConfigData : public GunnsFluidHiFiValveConfigData
{
    public:
        double        mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit for this Check Valve. */
        double        mClosePressure;      /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully closed. */
        double        mOpenPressure;       /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully opened. */
        /// @brief    Default constructs this Check Valve configuration data.
        GunnsFluidHiFiCheckValveConfigData(const std::string&   name                 = "",
                                           GunnsNodeList*       nodes                = 0,
                                           const CoeffTypes     coefficientType      = DISCHARGE_COEFF,
                                           const double         coefficientValue     = 0.0,
                                           const double         throatDiameter       = 0.0,
                                           const double         criticalReynolds     = 0.0,
                                           const double         expansionScaleFactor = 0.0,
                                           const double         flowTuningFactor     = 1.0,
                                           const double         rateLimit            = 0.0,
                                           const double         closePressure        = 0.0,
                                           const double         openPressure         = 0.0);
        /// @brief    Copy constructs this Check Valve configuration data.
        GunnsFluidHiFiCheckValveConfigData(const GunnsFluidHiFiCheckValveConfigData& that);
        /// @brief    Default destructs this Check Valve configuration data.
        virtual ~GunnsFluidHiFiCheckValveConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiCheckValveConfigData& operator =(const GunnsFluidHiFiCheckValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Check Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiCheckValveInputData : public GunnsFluidHiFiValveInputData
{
    public:
        bool          mMalfStuckFlag;   /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;  /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue; /**< (--)   Fail to position malfunction value. */
        /// @brief    Default constructs this Check Valve input data.
        GunnsFluidHiFiCheckValveInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const double position          = 0.0,
                                          const bool   malfLeakThruFlag  = false,
                                          const double malfLeakThruValue = 0.0,
                                          const bool   malfStuckFlag     = false,
                                          const bool   malfFailToFlag    = false,
                                          const double malfFailToValue   = 0.0);
        /// @brief    Copy constructs this Check Valve input data.
        GunnsFluidHiFiCheckValveInputData(const GunnsFluidHiFiCheckValveInputData& that);
        /// @brief    Default destructs this Check Valve input data.
        virtual ~GunnsFluidHiFiCheckValveInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiCheckValveInputData& operator =(const GunnsFluidHiFiCheckValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Check Valve Link Model
///
/// @details  The GUNNS Fluid Check Valve link model restricts flow to a single direction.
///           This valve transitions between fully open and fully closed based on the delta pressure
///           across the valve. The open pressure must be no less than the close pressure. There is
///           no external (automatic or manual) control.
///           \verbatim
///                                valve position
///                                    1 -|         -----------------
///                                       |        /
///                                       |       /
///                                       |      /
///                                       |     /
///                                       |    /
///                                       |   /
///                                       |  /
///                                       | /
///                                       |/
///                                       /
///                                      /|
///                                     / |
///                                    /  |
///                                   /   |
///            ======================|----------|-------------------- delta pressure
///                    mClosePressure            mOpenPressure
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiCheckValve : public GunnsFluidHiFiValve
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHiFiCheckValve);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool          mMalfStuckFlag;      /**< (--)   Stuck at current position malfunction flag. */
        bool          mMalfFailToFlag;     /**< (--)   Fail to position malfunction flag. */
        double        mMalfFailToValue;    /**< (--)   Fail to position malfunction value. */
        /// @}

        /// @brief    Default constructs this Check Valve.
        GunnsFluidHiFiCheckValve();
        /// @brief    Default destructs this Check Valve.
        virtual ~GunnsFluidHiFiCheckValve();
        /// @brief    Initializes this Check Valve with configuration and input data.
        void initialize(const GunnsFluidHiFiCheckValveConfigData&   configData,
                        const GunnsFluidHiFiCheckValveInputData&    inputData,
                        std::vector<GunnsBasicLink*>&               networkLinks,
                        const int                                   port0,
                        const int                                   port1);
        /// @brief Sets and resets the stuck malfunction
        void setMalfStuck(const bool flag = false);
        /// @brief Sets and resets the fail to position malfunction
        void setMalfFailTo(const bool flag = false, const double value = 0.0);

    protected:
        double        mRateLimit;          /**< (1/s)  trick_chkpnt_io(**) Position rate limit for this Check Valve. */
        double        mClosePressure;      /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully closed. */
        double        mOpenPressure;       /**< (kPa)  trick_chkpnt_io(**) Delta pressure at which this Check Valve is fully opened. */
        /// @brief    Validates the initialization of this Check Valve.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Check Valve.
        virtual void updateState(const double dt);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiCheckValve(const GunnsFluidHiFiCheckValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHiFiCheckValve& operator =(const GunnsFluidHiFiCheckValve&);
};

/// @}

#endif
