#ifndef UtGunnsFluidHiFiCheckValve_EXISTS
#define UtGunnsFluidHiFiCheckValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_HIFI_CHECK_VALVE    Check Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID
///
/// @details  Unit Tests for the GUNNS Fluid Check Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/hi-fi/GunnsFluidHiFiCheckValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFiCheckValve and befriend UtGunnsFluidHiFiCheckValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFiCheckValve : public GunnsFluidHiFiCheckValve
{
    public:
         FriendlyGunnsFluidHiFiCheckValve();
        virtual ~FriendlyGunnsFluidHiFiCheckValve();
        friend class UtGunnsFluidHiFiCheckValve;
};
inline FriendlyGunnsFluidHiFiCheckValve::FriendlyGunnsFluidHiFiCheckValve() : GunnsFluidHiFiCheckValve() {};
inline FriendlyGunnsFluidHiFiCheckValve::~FriendlyGunnsFluidHiFiCheckValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Check Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHiFiCheckValve: public CppUnit::TestFixture
{
    public:
        UtGunnsFluidHiFiCheckValve();
        virtual ~UtGunnsFluidHiFiCheckValve();
        void setUp();
        void tearDown();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testAccessors();
        void testModifiers();
        void testUpdateStateNominal();
        void testUpdateStateRateLimited();
        void testUpdateStateNoRange();
        void testUpdateStateMalfunction();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidHiFiCheckValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateRateLimited);
        CPPUNIT_TEST(testUpdateStateNoRange);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST_SUITE_END();

        std::string                               tLinkName;               /**< (--)  Nominal config data */
        GunnsFluidHiFiValveConfigData::CoeffTypes tCoefficientType;        /**< (--)  Nominal config data */
        double                                    tCoefficientValue;       /**< (--)  Nominal config data */
        double                                    tThroatDiameter;         /**< (--)  Nominal config data */
        double                                    tCriticalReynolds;       /**< (--)  Nominal config data */
        double                                    tExpansionScaleFactor;   /**< (--)  Nominal config data */
        double                                    tFlowTuningFactor;       /**< (--)  Nominal config data */
        double                                    tRateLimit;              /**< (1/s) Nominal config data */
        double                                    tClosePressure;          /**< (kPa)  Nominal config data */
        double                                    tOpenPressure;           /**< (kPa)  Nominal config data */
        GunnsFluidHiFiCheckValveConfigData*       tConfigData;             /**< (--)  Nominal config data */
        bool                                      tMalfBlockageFlag;       /**< (--)  Nominal input data */
        double                                    tMalfBlockageValue;      /**< (--)  Nominal input data */
        double                                    tPosition;               /**< (--)  Nominal input data */
        bool                                      tMalfLeakThruFlag;       /**< (--)  Nominal input data */
        double                                    tMalfLeakThruValue;      /**< (--)  Nominal input data */
        bool                                      tMalfStuckFlag;          /**< (--)  Nominal input data */
        bool                                      tMalfFailToFlag;         /**< (--)  Nominal input data */
        double                                    tMalfFailToValue;        /**< (--)  Nominal input data */
        GunnsFluidHiFiCheckValveInputData*        tInputData;              /**< (--)  Nominal input data */
        FriendlyGunnsFluidHiFiCheckValve*         tArticle;                /**< (--)  Article under test */
        GunnsFluidNode                            tNodes[5];               /**< (--)  Network nodes */
        GunnsNodeList                             tNodeList;               /**< (--)  Network node list */
        std::vector<GunnsBasicLink*>              tLinks;                  /**< (--)  Network links vector */
        int                                       tPort0;                  /**< (--)  Nominal init data */
        int                                       tPort1;                  /**< (--)  Nominal init data */
        double                                    tTimeStep;               /**< (s)   Nominal time step */
        DefinedFluidProperties*                   tFluidProperties;        /**< (--)  Nominal config data */
        PolyFluidConfigData*                      tFluidConfig;            /**< (--)  Nominal config data */
        PolyFluidInputData*                       tFluidInput1;            /**< (--)  Nominal input data */
        PolyFluidInputData*                       tFluidInput2;            /**< (--)  Nominal input data */
        PolyFluidInputData*                       tFluidInput3;            /**< (--)  Nominal input data */
        double*                                   fractions;               /**< (--)  Nominal input data */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHiFiCheckValve(const UtGunnsFluidHiFiCheckValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHiFiCheckValve& operator =(const UtGunnsFluidHiFiCheckValve&);
};

///@}

#endif
