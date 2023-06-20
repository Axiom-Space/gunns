/************************** TRICK HEADER ***********************************************************
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluidSolenoidValve.hh"

/// @details  Test identification number.
int UtGunnsFluidSolenoidValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Solenoid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSolenoidValve::UtGunnsFluidSolenoidValve()
    :
    CppUnit::TestFixture(),
    mTypes(),
    mFractions(),
    mFluidProperties(0),
    mFluidConfig(0),
    mFluidInput0(0),
    mFluidInput1(0),
    mLinks(),
    mName(""),
    mNodes(),
    mNodeList(),
    mPort0(0),
    mPort1(0),
    mMaxConductivity(0.0),
    mExpansionScaleFactor(0.0),
    mThermalLength(0.0),
    mThermalDiameter(0.0),
    mSurfaceRoughness(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mLatching(false),
    mOpenVoltage(0.0),
    mOpenTime(0.0),
    mCloseVoltage(0.0),
    mCloseTime(0.0),
    mConfigData(0),
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mPosition(0.0),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mWallTemperature(0.0),
    mFlux(0.0),
    mVoltage(0.0),
    mMalfStuckFlag(false),
    mMalfFailToFlag(false),
    mMalfFailToValue(0.0),
    mInputData(0),
    mArticle(0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0),
    mWallHeatFlux(0.0),
    mFlowRate(0),
    mTimeStep(0.0),
    mTolerance(0.0)
 {
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Solenoid Valve link model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidSolenoidValve::~UtGunnsFluidSolenoidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::setUp()
{
    /// - Define the nominal port fluids.
    mFluidProperties      = new DefinedFluidProperties();
    mTypes[0]             = FluidProperties::GUNNS_N2;
    mTypes[1]             = FluidProperties::GUNNS_O2;
    mFractions[0]         = 0.5;
    mFractions[1]         = 0.5;
    mFluidConfig          = new PolyFluidConfigData(mFluidProperties, mTypes, N_FLUIDS);
    mFluidInput0          = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);
    mFluidInput1          = new PolyFluidInputData(283.0,
                                                   111.0,
                                                     0.0,
                                                     0.0,
                                                     mFractions);

    /// - Initialize the nodes.
    mNodes[0].initialize("UtNode1", mFluidConfig);
    mNodes[1].initialize("UtNode2", mFluidConfig);
    mNodes[0].getContent()->initialize(*mFluidConfig, *mFluidInput0);
    mNodes[1].getContent()->initialize(*mFluidConfig, *mFluidInput1);

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes list.
    mNodeList.mNodes      = mNodes;
    mNodeList.mNumNodes   = N_NODES;

    /// - Define the nominal configuration data.
    mName                 = "nominal";
    mMaxConductivity      = 20.0;
    mExpansionScaleFactor =  0.5;
    mThermalLength        = 1.0;
    mThermalDiameter      = 0.01;
    mSurfaceRoughness     = 2.1336E-6;
    mThermalSurfaceArea   =  mThermalLength * mThermalDiameter * UnitConversion::PI_UTIL;
    mThermalROverD        =  mSurfaceRoughness / mThermalDiameter;
    mLatching             = false;
    mOpenVoltage          = 10.0;
    mOpenTime             = 0.075;
    mCloseVoltage         = 6.0;
    mCloseTime            = 0.025;
    mConfigData           = new GunnsFluidSolenoidValveConfigData(mName,
                                                                  &mNodeList,
                                                                  mMaxConductivity,
                                                                  mExpansionScaleFactor,
                                                                  mThermalLength,
                                                                  mThermalDiameter,
                                                                  mSurfaceRoughness,
                                                                  mLatching,
                                                                  mOpenVoltage,
                                                                  mOpenTime,
                                                                  mCloseVoltage,
                                                                  mCloseTime);

    /// - Define the nominal input data.
    mMalfBlockageFlag     = false;
    mMalfBlockageValue    = 0.3;
    mPosition             = 1.0;
    mMalfLeakThruFlag     = false;
    mMalfLeakThruValue    = 0.0;
    mPreviousLeakRate     = 0.0;
    mLeakConductivity     = 0.0;
    mWallTemperature      = 300.0;
    mFlux                 = 0.0;
    mVoltage              = 0.0;
    mMalfStuckFlag        = false;
    mMalfFailToFlag       = false;
    mMalfFailToValue      = 0.0;
    mInputData            = new GunnsFluidSolenoidValveInputData(mMalfBlockageFlag,
                                                                 mMalfBlockageValue,
                                                                 mPosition,
                                                                 mMalfLeakThruFlag,
                                                                 mMalfLeakThruValue,
                                                                 mWallTemperature,
                                                                 mFlux,
                                                                 mVoltage,
                                                                 mMalfStuckFlag,
                                                                 mMalfFailToFlag,
                                                                 mMalfFailToValue);

    /// - Define the nominal port mapping.
    mPort0                = 0;
    mPort1                = 1;

    /// - Default construct the nominal test article.
    mArticle              = new FriendlyGunnsFluidSolenoidValve;

    /// - Define the nominal flow rate, time step and comparison tolerance.
    mFlowRate             = 0.3;
    mTimeStep             = 0.01;
    mTolerance            = 1.0e-08;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mArticle;
    delete mInputData;
    delete mConfigData;
    delete mFluidInput1;
    delete mFluidInput0;
    delete mFluidConfig;
    delete mFluidProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model construction of configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testConfigAndInput()
{
    UT_RESULT_FIRST;

    /// @test    Configuration data nominal construction.
    CPPUNIT_ASSERT(mName                                          == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes                                         == mConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,                   mConfigData->mMaxConductivity,       0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor,              mConfigData->mExpansionScaleFactor,  0.0);
    CPPUNIT_ASSERT(mLatching                                      == mConfigData->mLatching);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenVoltage,                       mConfigData->mOpenVoltage,           0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenTime,                          mConfigData->mOpenTime,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCloseVoltage,                      mConfigData->mCloseVoltage,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCloseTime,                         mConfigData->mCloseTime,             0.0);

    /// @test    Input data nominal construction.
    CPPUNIT_ASSERT(mMalfBlockageFlag                              == mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,                 mInputData->mMalfBlockageValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,                          mInputData->mPosition,               0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag                              == mInputData->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,                 mInputData->mMalfLeakThruValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mFlux,                              mInputData->mFlux,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mVoltage,                           mInputData->mVoltage,                0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                                 == mInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                                == mInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,                   mInputData->mMalfFailToValue,        0.0);

    /// @test    Configuration data default construction.
    GunnsFluidSolenoidValveConfigData defaultConfig;
    CPPUNIT_ASSERT(0                                              == defaultConfig.mName.size());
    CPPUNIT_ASSERT(0                                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(                                                 !defaultConfig.mLatching);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mOpenVoltage,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mOpenTime,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mCloseVoltage,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultConfig.mCloseTime,            0.0);

    /// @test    Input data default construction.
    GunnsFluidSolenoidValveInputData   defaultInput;
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfBlockageValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mPosition,              0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfLeakThruValue,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mFlux,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mVoltage,               0.0);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(                                                 !defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                                defaultInput.mMalfFailToValue,       0.0);

    /// @test    Configuration data copy construction.
    GunnsFluidSolenoidValveConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mConfigData->mName                             == copyConfig.mName);
    CPPUNIT_ASSERT(mConfigData->mNodeList->mNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mMaxConductivity,      copyConfig.mMaxConductivity,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mExpansionScaleFactor, copyConfig.mExpansionScaleFactor,    0.0);
    CPPUNIT_ASSERT(mConfigData->mLatching                         == copyConfig.mLatching);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mOpenVoltage,          copyConfig.mOpenVoltage,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mOpenTime,             copyConfig.mOpenTime,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mCloseVoltage,         copyConfig.mCloseVoltage,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mConfigData->mCloseTime,            copyConfig.mCloseTime,               0.0);

    /// @test    Input data copy construction.
    GunnsFluidSolenoidValveInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag                  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfBlockageValue,     copyInput.mMalfBlockageValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mPosition,              copyInput.mPosition,                 0.0);
    CPPUNIT_ASSERT(mInputData->mMalfLeakThruFlag                  == copyInput.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfLeakThruValue,     copyInput.mMalfLeakThruValue,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mVoltage,               copyInput.mVoltage,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mFlux,                  copyInput.mFlux,                     0.0);
    CPPUNIT_ASSERT(mInputData->mMalfStuckFlag                     == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(mInputData->mMalfFailToFlag                    == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputData->mMalfFailToValue,       copyInput.mMalfFailToValue,          0.0);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test    Default construction configuration data.
    CPPUNIT_ASSERT(             "" == mArticle->mName);
    CPPUNIT_ASSERT(              0 == mArticle->mNodes);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalDiameter,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalSurfaceArea,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mThermalROverD,        0.0);
    CPPUNIT_ASSERT(                  !mArticle->mLatching);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOpenVoltage,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mOpenTime,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mCloseVoltage,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mCloseTime,            0.0);

    /// @test    Default construction input data.
    CPPUNIT_ASSERT(                  !mArticle->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfBlockageValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPosition,             0.0);
    CPPUNIT_ASSERT(                  !mArticle->mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfLeakThruValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mWallHeatFlux,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mFlux,                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mVoltage,              0.0);
    CPPUNIT_ASSERT(                  !mArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(                  !mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mMalfFailToValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPreviousLeakRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mLeakConductivity,     0.0);

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(                  !mArticle->mInitFlag);

    /// @test    New/delete for code coverage.
    GunnsFluidSolenoidValve* article = new GunnsFluidSolenoidValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    FriendlyGunnsFluidSolenoidValve article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Nominal configuration data.
    CPPUNIT_ASSERT(mName                             == article.mName);
    CPPUNIT_ASSERT(&mNodes[0]                        == article.mNodes[0]);
    CPPUNIT_ASSERT(&mNodes[1]                        == article.mNodes[1]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity,      article.mMaxConductivity,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mExpansionScaleFactor, article.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT(mLatching                         == article.mLatching);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenVoltage,          article.mOpenVoltage,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOpenTime,             article.mOpenTime,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCloseVoltage,         article.mCloseVoltage,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mCloseTime,            article.mCloseTime,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalSurfaceArea,   article.mThermalSurfaceArea,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalROverD,        article.mThermalROverD,        DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalDiameter,      article.mThermalDiameter,      DBL_EPSILON);

    /// @test    Nominal input data.
    CPPUNIT_ASSERT(mMalfBlockageFlag                 == article.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfBlockageValue,    article.mMalfBlockageValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPosition,             article.mPosition,             0.0);
    CPPUNIT_ASSERT(mMalfLeakThruFlag ==                 article.mMalfLeakThruFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfLeakThruValue,    article.mMalfLeakThruValue,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mWallTemperature,      article.mWallTemperature,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   article.mWallHeatFlux,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   article.mVoltage,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,                   article.mFlux,                 0.0);
    CPPUNIT_ASSERT(mMalfStuckFlag                    == article.mMalfStuckFlag);
    CPPUNIT_ASSERT(mMalfFailToFlag                   == article.mMalfFailToFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMalfFailToValue,      article.mMalfFailToValue,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mPreviousLeakRate,     article.mPreviousLeakRate,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mLeakConductivity,     article.mLeakConductivity,     0.0);


    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(                                     article.mInitFlag);

    ///  -  Ensure Restart model functionality
    article.restartModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,article.mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testAccessors()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    {
        /// @test    Get latching.
        const bool expected = true;
        mArticle->mLatching = expected;
        const bool returned = mArticle->getLatching();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    }
    {
        /// @test    Get voltage.
        const double expected = 1.00;
        mArticle->mFlux = expected;
        const double returned = mArticle->getFlux();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    }
    {
        /// @test    Get voltage.
        const double expected = 1.00;
        mArticle->mVoltage = expected;
        const double returned = mArticle->getVoltage();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    
    /// @test    The latching state is set by the access method.
    mArticle->setLatching(true);
    CPPUNIT_ASSERT(true  == mArticle->mLatching);

    /// @test    The flux is set by the access method.
    mArticle->setFlux(1.0);
    CPPUNIT_ASSERT(1.0  == mArticle->mFlux);

    /// @test    The voltage is set by the access method.
    mArticle->setVoltage(1.0);
    CPPUNIT_ASSERT(1.0  == mArticle->mVoltage);

    /// @test    The stuck malf is set by the access method.
    mArticle->setMalfStuck(true);
    CPPUNIT_ASSERT(true  == mArticle->mMalfStuckFlag);

    /// @test    The stuck malf is reset by the access method.
    mArticle->setMalfStuck();
    CPPUNIT_ASSERT(false == mArticle->mMalfStuckFlag);

    /// @test    The fail to position malf is set by the access method.
    mArticle->setMalfFailTo(true, 1.0);
    CPPUNIT_ASSERT(true  == mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(1.0   == mArticle->mMalfFailToValue);

    /// @test    The fail to position malf is reset by the access method.
    mArticle->setMalfFailTo();
    CPPUNIT_ASSERT(false == mArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0   == mArticle->mMalfFailToValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (nominal).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testUpdateStateNominal()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Effectively disable rate limiting.
    mArticle->mOpenTime = mTimeStep;
    mArticle->mCloseTime = mTimeStep;

    /// @test    With valve closed and sufficient positive delta voltage so it full opens.
    mArticle->mPosition = 0.0;
    mArticle->mVoltage = mOpenVoltage;
    double expected          = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient positive delta voltage so it partially opens.
    mArticle->mVoltage = mCloseVoltage + 0.75 * (mOpenVoltage - mCloseVoltage);
    expected                 = 0.75;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient negative delta voltage so it partially closes.
    mArticle->mVoltage = mCloseVoltage + 0.25 * (mOpenVoltage - mCloseVoltage);
    expected                 = 0.25;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With sufficient negative delta voltage so it closes.
    mArticle->mVoltage = mCloseVoltage;
    expected                 = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient negative delta voltage it partially closes.
    mArticle->mVoltage = mCloseVoltage + 0.25 * (mOpenVoltage - mCloseVoltage);
    expected                 = 0.25;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With insufficient positive delta voltage so it partially opens.
    mArticle->mVoltage = mCloseVoltage + 0.75 * (mOpenVoltage - mCloseVoltage);
    expected                 = 0.75;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    /// @test    With sufficient positive delta voltage so it fully opens.
    mArticle->mVoltage = mOpenVoltage;
    expected                 = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mMaxConductivity * expected,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (rate limiting).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testUpdateStateRateLimited()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test    Rate limiting with closed valve and delta v larger than rate limit * dt.
    mArticle->mOpenTime     = 0.075;
    mArticle->mPosition      = 0.0;
    mArticle->mVoltage = mOpenVoltage;
    double expected = mTimeStep / mOpenTime;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    /// @test    Rate limiting with open valve and delta v larger than rate limit * dt.
    mArticle->mCloseTime     = 0.025;
    mArticle->mPosition      = 1.0;
    mArticle->mVoltage = mCloseVoltage;
    expected = 1.0 - mTimeStep / mCloseTime;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testUpdateStateNoRange()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Effectively disable rate limiting and set open and closed voltage to zero.
    mArticle->mOpenTime = mTimeStep;
    mArticle->mCloseTime = mTimeStep;
    mArticle->mOpenVoltage  = 0.0;
    mArticle->mCloseVoltage = 0.0;

    /// @test    With valve closed and a very small positive delta voltage so it fully opens.
    mArticle->mPosition = 0.0;
    mArticle->mVoltage  = +DBL_EPSILON;
    double expected     = 1.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    /// @test    With valve open and a very small negative delta voltage so it closes.
    mArticle->mVoltage = -DBL_EPSILON;
    expected = 0.0;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, mArticle->mPosition, mTolerance);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model update state (open and close voltages
///           both zero).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testUpdateStateMalfunction()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    mConfigData->mCloseVoltage = 12.0;
    mConfigData->mOpenVoltage  = 16.0;
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);
    mNodes[0].getContent()->setPressure(112.0);
    mNodes[1].getContent()->setPressure(100.0);
    mNodes[0].getContent()->setTemperature(283.0);
    mNodes[1].getContent()->setTemperature(283.0);
    mArticle->mOpenTime = mTimeStep;
    mArticle->mCloseTime = mTimeStep;

    mArticle->mVoltage      = 12.0;
    const double leakRate         = 1.0;
    const double leakConductivity = GunnsFluidUtils::predictConductivity(leakRate,
                                                                         mArticle->mMinLinearizationPotential,
                                                                         mNodes[0].getOutflow(),
                                                                         mNodes[1].getOutflow());

    /// @test     Update state with initial leak rate malfunction insertion.
    mArticle->mMalfLeakThruFlag  = true;
    mArticle->mMalfLeakThruValue = leakRate;
    mArticle->updateState(mTimeStep);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mMalfLeakThruValue,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakRate,
                                 mArticle->mPreviousLeakRate,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mLeakConductivity,
                                 mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(leakConductivity,
                                 mArticle->mEffectiveConductivity,
                                 mTolerance);

    mConfigData->mCloseVoltage = mCloseVoltage;
    mConfigData->mOpenVoltage  = mOpenVoltage;

    /// - Create handle fail to position criteria 
    mArticle->mMalfStuckFlag = false;
    mArticle->mMalfFailToFlag = true;
    mArticle->mMalfFailToValue = 0.4;
    mArticle->updateState(mTimeStep);
    
    /// @test     Test <1.0 position setting for handle failure
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, mArticle->mPosition, 0.0);


    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Solenoid Valve link model initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidSolenoidValve::testInitializationExceptions()
{
    UT_RESULT;

    /// - Default construct a test article.
    GunnsFluidSolenoidValve article;

    /// @test    Initialization exception on invalid config data: no name.
    mConfigData->mName = "";
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mName = mName;

    /// @test    Initialization exception on invalid config data: max conductivity < 0.
    mConfigData->mMaxConductivity = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mMaxConductivity = mMaxConductivity;

    /// @test    Initialization exception on invalid config data: expansion scale factor < 0.
    mConfigData->mExpansionScaleFactor = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mExpansionScaleFactor = mExpansionScaleFactor;

    /// @test    Initialization exception on invalid config data: mOpenTime < 0.
    mConfigData->mOpenTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mOpenTime  = mOpenTime;

    /// @test    Initialization exception on invalid config data: mCloseTime < 0.
    mConfigData->mCloseTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mCloseTime  = mCloseTime;

    /// @test    Initialization exception on invalid config data: mOpenPosition < mClosePosition.
    mConfigData->mCloseVoltage  = +DBL_EPSILON * 0.5;
    mConfigData->mOpenVoltage   = -DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mConfigData->mCloseVoltage  = mCloseVoltage;
    mConfigData->mOpenVoltage   = mOpenVoltage;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue < 0.
    mInputData->mMalfBlockageValue = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: mMalfBlockageValue > 1.
    mInputData->mMalfBlockageValue = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mMalfBlockageValue = mMalfBlockageValue;

    /// @test    Initialization exception on invalid input data: wall temperature < 0.
    mInputData->mWallTemperature = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mWallTemperature = mWallTemperature;

    /// @test    Initialization exception on invalid input data: mPosition < 0.
    mInputData->mPosition = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mPosition > 1.
    mInputData->mPosition = 1.0 + FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mPosition = mPosition;

    /// @test    Initialization exception on invalid input data: mVoltage < 0.
    mInputData->mVoltage = 0.0 - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1),
                         TsInitializationException);
    mInputData->mVoltage = mVoltage;

    UT_PASS_LAST;
}
