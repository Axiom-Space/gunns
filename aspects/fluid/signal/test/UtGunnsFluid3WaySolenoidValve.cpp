/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/fluid/conductor/GunnsFluid3WaySolenoidValve.o)
    )
***************************************************************************************************/
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsFluid3WaySolenoidValve.hh"

/// @details  Test identification number.
int UtGunnsFluid3WaySolenoidValve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluid3WaySolenoidValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluid3WaySolenoidValve::UtGunnsFluid3WaySolenoidValve()
    :
    tArticle(),
    tLinkName(),
    tConfigData(),
    tPositionPathB(false),
    tMaxConductivityA(0.0),
    tMaxConductivityB(0.0),
    tExpansionScaleFactorA(0.0),
    tExpansionScaleFactorB(0.0),
    tThermalLengthA(0.0),
    tThermalLengthB(0.0),
    tThermalDiameterA(0.0),
    tThermalDiameterB(0.0),
    tSurfaceRoughnessA(0.0),
    tSurfaceRoughnessB(0.0),
    tNonMixingBand(0.0),
    tLatching(false),
    tOpenVoltage(0.0),
    tOpenTime(0.0),
    tCloseVoltage(0.0),
    tCloseTime(0.0),
    tInputData(),
    tPosition(0.0),
    tWallTemperatureA(0.0),
    tWallTemperatureB(0.0),
    tFlux(0.0),
    tVoltage(0.0),
    tMalfStuckFlag(false),
    tMalfFailToFlag(false),
    tMalfFailToValue(0.0),
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
/// @details  This is the default destructor for the UtGunnsFluid3WaySolenoidValve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluid3WaySolenoidValve::~UtGunnsFluid3WaySolenoidValve()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::tearDown()
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
void UtGunnsFluid3WaySolenoidValve::setUp()
{
    /// - Define nominal configuration data
    tLinkName              = "Test Three Way Solenoid Valve";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tPositionPathB         = true;
    tMaxConductivityA      = 0.6;
    tMaxConductivityB      = 0.5;
    tExpansionScaleFactorA = 0.4;
    tExpansionScaleFactorB = 0.3;
    tThermalLengthA        = 1.0;
    tThermalLengthB        = 1.1;
    tThermalDiameterA      = 0.9;
    tThermalDiameterB      = 0.8;
    tSurfaceRoughnessA     = 1.0E-4;
    tSurfaceRoughnessB     = 2.0E-4;
    tNonMixingBand         = 0.2;
    tLatching             = false;
    tOpenVoltage          = 10.0;
    tOpenTime             = 0.075;
    tCloseVoltage         = 6.0;
    tCloseTime            = 0.025;
    tConfigData            = new GunnsFluid3WaySolenoidValveConfigData(tLinkName,
                                                                       &tNodeList,
                                                                       tPositionPathB,
                                                                       tMaxConductivityA,
                                                                       tMaxConductivityB,
                                                                       tExpansionScaleFactorA,
                                                                       tExpansionScaleFactorB,
                                                                       tThermalLengthA,
                                                                       tThermalLengthB,
                                                                       tThermalDiameterA,
                                                                       tThermalDiameterB,
                                                                       tSurfaceRoughnessA,
                                                                       tSurfaceRoughnessB,
                                                                       tNonMixingBand,
                                                                       tLatching,
                                                                       tOpenVoltage,
                                                                       tOpenTime,
                                                                       tCloseVoltage,
                                                                       tCloseTime);

    /// - Set up nominal input data.
    tPosition              = 1.0;
    tWallTemperatureA      = 300.0;
    tWallTemperatureB      = 290.0;
    tFlux                  = 0.0;
    tVoltage               = 0.0;
    tMalfStuckFlag         = false;
    tMalfFailToFlag        = false;
    tMalfFailToValue       = 0.0;
    tInputData             = new GunnsFluid3WaySolenoidValveInputData(tPosition,
                                                                      tWallTemperatureA,
                                                                      tWallTemperatureB,
                                                                      tFlux,
                                                                      tVoltage,
                                                                      tMalfStuckFlag,
                                                                      tMalfFailToFlag,
                                                                      tMalfFailToValue);

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
    tArticle               = new FriendlyGunnsFluid3WaySolenoidValve;
    tPathA                 = static_cast<FriendlyGunnsFluidValveFor3Way*>(&tArticle->mPathA);
    tPathB                 = static_cast<FriendlyGunnsFluidValveFor3Way*>(&tArticle->mPathB);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testConfig()
{
    UT_RESULT_FIRST;

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivityA      == tConfigData->mMaxConductivityA);
    CPPUNIT_ASSERT(tMaxConductivityB      == tConfigData->mMaxConductivityB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA == tConfigData->mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB == tConfigData->mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tThermalLengthA        == tConfigData->mThermalLengthA);
    CPPUNIT_ASSERT(tThermalLengthB        == tConfigData->mThermalLengthB);
    CPPUNIT_ASSERT(tThermalDiameterA      == tConfigData->mThermalDiameterA);
    CPPUNIT_ASSERT(tThermalDiameterB      == tConfigData->mThermalDiameterB);
    CPPUNIT_ASSERT(tSurfaceRoughnessA     == tConfigData->mSurfaceRoughnessA);
    CPPUNIT_ASSERT(tSurfaceRoughnessB     == tConfigData->mSurfaceRoughnessB);
    CPPUNIT_ASSERT(tNonMixingBand         == tConfigData->mNonMixingBand);
    CPPUNIT_ASSERT(tLatching              == tConfigData->mLatching);
    CPPUNIT_ASSERT(tOpenVoltage           == tConfigData->mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime              == tConfigData->mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage          == tConfigData->mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime             == tConfigData->mCloseTime);

    /// - Check default config construction
    GunnsFluid3WaySolenoidValveConfigData defaultConfig;
    CPPUNIT_ASSERT(""                     == defaultConfig.mName);
    CPPUNIT_ASSERT(0                      == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mMaxConductivityA);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mMaxConductivityB);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mThermalLengthA);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mThermalLengthB);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mThermalDiameterA);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mThermalDiameterB);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mSurfaceRoughnessA);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mSurfaceRoughnessB);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mNonMixingBand);
    CPPUNIT_ASSERT(false                  == defaultConfig.mLatching);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mOpenVoltage);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mOpenTime);
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mCloseVoltage);;
    CPPUNIT_ASSERT(0.0                    == defaultConfig.mCloseTime);

    /// - Check copy config construction
    GunnsFluid3WaySolenoidValveConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivityA      == copyConfig.mMaxConductivityA);
    CPPUNIT_ASSERT(tMaxConductivityB      == copyConfig.mMaxConductivityB);
    CPPUNIT_ASSERT(tExpansionScaleFactorA == copyConfig.mExpansionScaleFactorA);
    CPPUNIT_ASSERT(tExpansionScaleFactorB == copyConfig.mExpansionScaleFactorB);
    CPPUNIT_ASSERT(tThermalLengthA        == copyConfig.mThermalLengthA);
    CPPUNIT_ASSERT(tThermalLengthB        == copyConfig.mThermalLengthB);
    CPPUNIT_ASSERT(tThermalDiameterA      == copyConfig.mThermalDiameterA);
    CPPUNIT_ASSERT(tThermalDiameterB      == copyConfig.mThermalDiameterB);
    CPPUNIT_ASSERT(tSurfaceRoughnessA     == copyConfig.mSurfaceRoughnessA);
    CPPUNIT_ASSERT(tSurfaceRoughnessB     == copyConfig.mSurfaceRoughnessB);
    CPPUNIT_ASSERT(tNonMixingBand         == copyConfig.mNonMixingBand);
    CPPUNIT_ASSERT(tLatching              == copyConfig.mLatching);
    CPPUNIT_ASSERT(tOpenVoltage           == copyConfig.mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime              == copyConfig.mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage          == copyConfig.mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime             == copyConfig.mCloseTime);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testInput()
{
    UT_RESULT;

    /// - Check nominal input data construction
    CPPUNIT_ASSERT(tPosition              == tInputData->mPosition);
    CPPUNIT_ASSERT(tWallTemperatureA      == tInputData->mWallTemperatureA);
    CPPUNIT_ASSERT(tWallTemperatureB      == tInputData->mWallTemperatureB);
    CPPUNIT_ASSERT(tFlux                  == tInputData->mFlux);
    CPPUNIT_ASSERT(tVoltage               == tInputData->mVoltage);
    CPPUNIT_ASSERT(tMalfStuckFlag         == tInputData->mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag        == tInputData->mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue       == tInputData->mMalfFailToValue);

    /// - Check default input data construction
    GunnsFluid3WaySolenoidValveInputData defaultInput;
    CPPUNIT_ASSERT(0.0                    == defaultInput.mPosition);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mWallTemperatureA);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mWallTemperatureB);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mFlux);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mVoltage);
    CPPUNIT_ASSERT(false                  == defaultInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(false                  == defaultInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                    == defaultInput.mMalfFailToValue);

    /// - Check copy input data construction
    GunnsFluid3WaySolenoidValveInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tPosition              == copyInput.mPosition);
    CPPUNIT_ASSERT(tWallTemperatureA      == copyInput.mWallTemperatureA);
    CPPUNIT_ASSERT(tWallTemperatureB      == copyInput.mWallTemperatureB);
    CPPUNIT_ASSERT(tFlux                  == copyInput.mFlux);
    CPPUNIT_ASSERT(tVoltage               == copyInput.mVoltage);
    CPPUNIT_ASSERT(tMalfStuckFlag         == copyInput.mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag        == copyInput.mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue       == copyInput.mMalfFailToValue);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testDefaultConstruction()
{
    UT_RESULT;

    /// @test config data
    CPPUNIT_ASSERT(false                  == tArticle->mPositionPathB);
    CPPUNIT_ASSERT(0.0                    == tArticle->mNonMixingBand);
    CPPUNIT_ASSERT(0.0                    == tArticle->mPosition);
    CPPUNIT_ASSERT(false                  == tArticle->mLatching);
    CPPUNIT_ASSERT(0.0                    == tArticle->mOpenVoltage);
    CPPUNIT_ASSERT(0.0                    == tArticle->mOpenTime);
    CPPUNIT_ASSERT(0.0                    == tArticle->mCloseVoltage);
    CPPUNIT_ASSERT(0.0                    == tArticle->mCloseTime);

    /// @test init flag
    CPPUNIT_ASSERT(false                  == tArticle->mInitFlag);
    CPPUNIT_ASSERT(0.0                    == tArticle->mFlux);
    CPPUNIT_ASSERT(0.0                    == tArticle->mVoltage);
    CPPUNIT_ASSERT(false                  == tArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(false                  == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(0.0                    == tArticle->mMalfFailToValue);

    /// @test new/delete for code coverage
    GunnsFluid3WaySolenoidValve* article          =  new GunnsFluid3WaySolenoidValve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize the test article with nominal data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test base class data.
    CPPUNIT_ASSERT(tLinkName                 == tArticle->getName());

    /// @test config data.
    CPPUNIT_ASSERT(tPositionPathB            == tArticle->mPositionPathB);
    CPPUNIT_ASSERT(tNonMixingBand            == tArticle->mNonMixingBand);
    CPPUNIT_ASSERT(tPosition                 == tArticle->mPosition);
    CPPUNIT_ASSERT(tLatching                 == tArticle->mLatching);
    CPPUNIT_ASSERT(tOpenVoltage              == tArticle->mOpenVoltage);
    CPPUNIT_ASSERT(tOpenTime                 == tArticle->mOpenTime);
    CPPUNIT_ASSERT(tCloseVoltage             == tArticle->mCloseVoltage);
    CPPUNIT_ASSERT(tCloseTime                == tArticle->mCloseTime);

    /// @test input data
    CPPUNIT_ASSERT(tFlux                     == tArticle->mFlux);
    CPPUNIT_ASSERT(tVoltage                  == tArticle->mVoltage);
    CPPUNIT_ASSERT(tMalfStuckFlag            == tArticle->mMalfStuckFlag);
    CPPUNIT_ASSERT(tMalfFailToFlag           == tArticle->mMalfFailToFlag);
    CPPUNIT_ASSERT(tMalfFailToValue          == tArticle->mMalfFailToValue);

    /// @test embedded fluid valve's initialization.
    CPPUNIT_ASSERT(tMaxConductivityA         == tPathA->mMaxConductivity);
    CPPUNIT_ASSERT(tMaxConductivityB         == tPathB->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactorA    == tPathA->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tExpansionScaleFactorB    == tPathB->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tThermalDiameterA         == tPathA->mThermalDiameter);
    CPPUNIT_ASSERT(tThermalDiameterB         == tPathB->mThermalDiameter);
    const double areaA   = tThermalLengthA * UnitConversion::PI_UTIL * tThermalDiameterA;
    const double rOverdA = tSurfaceRoughnessA / tThermalDiameterA;
    const double areaB   = tThermalLengthB * UnitConversion::PI_UTIL * tThermalDiameterB;
    const double rOverdB = tSurfaceRoughnessB / tThermalDiameterB;
    CPPUNIT_ASSERT(areaA                     == tPathA->mThermalSurfaceArea);
    CPPUNIT_ASSERT(areaB                     == tPathB->mThermalSurfaceArea);
    CPPUNIT_ASSERT(rOverdA                   == tPathA->mThermalROverD);
    CPPUNIT_ASSERT(rOverdB                   == tPathB->mThermalROverD);
    CPPUNIT_ASSERT(0.0                       == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0                       == tPathB->mPosition);
    CPPUNIT_ASSERT(tWallTemperatureA         == tPathA->mWallTemperature);
    CPPUNIT_ASSERT(tWallTemperatureB         == tPathB->mWallTemperature);

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

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tPathA->mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tPathA->mSystemConductance,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tPathB->mEffectiveConductivity,DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,tPathB->mSystemConductance,DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testInitializationExceptions()
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

    /// @test for exception on embedded valve conductivity < 0
    tConfigData->mMaxConductivityA = -0.1;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test    Initialization exception on invalid config data: mOpenTime < 0.
    tConfigData->mOpenTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mOpenTime  = tOpenTime;

    /// @test    Initialization exception on invalid config data: mCloseTime < 0.
    tConfigData->mCloseTime  = -FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mCloseTime  = tCloseTime;

    /// @test    Initialization exception on invalid config data: mOpenPosition < mClosePosition.
    tConfigData->mCloseVoltage  = +DBL_EPSILON * 0.5;
    tConfigData->mOpenVoltage   = -DBL_EPSILON * 0.5;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tConfigData->mCloseVoltage  = tCloseVoltage;
    tConfigData->mOpenVoltage   = tOpenVoltage;

    /// @test    Initialization exception on invalid input data: mVoltage < 0.
    tInputData->mVoltage = 0.0 - FLT_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2),
                         TsInitializationException);
    tInputData->mVoltage = tVoltage;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testRestart()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mNonMixingBand = 0.0;
    tInputData->mPosition       = 0.5;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// - Step the article to update values in the flow path valves.
    tArticle->step(0.0);

    CPPUNIT_ASSERT(0.0 < tArticle->mPathA.getEffectiveConductivity());
    CPPUNIT_ASSERT(0.0 < tArticle->mPathB.getEffectiveConductivity());

    /// @test  Restart method clears the embedded flow path values.
    for (unsigned int i=0; i<tLinks.size(); ++i) {
        tLinks[i]->restart();
    }

    CPPUNIT_ASSERT(0.0 == tArticle->mPathA.getEffectiveConductivity());
    CPPUNIT_ASSERT(0.0 == tArticle->mPathB.getEffectiveConductivity());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testStep()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mNonMixingBand = 0.0;
    tInputData->mVoltage        = 0.0;
    tConfigData->mPositionPathB = false;
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

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for the step method with non-mixing configurations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testStepNonMixing()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data
    tConfigData->mPositionPathB = false;
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);

    /// @test  Non-mixing, position relative to flow path B positions:
    tArticle->mVoltage = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0 == tPathB->mPosition);

    tArticle->mVoltage = 8.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mPositionPathB = false;
    tArticle->mVoltage = 12.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    /// @test  Handles non-mixing range = 1:
    tArticle->mNonMixingBand = 1.0;
    tArticle->mVoltage       = 12.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(1.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    tArticle->mVoltage = 0.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(1.0 == tPathB->mPosition);

    tArticle->mVoltage = 8.0;
    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tPathA->mPosition);
    CPPUNIT_ASSERT(0.0 == tPathB->mPosition);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test dynamic port mapping.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testDynamicPortMapping()
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
void UtGunnsFluid3WaySolenoidValve::testComputeFlow()
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
    tArticle->mVoltage           =  8.0;
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
void UtGunnsFluid3WaySolenoidValve::testAccessMethods()
{
    UT_RESULT;

    CPPUNIT_ASSERT(tPathA       == tArticle->getPathA());
    CPPUNIT_ASSERT(tPathB       == tArticle->getPathB());
    CPPUNIT_ASSERT(tLatching    == tArticle->getLatching());
    CPPUNIT_ASSERT(tFlux        == tArticle->getFlux());
    CPPUNIT_ASSERT(tVoltage     == tArticle->getVoltage());

    UT_PASS_LAST;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Fluid Three Way Solenoid Valve link model modifiers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluid3WaySolenoidValve::testModifiers()
{
    UT_RESULT;

    /// - Initialize default test article with nominal initialization data.
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1, tPort2);
    
    /// @test    The latching state is set by the access method.
    tArticle->setLatching(true);
    CPPUNIT_ASSERT(true  == tArticle->mLatching);

    /// @test    The flux is set by the access method.
    tArticle->setFlux(1.0);
    CPPUNIT_ASSERT(1.0  == tArticle->mFlux);

    /// @test    The voltage is set by the access method.
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

    UT_PASS;
}