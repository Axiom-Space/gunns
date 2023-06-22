/************************** TRICK HEADER ***********************************************************
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHiFiCheckValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Check Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiCheckValve::UtGunnsFluidHiFiCheckValve()
    :
    tLinkName(),
    tCoefficientType(),
    tCoefficientValue(),
    tThroatDiameter(),
    tCriticalReynolds(),
    tExpansionScaleFactor(),
    tFlowTuningFactor(),
    tRateLimit(),
    tClosePressure(),
    tOpenPressure(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tPosition(),
    tMalfLeakThruFlag(),
    tMalfLeakThruValue(),
    tMalfStuckFlag(),
    tMalfFailToFlag(),
    tMalfFailToValue(),
    tInputData(),
    tArticle(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput1(),
    tFluidInput2(),
    tFluidInput3(),
    fractions()
 {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Check Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiCheckValve::~UtGunnsFluidHiFiCheckValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::setUp()
{
    tLinkName             = "Test Fluid Hi-Fi Check Valve";
    tNodeList.mNumNodes   = 5;
    tNodeList.mNodes      = tNodes;
    tPort0                = 0;
    tPort1                = 1;
    tTimeStep             = 0.01;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_WATER;
    fractions = new double[3];
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    fractions[2] = 0.0;
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 3);
    tFluidInput1 = new PolyFluidInputData(290.0,                    //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    tFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tFluidConfig, tFluidInput1);
    tNodes[1].initialize("UtTestNode1", tFluidConfig, tFluidInput2);

    tNodes[0].resetFlows();
    tNodes[1].resetFlows();

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    tFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    tNodes[2].initialize("UtTestNode2", tFluidConfig, tFluidInput3);
    tNodes[3].initialize("UtTestNode3", tFluidConfig, tFluidInput3);

    tNodes[2].resetFlows();
    tNodes[3].resetFlows();

    /// - Define nominal configuration data
    tCoefficientType        = GunnsFluidHiFiValveConfigData::DISCHARGE_COEFF;
    tCoefficientValue       = 0.7;
    tThroatDiameter         = 0.005;
    tCriticalReynolds       = 2300.0;
    tExpansionScaleFactor   = 1.0;
    tFlowTuningFactor       = 1.0;
    tRateLimit              = 1.0;
    tClosePressure          = 0.1;
    tOpenPressure           = 1.0;
    tConfigData             = new GunnsFluidHiFiCheckValveConfigData(tLinkName,
                                                                     &tNodeList,
                                                                     tCoefficientType,
                                                                     tCoefficientValue,
                                                                     tThroatDiameter,
                                                                     tCriticalReynolds,
                                                                     tExpansionScaleFactor,
                                                                     tFlowTuningFactor,
                                                                     tRateLimit,
                                                                     tClosePressure,
                                                                     tOpenPressure);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 1.0;
    tPosition          = 1.0;
    tMalfLeakThruFlag  = false;
    tMalfLeakThruValue = 1.0;
    tMalfStuckFlag     = false;
    tMalfFailToFlag    = false;
    tMalfFailToValue   = 0.0;
    tInputData         = new GunnsFluidHiFiCheckValveInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tPosition,
                                                                  tMalfLeakThruFlag,
                                                                  tMalfLeakThruValue,
                                                                  tMalfStuckFlag,
                                                                  tMalfFailToFlag,
                                                                  tMalfFailToValue);

    tArticle = new FriendlyGunnsFluidHiFiCheckValve;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tFluidConfig;
    delete tFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model construction of configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 01: testConfig ......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                                        == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                                           == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == tConfigData->mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == tConfigData->mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == tConfigData->mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == tConfigData->mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == tConfigData->mFlowTuningFactor);
    CPPUNIT_ASSERT(tRateLimit                                       == tConfigData->mRateLimit);
    CPPUNIT_ASSERT(tClosePressure                                   == tConfigData->mClosePressure);
    CPPUNIT_ASSERT(tOpenPressure                                    == tConfigData->mOpenPressure);

    /// - Check default config construction
    GunnsFluidHiFiCheckValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""                                               == defaultConfig.mName);
    CPPUNIT_ASSERT(0                                                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(GunnsFluidHiFiValveConfigData::DISCHARGE_COEFF   == defaultConfig.mCoefficientType);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCoefficientValue);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mThroatDiameter);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(1.0                                              == defaultConfig.mFlowTuningFactor);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mRateLimit);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mClosePressure);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mOpenPressure);

    /// - Check copy config construction
    GunnsFluidHiFiCheckValveConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                                        == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                                           == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == copyConfig.mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == copyConfig.mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == copyConfig.mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == copyConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == copyConfig.mFlowTuningFactor);
    CPPUNIT_ASSERT(tRateLimit                                       == copyConfig.mRateLimit);
    CPPUNIT_ASSERT(tClosePressure                                   == copyConfig.mClosePressure);
    CPPUNIT_ASSERT(tOpenPressure                                    == copyConfig.mOpenPressure);

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testInput()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 02: testInput .......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == tInputData->mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == tInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == tInputData->mMalfLeakThruValue);
    CPPUNIT_ASSERT(tMalfStuckFlag     == tInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag    == tInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue   == tInputData->mMalfFailToValue);

    /// - Check default config construction
    GunnsFluidHiFiCheckValveInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mPosition);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfLeakThruValue);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfFailToValue);

    /// - Check copy config construction
    GunnsFluidHiFiCheckValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == copyInput.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == copyInput.mMalfLeakThruValue);
    CPPUNIT_ASSERT(tMalfStuckFlag     == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag    == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue   == copyInput.mMalfFailToValue);

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 03: testDefaultConstruction .........";

    /// @test config data
    CPPUNIT_ASSERT(0.0                               == tArticle->mCdDefault);
    CPPUNIT_ASSERT(0.0                               == tArticle->mThroatDiameter);
    CPPUNIT_ASSERT(0.0                               == tArticle->mReCritical);
    CPPUNIT_ASSERT(0.0                               == tArticle->mExpansionScaleFactor);
    CPPUNIT_ASSERT(0.0                               == tArticle->mFlowTuningFactor);
    CPPUNIT_ASSERT(0.0                               == tArticle->mThroatArea);
    CPPUNIT_ASSERT(0.0                               == tArticle->mReActual);
    CPPUNIT_ASSERT(0.0                               == tArticle->mCdActual);
    CPPUNIT_ASSERT(0.0                               == tArticle->mSystemConductance);
    CPPUNIT_ASSERT(0.0                               == tArticle->mPressureRatio);
    CPPUNIT_ASSERT(false                             == tArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfLeakThruValue);
    CPPUNIT_ASSERT(0.0                               == tArticle->mPosition);
    CPPUNIT_ASSERT(0.0                               == tArticle->mLeakArea);
    CPPUNIT_ASSERT(0.0                               == tArticle->mLastLeakRate);
    CPPUNIT_ASSERT(0.0                               == tArticle->mRateLimit);
    CPPUNIT_ASSERT(0.0                               == tArticle->mClosePressure);
    CPPUNIT_ASSERT(0.0                               == tArticle->mOpenPressure);
    CPPUNIT_ASSERT(false                             == tArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(false                             == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfFailToValue);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidHiFiCheckValve* article = new GunnsFluidHiFiCheckValve();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 04: testNominalInitialization .......";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidHiFiCheckValve article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test initialization of the base class
    CPPUNIT_ASSERT(tLinkName                         == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue                == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tThroatDiameter                   == article.mThroatDiameter);
    CPPUNIT_ASSERT(tRateLimit                        == article.mRateLimit);
    CPPUNIT_ASSERT(tClosePressure                    == article.mClosePressure);
    CPPUNIT_ASSERT(tOpenPressure                     == article.mOpenPressure);

    /// @test config and input data
    CPPUNIT_ASSERT(tPosition                         == article.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag                 == article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue                == article.mMalfLeakThruValue);
    CPPUNIT_ASSERT(tMalfStuckFlag                    == article.mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag                   == article.mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue                  == article.mMalfFailToValue);

    /// @test state data
    CPPUNIT_ASSERT(0.0                               == article.mLeakArea);
    CPPUNIT_ASSERT(0.0                               == article.mLastLeakRate);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test accessors and getters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 06: testInitializationExceptions ....";

    /// - Default construct a test article.
    GunnsFluidHiFiCheckValve article;

    /// @test    Initialization exception on invalid config data: mRateLimit < 0.
    tConfigData->mRateLimit  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mRateLimit  = tRateLimit;

    /// @test    Initialization exception on invalid config data: mOpenPosition < mClosePosition.
    tConfigData->mClosePressure  = +DBL_EPSILON * 0.5;
    tConfigData->mOpenPressure   = -DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mClosePressure  = tClosePressure;
    tConfigData->mOpenPressure   = tOpenPressure;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue < 0.
    tInputData->mMalfFailToValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfFailToValue = tMalfFailToValue;

    /// @test    Initialization exception on invalid input data: mMalfFailToValue > 1.
    tInputData->mMalfFailToValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfFailToValue = tMalfFailToValue;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testRestart()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 07: testRestart .....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mPower = 1.0;

    /// @test restart rests terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0                               == tArticle->mPower);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testAccessors()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testModifiers()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 09: testModifiers .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    
    /// @test    The stuck malf is set by the access method.
    tArticle->setMalfStuck(true);
    CPPUNIT_ASSERT(true  == tArticle->mMalfStuckFlag);

    /// @test    The stuck malf is reset by the access method.
    tArticle->setMalfStuck();
    CPPUNIT_ASSERT(false == tArticle->mMalfStuckFlag);

    /// @test    The fail to position malf is set by the access method.
    tArticle->setMalfFailTo(true, 1.0);
    CPPUNIT_ASSERT(true  == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(1.0   == tArticle->mMalfFailToValue);

    /// @test    The fail to position malf is reset by the access method.
    tArticle->setMalfFailTo();
    CPPUNIT_ASSERT(false == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFailToValue);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testUpdateStateNominal()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 10: testUpdateStateNominal .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Effectively disable rate limiting.
    tArticle->mRateLimit = 1.0 / tTimeStep;

    /// @test    With valve closed and sufficient positive delta pressure so it full opens.
    tArticle->mPosition = 0.0;
    tArticle->mPotentialDrop = tOpenPressure;
    double expected          = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient positive delta pressure so it partially opens.
    tArticle->mPotentialDrop = tClosePressure + 0.75 * (tOpenPressure - tClosePressure);
    expected                 = 0.75;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient negative delta pressure so it partially closes.
    tArticle->mPotentialDrop = tClosePressure + 0.25 * (tOpenPressure - tClosePressure);
    expected                 = 0.25;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With sufficient negative delta pressure so it closes.
    tArticle->mPotentialDrop = tClosePressure;
    expected                 = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient negative delta pressure it partially closes.
    tArticle->mPotentialDrop = tClosePressure + 0.25 * (tOpenPressure - tClosePressure);
    expected                 = 0.25;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient positive delta pressure so it partially opens.
    tArticle->mPotentialDrop = tClosePressure + 0.75 * (tOpenPressure - tClosePressure);
    expected                 = 0.75;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With sufficient positive delta pressure so it fully opens.
    tArticle->mPotentialDrop = tOpenPressure;
    expected                 = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (rate limiting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testUpdateStateRateLimited()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 11: testUpdateStateRateLimited .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Rate limiting with closed valve and delta p larger than rate limit * dt.
    tArticle->mRateLimit     = 0.25;
    tArticle->mPosition      = 0.0;
    tArticle->mPotentialDrop = tOpenPressure;
    double expected = tArticle->mRateLimit * tTimeStep;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testUpdateStateNoRange()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 12: testUpdateStateNoRange .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Effectively disable rate limiting and set open and closed pressure to zero.
    tArticle->mRateLimit     = 1.0 / tTimeStep;
    tArticle->mOpenPressure  = 0.0;
    tArticle->mClosePressure = 0.0;

    /// @test    With valve closed and a very small positive delta pressure so it fully opens.
    tArticle->mPosition      = 0.0;
    tArticle->mPotentialDrop = +DBL_EPSILON;
    double expected          = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With valve open and a very small negative delta pressure so it closes.
    tArticle->mPotentialDrop = -DBL_EPSILON;
    expected = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Check Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiCheckValve::testUpdateStateMalfunction()
{
    std::cout << "\n UtGunnsFluidHiFiCheckValve ........ 13: testUpdateStateNoRange .....................";

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mClosePressure = 12.0;
    tConfigData->mOpenPressure  = 16.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tNodes[0].getContent()->setPressure(112.0);
    tNodes[1].getContent()->setPressure(100.0);
    tNodes[0].getContent()->setTemperature(283.0);
    tNodes[1].getContent()->setTemperature(283.0);
    tArticle->mRateLimit     = 1.0 / tTimeStep;

    tArticle->mPotentialDrop      = 12.0;
    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         tArticle->mMinLinearizationPotential,
                                                                         tNodes[0].getOutflow(),
                                                                         tNodes[1].getOutflow());

    /// @test     Update state with initial leak rate malfunction insertion.
    tArticle->mMalfLeakThruFlag  = true;
    tArticle->mMalfLeakThruValue = leakRate;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 tArticle->mMalfLeakThruValue,
                                 DBL_EPSILON);

    tConfigData->mClosePressure = tClosePressure;
    tConfigData->mOpenPressure  = tOpenPressure;

    /// - Create handle fail to position criteria 
    tArticle->mMalfStuckFlag = false;
    tArticle->mMalfFailToFlag = true;
    tArticle->mMalfFailToValue = 0.4;
    tArticle->updateState(tTimeStep);
    
    /// @test     Test <1.0 position setting for handle failure
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, tArticle->mPosition, 0.0);

    std::cout << "... Pass";
}