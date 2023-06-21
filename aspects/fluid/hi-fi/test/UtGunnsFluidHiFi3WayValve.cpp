/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((aspects/fluid/conductor/GunnsFluidHiFi3WayValve.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHiFi3WayValve.hh"

/// @details  Test identification number.
int UtGunnsFluidHiFi3WayValve::TEST_ID = 2;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidHiFi3WayValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFi3WayValve::UtGunnsFluidHiFi3WayValve()
    :
    tArticle(),
    tLinkName(),
    tConfigData(),
    tPositionPathB(false),
    tCoefficientTypeA(GunnsFluidHiFiOrificeConfigData::CoeffTypes::DISCHARGE_COEFF),
    tCoefficientTypeB(GunnsFluidHiFiOrificeConfigData::CoeffTypes::DISCHARGE_COEFF),
    tCoefficientValueA(0.0),
    tCoefficientValueB(0.0),
    tThroatDiameterA(0.0),
    tThroatDiameterB(0.0),
    tCriticalReynoldsA(0.0),
    tCriticalReynoldsB(0.0),
    tExpansionScaleFactorA(0.0),
    tExpansionScaleFactorB(0.0),
    tFlowTuningFactorA(0.0),
    tFlowTuningFactorB(0.0),
    tNonMixingBand(0.0),
    tInputData(),
    tPosition(0.0),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(0),
    tPort1(0),
    tPort2(0),
    tTimeStep(0),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tFluidInput3(),
    tPathA(0),
    tPathB(0)
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidHiFi3WayValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFi3WayValve::~UtGunnsFluidHiFi3WayValve()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tFluidConfig;
    delete tFluidProperties;
    delete tFluidInput3;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::setUp()
{
    /// - Define nominal configuration data
    tLinkName              = "Test Hi-Fi Three Way Valve";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tPositionPathB         = true;
    tCoefficientTypeA      = GunnsFluidHiFiOrificeConfigData::CoeffTypes::DISCHARGE_COEFF,
    tCoefficientTypeB      = GunnsFluidHiFiOrificeConfigData::CoeffTypes::DISCHARGE_COEFF,
    tCoefficientValueA     = 0.7,
    tCoefficientValueB     = 0.6,
    tThroatDiameterA       = 0.005,
    tThroatDiameterB       = 0.005,
    tCriticalReynoldsA     = 2300.0,
    tCriticalReynoldsB     = 2300.0,
    tExpansionScaleFactorA = 1.0,
    tExpansionScaleFactorB = 1.0,
    tFlowTuningFactorA     = 1.0,
    tFlowTuningFactorB     = 1.0,
    tNonMixingBand         = 0.2;
    tConfigData            = new GunnsFluidHiFi3WayValveConfigData(tLinkName,
                                                                   &tNodeList,
                                                                   tPositionPathB,
                                                                   tCoefficientTypeA,
                                                                   tCoefficientTypeB,
                                                                   tCoefficientValueA,
                                                                   tCoefficientValueB,
                                                                   tThroatDiameterA,
                                                                   tThroatDiameterB,
                                                                   tCriticalReynoldsA,
                                                                   tCriticalReynoldsB,
                                                                   tExpansionScaleFactorA,
                                                                   tExpansionScaleFactorB,
                                                                   tFlowTuningFactorA,
                                                                   tFlowTuningFactorB,
                                                                   tNonMixingBand);

    /// - Set up nominal input data.
    tPosition              = 1.0;
    tInputData             = new GunnsFluidHiFi3WayValveInputData(tPosition);

    /// - Set up other init data.
    tPort0                 = 0;
    tPort1                 = 1;
    tPort2                 = 2;
    tTimeStep              = 0.1;

    /// - Set up fluid nodes
    double fractions1[2]   = {1.0, 0.0};
    tFluidInput1 = new PolyFluidInputData(300.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions1);              //massFractions
    double fractions2[2]   = {0.0, 1.0};
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          589.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      // mass
                                          fractions2);              //massFractions
    double fractions3[2]   = {0.5, 0.5};
    tFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          101.325,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions3);              //massFractions

    /// - Have to initialize the nodes with the fluid config (normally done by GUNNS)
    tFluidProperties       = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tFluidConfig           = new PolyFluidConfigData(tFluidProperties, types, 2);

    tNodes[0].initialize("UtTestNode0", tFluidConfig);
    tNodes[1].initialize("UtTestNode1", tFluidConfig);
    tNodes[2].initialize("UtTestNode2", tFluidConfig);

    tNodes[0].getContent()->initialize(*tFluidConfig, *tFluidInput1);
    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tFluidConfig, *tFluidInput3);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();
    tNodes[2].resetFlows();

    /// - Create the test article.
    tArticle               = new FriendlyGunnsFluidHiFi3WayValve;
    tPathA                 = static_cast<FriendlyGunnsFluidHiFiValveFor3Way*>(&tArticle->mPathA);
    tPathB                 = static_cast<FriendlyGunnsFluidHiFiValveFor3Way*>(&tArticle->mPathB);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
    // tCoefficientTypeA
    // tCoefficientTypeB
    // tCoefficientValueA
    // tCoefficientValueB
    // tThroatDiameterA
    // tThroatDiameterB
    // tCriticalReynoldsA
    // tCriticalReynoldsB
    // tExpansionScaleFactorA
    // tExpansionScaleFactorB
    // tFlowTuningFactorA
    // tFlowTuningFactorB
void UtGunnsFluidHiFi3WayValve::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                   == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientTypeA        == tConfigData->mCoefficientTypeA);
    CPPUNIT_ASSERT(tCoefficientTypeB        == tConfigData->mCoefficientTypeB);
    CPPUNIT_ASSERT(tCoefficientValueA       == tConfigData->mCoefficientValueA);
    CPPUNIT_ASSERT(tCoefficientValueB       == tConfigData->mCoefficientValueB);
    CPPUNIT_ASSERT(tThroatDiameterA         == tConfigData->mThroatDiameterA);
    CPPUNIT_ASSERT(tThroatDiameterB         == tConfigData->mThroatDiameterB);
    CPPUNIT_ASSERT(tCriticalReynoldsA       == tConfigData->mCriticalReynoldsA);
    CPPUNIT_ASSERT(tCriticalReynoldsB       == tConfigData->mCriticalReynoldsB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA   == tConfigData->mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB   == tConfigData->mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tFlowTuningFactorA       == tConfigData->mFlowTuningFactorA);
    CPPUNIT_ASSERT(tFlowTuningFactorB       == tConfigData->mFlowTuningFactorB);
    CPPUNIT_ASSERT(tNonMixingBand           == tConfigData->mNonMixingBand);

    /// - Check default config construction
    GunnsFluidHiFi3WayValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""                       == defaultConfig.mName);
    CPPUNIT_ASSERT(0                        == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCoefficientTypeA);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCoefficientTypeB);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCoefficientValueA);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCoefficientValueB);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mThroatDiameterA);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mThroatDiameterB);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCriticalReynoldsA);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mCriticalReynoldsB);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(1.0                      == defaultConfig.mFlowTuningFactorA);
    CPPUNIT_ASSERT(1.0                      == defaultConfig.mFlowTuningFactorB);
    CPPUNIT_ASSERT(0.0                      == defaultConfig.mNonMixingBand);

    /// - Check copy config construction
    GunnsFluidHiFi3WayValveConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                   == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientTypeA        == copyConfig.mCoefficientTypeA);
    CPPUNIT_ASSERT(tCoefficientTypeB        == copyConfig.mCoefficientTypeB);
    CPPUNIT_ASSERT(tCoefficientValueA       == copyConfig.mCoefficientValueA);
    CPPUNIT_ASSERT(tCoefficientValueB       == copyConfig.mCoefficientValueB);
    CPPUNIT_ASSERT(tThroatDiameterA         == copyConfig.mThroatDiameterA);
    CPPUNIT_ASSERT(tThroatDiameterB         == copyConfig.mThroatDiameterB);
    CPPUNIT_ASSERT(tCriticalReynoldsA       == copyConfig.mCriticalReynoldsA);
    CPPUNIT_ASSERT(tCriticalReynoldsB       == copyConfig.mCriticalReynoldsB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA   == copyConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB   == copyConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tFlowTuningFactorA       == copyConfig.mFlowTuningFactorA);
    CPPUNIT_ASSERT(tFlowTuningFactorB       == copyConfig.mFlowTuningFactorB);
    CPPUNIT_ASSERT(tNonMixingBand           == copyConfig.mNonMixingBand);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testInput()
{
    UT_RESULT;

    /// - Check nominal input data construction
    CPPUNIT_ASSERT(tPosition              == tInputData->mPosition);

    /// - Check default input data construction
    GunnsFluidHiFi3WayValveInputData defaultInput;
    CPPUNIT_ASSERT(0.0                    == defaultInput.mPosition);

    /// - Check copy input data construction
    GunnsFluidHiFi3WayValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tPosition              == copyInput.mPosition);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test config data
    CPPUNIT_ASSERT(false                  == tArticle->mPositionPathB);
    CPPUNIT_ASSERT(0.0                    == tArticle->mNonMixingBand);
    CPPUNIT_ASSERT(0.0                    == tArticle->mPosition);

    /// @test init flag
    CPPUNIT_ASSERT(false                  == tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidHiFi3WayValve* article          =  new GunnsFluidHiFi3WayValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test base class data.
    CPPUNIT_ASSERT(tLinkName                 == tArticle->getName());

    /// @test config and input data.
    CPPUNIT_ASSERT(tPositionPathB            == tArticle->mPositionPathB);
    CPPUNIT_ASSERT(tNonMixingBand            == tArticle->mNonMixingBand);
    CPPUNIT_ASSERT(tPosition                 == tArticle->mPosition);

    /// @test embedded fluid valve's initialization.
    CPPUNIT_ASSERT(tThroatDiameterA             == tPathA->mThroatDiameter);
    CPPUNIT_ASSERT(tThroatDiameterB             == tPathB->mThroatDiameter);
    CPPUNIT_ASSERT(tExpansionScaleFactorA       == tPathA->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tExpansionScaleFactorB       == tPathB->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactorA           == tPathA->mFlowTuningFactor);
    CPPUNIT_ASSERT(tFlowTuningFactorB           == tPathB->mFlowTuningFactor);
    CPPUNIT_ASSERT(0.0                          == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0                          == tPathB->mPosition);

    /// @test init flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test path A & B sub-valve links are added to the network links vector.
    CPPUNIT_ASSERT(3 == tLinks.size());

    /// @test The setMinLinearizationPotential method.
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->setMinLinearizationPotential(1.0);
    }
    CPPUNIT_ASSERT(1.0                       == tArticle->mMinLinearizationPotential);
    CPPUNIT_ASSERT(1.0                       == tPathA->mMinLinearizationPotential);
    CPPUNIT_ASSERT(1.0                       == tPathB->mMinLinearizationPotential);

    /// @test Verify restartModel functionality
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->restart();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testInitializationExceptions()
{
    UT_RESULT;

    /// @test for exception on non-mixing band < 0
    tConfigData->mNonMixingBand = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);

    /// @test for exception on non-mixing band >= 1
    tConfigData->mNonMixingBand = 1.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mNonMixingBand = tNonMixingBand;

    /// @test for exception on position > 1
    tInputData->mPosition = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);

    /// @test for exception on position < 0
    tInputData->mPosition = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tInputData->mPosition = 0.0;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mNonMixingBand = 0.0;
    tInputData->mPosition       = 0.5;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Step the article to update values in the flow path valves.
    tArticle->step(0.0);
    /// @test  Restart method clears the embedded flow path values.
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->restart();
    }

    CPPUNIT_ASSERT(0.5 == tArticle->mPosition);
    CPPUNIT_ASSERT(0.0 == tArticle->mPathA.getFlux());
    CPPUNIT_ASSERT(0.0 == tArticle->mPathB.getFlux());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mNonMixingBand = 0.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set test article inputs.
    tArticle->mPotentialVector[0]  = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1]  = tFluidInput2->mPressure;
    tArticle->mPotentialVector[2]  = tFluidInput3->mPressure;
    tPathA->mPotentialVector[0]    = tArticle->mPotentialVector[0];
    tPathA->mPotentialVector[1]    = tArticle->mPotentialVector[2];
    tPathB->mPotentialVector[0]    = tArticle->mPotentialVector[1];
    tPathB->mPotentialVector[1]    = tArticle->mPotentialVector[2];
    tArticle->mAdmittanceMatrix[0] = 1.0;
    tArticle->mAdmittanceMatrix[8] = 1.0;
    tArticle->mSourceVector[0]     = 1.0;
    tArticle->mSourceVector[2]     = 1.0;

    /// - Test embedded valve positions from 3-way position relative to configured path.
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->step(tTimeStep);
    }
    CPPUNIT_ASSERT(0.0                         == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0                         == tPathB->mPosition);

    /// - Test embedded valve potential vectors get set.
    CPPUNIT_ASSERT(tFluidInput1->mPressure     == tPathA->mPotentialVector[0]);
    CPPUNIT_ASSERT(tFluidInput3->mPressure     == tPathA->mPotentialVector[1]);
    CPPUNIT_ASSERT(tFluidInput2->mPressure     == tPathB->mPotentialVector[0]);
    CPPUNIT_ASSERT(tFluidInput3->mPressure     == tPathB->mPotentialVector[1]);

    /// - Test parent link zeroes contributions to network system.
    CPPUNIT_ASSERT(false                       == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[4]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[5]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[6]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[7]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mAdmittanceMatrix[8]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mSourceVector[0]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mSourceVector[1]);
    CPPUNIT_ASSERT(0.0                         == tArticle->mSourceVector[2]);

    /// - Test embedded valve contributions to the network system.
    CPPUNIT_ASSERT(0.0 == tPathA->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(0.0 == tPathA->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(0.0 == tPathA->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT(0.0 == tPathA->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(0.0 <  tPathB->mSystemConductance);
    CPPUNIT_ASSERT( tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(-tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(-tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT( tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(true                        == tPathB->mAdmittanceUpdate);

    /// - Now repeat the previous tests, only the position is now mixed-flow and defined relative to
    ///   path A.
    tArticle->mPosition                        =  0.2;
    tArticle->mPositionPathB                   =  false;
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->step(tTimeStep);
    }
    CPPUNIT_ASSERT(0.2                         == tPathA->mPosition);
    CPPUNIT_ASSERT(0.8                         == tPathB->mPosition);

    CPPUNIT_ASSERT(0.0 <  tPathA->mSystemConductance);
    CPPUNIT_ASSERT( tPathA->mSystemConductance == tPathA->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(-tPathA->mSystemConductance == tPathA->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(-tPathA->mSystemConductance == tPathA->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT( tPathA->mSystemConductance == tPathA->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(true                        == tPathA->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0 <  tPathB->mSystemConductance);
    CPPUNIT_ASSERT( tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[0]);
    CPPUNIT_ASSERT(-tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[1]);
    CPPUNIT_ASSERT(-tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[2]);
    CPPUNIT_ASSERT( tPathB->mSystemConductance == tPathB->mAdmittanceMatrix[3]);
    CPPUNIT_ASSERT(true                        == tPathB->mAdmittanceUpdate);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method with non-mixing configurations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testStepNonMixing()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test  Non-mixing, position relative to flow path B positions:
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0 == tPathB->mPosition);

    tArticle->mPosition = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.5;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.6;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.4;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.8;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.2;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    /// @test  Non-mixing, position relative to flow path A positions:
    tArticle->mPositionPathB = false;
    tArticle->mPosition = 1.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0 == tPathB->mPosition);

    tArticle->mPosition = 0.5;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.6;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.4;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.8;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.2;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tPathB->mPosition, DBL_EPSILON);

    /// @test  Handles non-mixing range = 1:
    tArticle->mNonMixingBand = 1.0;
    tArticle->mPosition      = 1.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0 == tPathB->mPosition);

    tArticle->mPosition = 0.5;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPosition = 0.999;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    tArticle->mPosition = 0.001;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathA->mPosition, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tPathB->mPosition, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test dynamic port mapping.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testDynamicPortMapping()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Set port mapping commands to map port 1 to node 2 (ground), verify the link and embedded
    ///   valves take it.
    tArticle->mUserPortSelect     =  1;
    tArticle->mUserPortSetControl =  GunnsBasicLink::GROUND;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(2              == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(2              == tPathB->mNodeMap[0]);
    CPPUNIT_ASSERT(&tNodes[2]     == tPathB->mNodes[0]);

    /// - Set an invalid port command and verify it is rejected by the link and embedded valves do
    ///   not change.
    tArticle->mUserPortSelect     =  1;
    tArticle->mUserPortSelectNode =  0;
    tArticle->mUserPortSetControl =  GunnsBasicLink::EXECUTE;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(2              == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(2              == tPathB->mNodeMap[0]);
    CPPUNIT_ASSERT(&tNodes[2]     == tPathB->mNodes[0]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the compute flow method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testComputeFlow()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mNonMixingBand = 0.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    tArticle->mPotentialVector[0] = tFluidInput1->mPressure;
    tArticle->mPotentialVector[1] = tFluidInput2->mPressure;
    tArticle->mPotentialVector[2] = tFluidInput3->mPressure;
    tPathA->mPotentialVector[0]   = tArticle->mPotentialVector[0];
    tPathA->mPotentialVector[1]   = tArticle->mPotentialVector[2];
    tPathB->mPotentialVector[0]   = tArticle->mPotentialVector[1];
    tPathB->mPotentialVector[1]   = tArticle->mPotentialVector[2];
    tArticle->mPosition           =  0.5;
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->step(tTimeStep);
    }
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->computeFlows(tTimeStep);
    }
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->transportFlows(tTimeStep);
    }

    /// - Test embedded valve flow rates are both positive and path A is larger than path B because
    ///   it has higher conductivity and inlet pressure.
    CPPUNIT_ASSERT(0.0                         <  tPathA->mFlowRate);
    CPPUNIT_ASSERT(0.0                         <  tPathB->mFlowRate);
    CPPUNIT_ASSERT(tPathB->mFlowRate           <  tPathA->mFlowRate);

    /// - Test fluid transport between the proper nodes.
    CPPUNIT_ASSERT(tPathA->mFlowRate           == tNodes[0].getOutflux());
    CPPUNIT_ASSERT(tPathB->mFlowRate           == tNodes[1].getOutflux());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tPathA->mFlowRate + tPathB->mFlowRate,
                                 tNodes[2].getInflux(), DBL_EPSILON);

    /// - Test total 3-way flow parameters.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlux,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mFlowRate,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mVolFlowRate, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mPower,       DBL_EPSILON);

    /// - Verify port directions are updated for parent link and valve sub-links.
    tArticle->mPotentialVector[0] = 0.8;
    tArticle->mPotentialVector[1] = 0.8;
    tArticle->mPotentialVector[2] = 0.0;
    tPathA->mPotentialVector[0]   = tArticle->mPotentialVector[0];
    tPathA->mPotentialVector[1]   = tArticle->mPotentialVector[2];
    tPathB->mPotentialVector[0]   = tArticle->mPotentialVector[1];
    tPathB->mPotentialVector[1]   = tArticle->mPotentialVector[2];
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->computeFlows(tTimeStep);
    }

    CPPUNIT_ASSERT(tPathA->mFlux > 0.0);
    CPPUNIT_ASSERT(tPathB->mFlux > 0.0);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::NONE   == tArticle->mPortDirections[2]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tPathA->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tPathA->mPortDirections[1]);
    CPPUNIT_ASSERT(GunnsBasicLink::SOURCE == tPathB->mPortDirections[0]);
    CPPUNIT_ASSERT(GunnsBasicLink::SINK   == tPathB->mPortDirections[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the getter & setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFi3WayValve::testAccessMethods()
{
    UT_RESULT;

    tArticle->setPosition(0.6);
    CPPUNIT_ASSERT(0.6    == tArticle->mPosition);
    CPPUNIT_ASSERT(tPathA == tArticle->getPathA());
    CPPUNIT_ASSERT(tPathB == tArticle->getPathB());

    UT_PASS_LAST;
}
