#ifndef UtGunnsFluidSolenoidValve_EXISTS
#define UtGunnsFluidSolenoidValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOLENOID_VALVE    Solenoid Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID
///
/// @details  Unit Tests for the GUNNS Fluid Solenoid Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/signal/GunnsFluidSolenoidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSolenoidValve and befriend UtGunnsFluidSolenoidValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSolenoidValve : public GunnsFluidSolenoidValve
{
    public:
         FriendlyGunnsFluidSolenoidValve();
        virtual ~FriendlyGunnsFluidSolenoidValve();
        friend class UtGunnsFluidSolenoidValve;
};
inline FriendlyGunnsFluidSolenoidValve::FriendlyGunnsFluidSolenoidValve() : GunnsFluidSolenoidValve() {};
inline FriendlyGunnsFluidSolenoidValve::~FriendlyGunnsFluidSolenoidValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Solenoid Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Solenoid Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSolenoidValve: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Solenoid Valve unit test.
        UtGunnsFluidSolenoidValve();
        /// @brief    Default destructs this Solenoid Valve unit test.
        virtual ~UtGunnsFluidSolenoidValve();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateStateNominal();
        /// @brief    Tests update state method (rate limiting).
        void testUpdateStateRateLimited();
        /// @brief    Tests update state method (no range).
        void testUpdateStateNoRange();
        /// @brief    Tests update state method (malfunction).
        void testUpdateStateMalfunction();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSolenoidValve);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateRateLimited);
        CPPUNIT_TEST(testUpdateStateNoRange);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 4, N_FLUIDS = 2};
        FluidProperties::FluidType        mTypes[N_FLUIDS];        /**< (--)            Constituent fluid types array. */
        double                            mFractions[N_FLUIDS];    /**< (--)            Constituent fluid mass fractions array. */
        DefinedFluidProperties*           mFluidProperties;        /**< (--)            Predefined fluid properties. */
        PolyFluidConfigData*              mFluidConfig;            /**< (--)            Fluid config data. */
        PolyFluidInputData*               mFluidInput0;            /**< (--)            Fluid input data for node 0. */
        PolyFluidInputData*               mFluidInput1;            /**< (--)            Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>      mLinks;                  /**< (--)            Link vector. */
        std::string                       mName;                   /**< (--)            Nominal name. */
        GunnsFluidNode                    mNodes[N_NODES];         /**< (--)            Nominal connected nodes. */
        GunnsNodeList                     mNodeList;               /**< (--)            Network node structure. */
        int                               mPort0;                  /**< (--)            Nominal inlet port node index. */
        int                               mPort1;                  /**< (--)            Nominal outlet port node index. */
        double                            mMaxConductivity;        /**< (m2)            Nominal maximum conductivity. */
        double                            mExpansionScaleFactor;   /**< (--)            Nominal scale factor for isentropic gas cooling. */
        double                            mThermalLength;          /**< (m)             Tube length for thermal convection. */
        double                            mThermalDiameter;        /**< (m)             Tube inner diameter for thermal convection. */
        double                            mSurfaceRoughness;       /**< (m)             Tube wall surface roughness for thermal convection. */
        double                            mThermalSurfaceArea;     /**< (m2)            Tube inner surface area for thermal convection       */
        double                            mThermalROverD;          /**< (--)            Tube surface roughness over diameter for convection  */
        double                            mOpenVoltage;            /**< (V)             Voltage at which valve is open.  */
        double                            mOpenTime;               /**< (s)             Time it takes for the valve to open  */
        double                            mCloseVoltage;           /**< (V)             Voltage at which valve is closed.  */
        double                            mCloseTime;              /**< (s)             Time it takes for the valve to close  */
        GunnsFluidSolenoidValveConfigData*   mConfigData;             /**< (--)            Pointer to the nominal configuration data. */
        bool                              mMalfBlockageFlag;       /**< (--)            Blockage malfunction flag. */
        double                            mMalfBlockageValue;      /**< (--)            Blockage malfunction value. */
        double                            mPosition;               /**< (--)            Fractional position of this valve. */
        bool                              mMalfLeakThruFlag;       /**< (--)            Leak through rate malfunction flag. */
        double                            mMalfLeakThruValue;      /**< (kg/s)          Leak through rate malfunction value. */
        double                            mWallTemperature;        /**< (K)             Tube wall temperature for thermal convection           */
        double                            mVoltage;                /**< (V)             Voltage across the solenoid coil  */
        bool                              mMalfStuckFlag;          /**< (--)            Stuck at current position malfunction flag. */
        bool                              mMalfFailToFlag;         /**< (--)            Fail to position malfunction flag. */
        double                            mMalfFailToValue;        /**< (--)            Fail to position malfunction value. */
        GunnsFluidSolenoidValveInputData*    mInputData;              /**< (--)            Pointer to the nominal input data. */
        FriendlyGunnsFluidSolenoidValve*     mArticle;                /**< (--)            Pointer to the friendly Solenoid Valve under test. */
        double                            mPreviousLeakRate;       /**< (kg/s)          Previous leak thru rate value. */
        double                            mLeakConductivity;       /**< (m2)            Conductivity equivalent to the leak. */
        double                            mWallHeatFlux;           /**< (W)             Convection heat flux from the fluid to the tube wall */
        double                            mFlowRate;               /**< (kg/s)          Nominal mass flow rate. */
        double                            mTimeStep;               /**< (s)             Nominal time step. */
        double                            mTolerance;              /**< (--)            Nominal tolerance for comparison of expected and returned values. */
        static int                        TEST_ID;                 /**< (--)            Test identification number. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSolenoidValve(const UtGunnsFluidSolenoidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSolenoidValve& operator =(const UtGunnsFluidSolenoidValve&);
};

///@}

#endif
