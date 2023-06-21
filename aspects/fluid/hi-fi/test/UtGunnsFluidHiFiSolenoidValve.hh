#ifndef UtGunnsFluidHiFiSolenoidValve_EXISTS
#define UtGunnsFluidHiFiSolenoidValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_HIFI_SOLENOID_VALVE    Solenoid Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID
///
/// @details  Unit Tests for the GUNNS Fluid Solenoid Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/hi-fi/GunnsFluidHiFiSolenoidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFiSolenoidValve and befriend UtGunnsFluidHiFiSolenoidValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFiSolenoidValve : public GunnsFluidHiFiSolenoidValve
{
    public:
         FriendlyGunnsFluidHiFiSolenoidValve();
        virtual ~FriendlyGunnsFluidHiFiSolenoidValve();
        friend class UtGunnsFluidHiFiSolenoidValve;
};
inline FriendlyGunnsFluidHiFiSolenoidValve::FriendlyGunnsFluidHiFiSolenoidValve() : GunnsFluidHiFiSolenoidValve() {};
inline FriendlyGunnsFluidHiFiSolenoidValve::~FriendlyGunnsFluidHiFiSolenoidValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Solenoid Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Solenoid Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHiFiSolenoidValve: public CppUnit::TestFixture
{
    public:
        UtGunnsFluidHiFiSolenoidValve();
        virtual ~UtGunnsFluidHiFiSolenoidValve();
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
        CPPUNIT_TEST_SUITE(UtGunnsFluidHiFiSolenoidValve);
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

        std::string                               tLinkName;               /**< (--) Nominal config data */
        GunnsFluidHiFiValveConfigData::CoeffTypes tCoefficientType;        /**< (--) Nominal config data */
        double                                    tCoefficientValue;       /**< (--) Nominal config data */
        double                                    tThroatDiameter;         /**< (--) Nominal config data */
        double                                    tCriticalReynolds;       /**< (--) Nominal config data */
        double                                    tExpansionScaleFactor;   /**< (--) Nominal config data */
        double                                    tFlowTuningFactor;       /**< (--) Nominal config data */
        bool                                      tLatching;               /**< (V)  Nominal config data */
        double                                    tOpenVoltage;            /**< (V)  Nominal config data */
        double                                    tOpenTime;               /**< (s)  Nominal config data */
        double                                    tCloseVoltage;           /**< (V)  Nominal config data */
        double                                    tCloseTime;              /**< (s)  Nominal config data */
        GunnsFluidHiFiSolenoidValveConfigData*    tConfigData;             /**< (--) Nominal config data */
        bool                                      tMalfBlockageFlag;       /**< (--) Nominal input data */
        double                                    tMalfBlockageValue;      /**< (--) Nominal input data */
        double                                    tPosition;               /**< (--) Nominal input data */
        bool                                      tMalfLeakThruFlag;       /**< (--) Nominal input data */
        double                                    tMalfLeakThruValue;      /**< (--) Nominal input data */
        double                                    tFlux;                   /**< (A)  Nominal input data */
        double                                    tVoltage;                /**< (V)  Nominal input data */
        bool                                      tMalfStuckFlag;          /**< (--) Nominal input data */
        bool                                      tMalfFailToFlag;         /**< (--) Nominal input data */
        double                                    tMalfFailToValue;        /**< (--) Nominal input data */
        GunnsFluidHiFiSolenoidValveInputData*     tInputData;              /**< (--) Nominal input data */
        FriendlyGunnsFluidHiFiSolenoidValve*      tArticle;                /**< (--) Article under test */
        GunnsFluidNode                            tNodes[5];               /**< (--) Network nodes */
        GunnsNodeList                             tNodeList;               /**< (--) Network node list */
        std::vector<GunnsBasicLink*>              tLinks;                  /**< (--) Network links vector */
        int                                       tPort0;                  /**< (--) Nominal init data */
        int                                       tPort1;                  /**< (--) Nominal init data */
        double                                    tTimeStep;               /**< (s)  Nominal time step */
        DefinedFluidProperties*                   tFluidProperties;        /**< (--) Nominal config data */
        PolyFluidConfigData*                      tFluidConfig;            /**< (--) Nominal config data */
        PolyFluidInputData*                       tFluidInput1;            /**< (--) Nominal input data */
        PolyFluidInputData*                       tFluidInput2;            /**< (--) Nominal input data */
        PolyFluidInputData*                       tFluidInput3;            /**< (--) Nominal input data */
        double*                                   fractions;               /**< (--) Nominal input data */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHiFiSolenoidValve(const UtGunnsFluidHiFiSolenoidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHiFiSolenoidValve& operator =(const UtGunnsFluidHiFiSolenoidValve&);
};

///@}

#endif
