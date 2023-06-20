/************************** TRICK HEADER ***********************************************************
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidHiFiSolenoidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Solenoid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiSolenoidValve::UtGunnsFluidHiFiSolenoidValve()
    :
    tLinkName(),
    tCoefficientType(),
    tCoefficientValue(),
    tThroatDiameter(),
    tCriticalReynolds(),
    tExpansionScaleFactor(),
    tFlowTuningFactor(),
    tOpenVoltage(),
    tOpenTime(),
    tCloseVoltage(),
    tCloseTime(),
    tConfigData(),
    tMalfBlockageFlag(),
    tMalfBlockageValue(),
    tPosition(),
    tMalfLeakThruFlag(),
    tMalfLeakThruValue(),
    tVoltage(),
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
/// @details  Default destructs this GUNNS Fluid Solenoid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidHiFiSolenoidValve::~UtGunnsFluidHiFiSolenoidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::setUp()
{
    tLinkName             = "Test Fluid Hi-Fi Solenoid Valve";
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
    tOpenVoltage            = 10.0;
    tOpenTime               = 0.075;
    tCloseVoltage           = 6.0;
    tCloseTime              = 0.025;
    tConfigData             = new GunnsFluidHiFiSolenoidValveConfigData(tLinkName,
                                                                        &tNodeList,
                                                                        tCoefficientType,
                                                                        tCoefficientValue,
                                                                        tThroatDiameter,
                                                                        tCriticalReynolds,
                                                                        tExpansionScaleFactor,
                                                                        tFlowTuningFactor,
                                                                        tOpenVoltage,
                                                                        tOpenTime,
                                                                        tCloseVoltage,
                                                                        tCloseTime);

    /// - Define nominal input data
    tMalfBlockageFlag  = false;
    tMalfBlockageValue = 1.0;
    tPosition          = 1.0;
    tMalfLeakThruFlag  = false;
    tMalfLeakThruValue = 1.0;
    tVoltage           = 0.0;
    tMalfStuckFlag     = false;
    tMalfFailToFlag    = false;
    tMalfFailToValue   = 0.0;
    tInputData         = new GunnsFluidHiFiSolenoidValveInputData(tMalfBlockageFlag,
                                                                  tMalfBlockageValue,
                                                                  tPosition,
                                                                  tMalfLeakThruFlag,
                                                                  tMalfLeakThruValue,
                                                                  tVoltage,
                                                                  tMalfStuckFlag,
                                                                  tMalfFailToFlag,
                                                                  tMalfFailToValue);

    tArticle = new FriendlyGunnsFluidHiFiSolenoidValve;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::tearDown()
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
/// @details  Tests for GUNNS Fluid Solenoid Valve link model construction of configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 01: testConfig ......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName                                        == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                                           == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == tConfigData->mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == tConfigData->mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == tConfigData->mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == tConfigData->mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == tConfigData->mFlowTuningFactor);
    CPPUNIT_ASSERT(tOpenVoltage                                     == tConfigData->mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime                                        == tConfigData->mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage                                    == tConfigData->mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime                                       == tConfigData->mCloseTime);

    /// - Check default config construction
    GunnsFluidHiFiSolenoidValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""                                               == defaultConfig.mName);
    CPPUNIT_ASSERT(0                                                == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(GunnsFluidHiFiValveConfigData::DISCHARGE_COEFF   == defaultConfig.mCoefficientType);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCoefficientValue);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mThroatDiameter);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(1.0                                              == defaultConfig.mFlowTuningFactor);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mOpenVoltage);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mOpenTime);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCloseVoltage);
    CPPUNIT_ASSERT(0.0                                              == defaultConfig.mCloseTime);

    /// - Check copy config construction
    GunnsFluidHiFiSolenoidValveConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName                                        == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                                           == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tCoefficientType                                 == copyConfig.mCoefficientType);
    CPPUNIT_ASSERT(tCoefficientValue                                == copyConfig.mCoefficientValue);
    CPPUNIT_ASSERT(tThroatDiameter                                  == copyConfig.mThroatDiameter);
    CPPUNIT_ASSERT(tCriticalReynolds                                == copyConfig.mCriticalReynolds);
    CPPUNIT_ASSERT(tExpansionScaleFactor                            == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tFlowTuningFactor                                == copyConfig.mFlowTuningFactor);
    CPPUNIT_ASSERT(tOpenVoltage                                     == copyConfig.mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime                                        == copyConfig.mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage                                    == copyConfig.mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime                                       == copyConfig.mCloseTime);

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testInput()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 02: testInput .......................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tMalfBlockageFlag  == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == tInputData->mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == tInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == tInputData->mMalfLeakThruValue);
    CPPUNIT_ASSERT(tVoltage           == tInputData->mVoltage);
    CPPUNIT_ASSERT(tMalfStuckFlag     == tInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag    == tInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue   == tInputData->mMalfFailToValue);

    /// - Check default config construction
    GunnsFluidHiFiSolenoidValveInputData defaultInput;
    CPPUNIT_ASSERT(false              == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mPosition);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfLeakThruValue);
    CPPUNIT_ASSERT(0.0                == defaultInput.mVoltage);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(false              == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                == defaultInput.mMalfFailToValue);

    /// - Check copy config construction
    GunnsFluidHiFiSolenoidValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tPosition          == copyInput.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue == copyInput.mMalfLeakThruValue);
    CPPUNIT_ASSERT(tVoltage           == copyInput.mVoltage);
    CPPUNIT_ASSERT(tMalfStuckFlag     == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag    == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue   == copyInput.mMalfFailToValue);

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 03: testDefaultConstruction .........";

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
    CPPUNIT_ASSERT(0.0                               == tArticle->mOpenVoltage);
    CPPUNIT_ASSERT(0.0                               == tArticle->mOpenTime);
    CPPUNIT_ASSERT(0.0                               == tArticle->mCloseVoltage);
    CPPUNIT_ASSERT(0.0                               == tArticle->mCloseTime);
    CPPUNIT_ASSERT(0.0                               == tArticle->mVoltage);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                               == tArticle->mMalfFailToValue);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidHiFiSolenoidValve* article = new GunnsFluidHiFiSolenoidValve();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 04: testNominalInitialization .......";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidHiFiSolenoidValve article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test initialization of the base class
    CPPUNIT_ASSERT(tLinkName                         == article.getName());
    CPPUNIT_ASSERT(tMalfBlockageValue                == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tThroatDiameter                   == article.mThroatDiameter);
    CPPUNIT_ASSERT(tOpenVoltage                      == article.mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime                         == article.mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage                     == article.mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime                        == article.mCloseTime);

    /// @test config and input data
    CPPUNIT_ASSERT(tPosition                         == article.mPosition);
    CPPUNIT_ASSERT(tMalfLeakThruFlag                 == article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT(tMalfLeakThruValue                == article.mMalfLeakThruValue);
    CPPUNIT_ASSERT(tVoltage                          == article.mVoltage);
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
void UtGunnsFluidHiFiSolenoidValve::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 06: testInitializationExceptions ....";
    /// - Default construct a test article.
    GunnsFluidHiFiSolenoidValve article;

    /// @test    Initialization exception on invalid config data: mOpenTime < 0.
    tConfigData->mOpenTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mOpenTime  = tOpenTime;

    /// @test    Initialization exception on invalid config data: mCloseTime < 0.
    tConfigData->mCloseTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCloseTime  = tCloseTime;

    /// @test    Initialization exception on invalid config data: mOpenPosition < mClosePosition.
    tConfigData->mCloseVoltage  = +DBL_EPSILON * 0.5;
    tConfigData->mOpenVoltage   = -DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tConfigData->mCloseVoltage  = tCloseVoltage;
    tConfigData->mOpenVoltage   = tOpenVoltage;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
    tInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfBlockageValue = tMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
    tInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mMalfBlockageValue = tMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mPosition < 0.
    tInputData->mPosition = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mPosition = tPosition;

    /// @test    Initialization exception on invalid input data: mPosition > 1.
    tInputData->mPosition = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mPosition = tPosition;

    /// @test    Initialization exception on invalid input data: mVoltage < 0.
    tInputData->mVoltage = 0.0 - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    tInputData->mVoltage = tVoltage;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testRestart()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 07: testRestart .....................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set some stuff in the article and its base class that should be reset on restart.
    tArticle->mPower                 = 1.0;

    /// @test restart rests terms
    tArticle->restart();

    CPPUNIT_ASSERT(0.0                               == tArticle->mPower);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testAccessors()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 08: testAccessors .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    {
        /// @test    For more than nominal full open position.
        const double expected = 1.00;
        tArticle->mVoltage = expected;
        const double returned = tArticle->getPosition();
        CPPUNIT_ASSERT(expected                 == returned);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testModifiers()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 09: testModifiers .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    
    /// @test    The stuck malf is set by the access method.
    tArticle->setVoltage(1.0);
    CPPUNIT_ASSERT(1.0  == tArticle->mVoltage);

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
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testUpdateStateNominal()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 10: testUpdateStateNominal .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Effectively disable rate limiting.
    tArticle->mOpenTime = tTimeStep;
    tArticle->mCloseTime = tTimeStep;

    /// @test    With valve closed and sufficient positive delta voltage so it full opens.
    tArticle->mPosition = 0.0;
    tArticle->mVoltage = tOpenVoltage;
    double expected          = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient positive delta voltage so it partially opens.
    tArticle->mVoltage = tCloseVoltage + 0.75 * (tOpenVoltage - tCloseVoltage);
    expected                 = 0.75;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient negative delta voltage so it partially closes.
    tArticle->mVoltage = tCloseVoltage + 0.25 * (tOpenVoltage - tCloseVoltage);
    expected                 = 0.25;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With sufficient negative delta voltage so it closes.
    tArticle->mVoltage = tCloseVoltage;
    expected                 = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient negative delta voltage it partially closes.
    tArticle->mVoltage = tCloseVoltage + 0.25 * (tOpenVoltage - tCloseVoltage);
    expected                 = 0.25;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With insufficient positive delta voltage so it partially opens.
    tArticle->mVoltage = tCloseVoltage + 0.75 * (tOpenVoltage - tCloseVoltage);
    expected                 = 0.75;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With sufficient positive delta voltage so it fully opens.
    tArticle->mVoltage = tOpenVoltage;
    expected                 = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (rate limiting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testUpdateStateRateLimited()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 11: testUpdateStateRateLimited .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test    Rate limiting with closed valve and delta v larger than rate limit * dt.
    tArticle->mOpenTime     = 0.075;
    tArticle->mPosition      = 0.0;
    tArticle->mVoltage = tOpenVoltage;
    double expected = tTimeStep / tOpenTime;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    Rate limiting with open valve and delta v larger than rate limit * dt.
    tArticle->mCloseTime     = 0.025;
    tArticle->mPosition      = 1.0;
    tArticle->mVoltage = tCloseVoltage;
    expected = 1.0 - tTimeStep / tCloseTime;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testUpdateStateNoRange()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 12: testUpdateStateNoRange .....................";

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Effectively disable rate limiting and set open and closed voltage to zero.
    tArticle->mOpenTime = tTimeStep;
    tArticle->mCloseTime = tTimeStep;
    tArticle->mOpenVoltage  = 0.0;
    tArticle->mCloseVoltage = 0.0;

    /// @test    With valve closed and a very small positive delta voltage so it fully opens.
    tArticle->mPosition = 0.0;
    tArticle->mVoltage  = +DBL_EPSILON;
    double expected     = 1.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    /// @test    With valve open and a very small negative delta voltage so it closes.
    tArticle->mVoltage = -DBL_EPSILON;
    expected = 0.0;
    tArticle->updateState(tTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->mPosition, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidHiFiSolenoidValve::testUpdateStateMalfunction()
{
    std::cout << "\n UtGunnsFluidHiFiSolenoidValve ........ 13: testUpdateStateNoRange .....................";

    /// - Initialize default test article with nominal initialization data.
    tConfigData->mCloseVoltage = 12.0;
    tConfigData->mOpenVoltage  = 16.0;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    tNodes[0].getContent()->setPressure(112.0);
    tNodes[1].getContent()->setPressure(100.0);
    tNodes[0].getContent()->setTemperature(283.0);
    tNodes[1].getContent()->setTemperature(283.0);
    tArticle->mOpenTime = tTimeStep;
    tArticle->mCloseTime = tTimeStep;

    tArticle->mVoltage      = 12.0;
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

    tConfigData->mCloseVoltage = tCloseVoltage;
    tConfigData->mOpenVoltage  = tOpenVoltage;

    /// - Create handle fail to position criteria 
    tArticle->mMalfStuckFlag = false;
    tArticle->mMalfFailToFlag = true;
    tArticle->mMalfFailToValue = 0.4;
    tArticle->updateState(tTimeStep);
    
    /// @test     Test <1.0 position setting for handle failure
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, tArticle->mPosition, 0.0);


    std::cout << "... Pass";
}