#ifndef UtGunnsFluidHiFi3WayValve_EXISTS
#define UtGunnsFluidHiFi3WayValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_HIFI_THREE_WAY_VALVE Three Way Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID
///
/// @details  Unit Tests for the Gunns Three Way Valve
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/hi-fi/GunnsFluidHiFi3WayValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFi3WayValve and befriend UtGunnsFluidHiFi3WayValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFi3WayValve : public GunnsFluidHiFi3WayValve
{
    public:
        FriendlyGunnsFluidHiFi3WayValve();
        virtual ~FriendlyGunnsFluidHiFi3WayValve();
        friend class UtGunnsFluidHiFi3WayValve;
};
inline FriendlyGunnsFluidHiFi3WayValve::FriendlyGunnsFluidHiFi3WayValve() : GunnsFluidHiFi3WayValve() {};
inline FriendlyGunnsFluidHiFi3WayValve::~FriendlyGunnsFluidHiFi3WayValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHiFiValve and befriend UtGunnsFluidHiFi3WayValve.
///
/// @details  Class derived from an object embedded in the unit under test. It befriends the unit
///           test case driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHiFiValveFor3Way : public GunnsFluidHiFiValve
{
    public:
        FriendlyGunnsFluidHiFiValveFor3Way();
        virtual ~FriendlyGunnsFluidHiFiValveFor3Way();
        friend class UtGunnsFluidHiFi3WayValve;
};
inline FriendlyGunnsFluidHiFiValveFor3Way::FriendlyGunnsFluidHiFiValveFor3Way() : GunnsFluidHiFiValve() {};
inline FriendlyGunnsFluidHiFiValveFor3Way::~FriendlyGunnsFluidHiFiValveFor3Way() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Three Way Valve  unit tests.
////
/// @details  This class provides the unit tests for the Three Way Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHiFi3WayValve: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidHiFi3WayValve(const UtGunnsFluidHiFi3WayValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidHiFi3WayValve& operator =(const UtGunnsFluidHiFi3WayValve& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidHiFi3WayValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepNonMixing);
        CPPUNIT_TEST(testDynamicPortMapping);
        CPPUNIT_TEST(testComputeFlow);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluidHiFi3WayValve*                tArticle;               /**< (--) Test article          */
        std::string                                     tLinkName;              /**< (--) Nominal init data     */
        GunnsFluidHiFi3WayValveConfigData*              tConfigData;            /**< (--) Nominal config data   */
        bool                                            tPositionPathB;         /**< (--) Nominal config data   */
        GunnsFluidHiFiOrificeConfigData::CoeffTypes     tCoefficientTypeA;      /**< (--) Nominal config data   */
        GunnsFluidHiFiOrificeConfigData::CoeffTypes     tCoefficientTypeB;      /**< (--) Nominal config data   */
        double                                          tCoefficientValueA;     /**< (--) Nominal config data   */
        double                                          tCoefficientValueB;     /**< (--) Nominal config data   */
        double                                          tThroatDiameterA;       /**< (m)  Nominal config data   */
        double                                          tThroatDiameterB;       /**< (m)  Nominal config data   */
        double                                          tCriticalReynoldsA;     /**< (--) Nominal config data   */
        double                                          tCriticalReynoldsB;     /**< (--) Nominal config data   */
        double                                          tExpansionScaleFactorA; /**< (--) Nominal config data   */
        double                                          tExpansionScaleFactorB; /**< (--) Nominal config data   */
        double                                          tFlowTuningFactorA;     /**< (--) Nominal config data   */
        double                                          tFlowTuningFactorB;     /**< (--) Nominal config data   */
        double                                          tNonMixingBand;         /**< (--) Nominal config data   */
        GunnsFluidHiFi3WayValveInputData*               tInputData;             /**< (--) Nominal input data    */
        double                                          tPosition;              /**< (--) Nominal input data    */
        GunnsFluidNode                                  tNodes[3];              /**< (--) Test nodes            */
        GunnsNodeList                                   tNodeList;              /**< (--) Test node list        */
        std::vector<GunnsBasicLink*>                    tLinks;                 /**< (--) Test links vector     */
        int                                             tPort0;                 /**< (--) Nominal init data     */
        int                                             tPort1;                 /**< (--) Nominal init data     */
        int                                             tPort2;                 /**< (--) Nominal init data     */
        double                                          tTimeStep;              /**< (--) Test time step        */
        DefinedFluidProperties*                         tFluidProperties;       /**< (--) Nominal init data     */
        PolyFluidConfigData*                            tFluidConfig;           /**< (--) Nominal init data     */
        PolyFluidInputData*                             tFluidInput1;           /**< (--) Nominal init data     */
        PolyFluidInputData*                             tFluidInput2;           /**< (--) Nominal init data     */
        PolyFluidInputData*                             tFluidInput3;           /**< (--) Nominal init data     */
        FriendlyGunnsFluidHiFiValveFor3Way*             tPathA;                 /**< (--) Embedded path A valve */
        FriendlyGunnsFluidHiFiValveFor3Way*             tPathB;                 /**< (--) Embedded path B valve */
        static int                                      TEST_ID;                /**< (--) Test identification number. */

    public:
        UtGunnsFluidHiFi3WayValve();
        virtual ~UtGunnsFluidHiFi3WayValve();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStep();
        void testStepNonMixing();
        void testDynamicPortMapping();
        void testComputeFlow();
        void testAccessMethods();
};

///@}

#endif
