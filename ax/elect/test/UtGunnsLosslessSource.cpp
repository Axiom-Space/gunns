/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (ax/elect/GunnsLosslessSource.o)
    )
***************************************************************************************************/
#include "UtGunnsLosslessSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsLosslessSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLosslessSource::UtGunnsLosslessSource()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mLinkName(),
    mInitialDemand(),
    mNodes(),
    mNodeList(),
    mLinks(),
    mPort0(),
    mPort1(),
    mTimeStep(),
    mTolerance()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsLosslessSource class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsLosslessSource::~UtGunnsLosslessSource()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::tearDown()
{
    /// - Deletes for news in setUp
    delete mArticle;
    delete mInputData;
    delete mConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::setUp()
{
    mLinkName             = "Test Lossless Source";
    mNodeList.mNumNodes   = 2;
    mNodeList.mNodes      = mNodes;
    mInitialDemand        = 0.5;
    mPort0                = 0;
    mPort1                = 1;

    /// - Define nominal configuration data
    mConfigData = new GunnsLosslessSourceConfigData(mLinkName, &mNodeList);

    /// - Define nominal input data
    mInputData = new GunnsLosslessSourceInputData(true, 0.5, mInitialDemand);

    /// - Create the test article
    mArticle = new FriendlyGunnsLosslessSource;

    /// - Declare the nominal test data
    mTolerance = 1.0e-08;
    mTimeStep = 0.1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsLosslessSource ..... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mLinkName == mConfigData->mName);
    CPPUNIT_ASSERT(mNodes    == mConfigData->mNodeList->mNodes);

    /// - Check default config construction
    GunnsLosslessSourceConfigData defaultConfig;
    CPPUNIT_ASSERT(""        == defaultConfig.mName);
    CPPUNIT_ASSERT(0         == defaultConfig.mNodeList);

    /// - Check copy config construction
    GunnsLosslessSourceConfigData copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mLinkName == copyConfig.mName);
    CPPUNIT_ASSERT(mNodes    == copyConfig.mNodeList->mNodes);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testInput()
{
    std::cout << "\n UtGunnsLosslessSource ..... 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.5            == mInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand == mInputData->mSourceFlux);

    /// - Check default input construction
    GunnsLosslessSourceInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0 == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0 == defaultInput.mSourceFlux);

    /// - Check copy input construction
    GunnsLosslessSourceInputData copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(mInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(mInitialDemand                 == copyInput.mSourceFlux);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testDefaultConstruction()
{
    std::cout << "\n UtGunnsLosslessSource ..... 03: testDefaultConstruction ...............";

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->mSourceFlux,
                                 mTolerance);

    /// @test init flag
    CPPUNIT_ASSERT(!mArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsLosslessSource* article = new GunnsLosslessSource();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testNominalInitialization()
{
    std::cout << "\n UtGunnsLosslessSource ..... 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsLosslessSource article;
    article.initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// @test config data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5,            article.mMalfBlockageValue, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInitialDemand, article.mSourceFlux,        0.0);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testStep()
{
    std::cout << "\n UtGunnsLosslessSource ..... 05: testStep ..............................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  0.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =   0.0;

    mArticle->step(mTimeStep);

    /// - Not using the malfunction flag now, may change this in the future
    // /// - during step the source Vector will be equal to the source flux times the blockage malf.
    // CPPUNIT_ASSERT_DOUBLES_EQUAL(mArticle->mSourceFlux * 0.5,
    //                              mArticle->mSourceVector[1], DBL_EPSILON);

    /** This is the change from GunnsBasicSource
     * Potential*flux = "Power"
     * For conservation of power we expect Pot_in*flux_in = Pot_out*flux_out
     * We set flux_out, so solving for flux_in:
     * flux_in = (Pot_out/Pot_in)*flux_out
     */
    double expected_influx = -1*mNodes[1].getPotential()/mNodes[0].getPotential()*mArticle->mSourceFlux;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx,
                                 mArticle->mSourceVector[0],            DBL_EPSILON);


    mNodes[0].resetFlows();
    mNodes[1].resetFlows();

    /// - Initialize the nodes potentials at something more interesting
    mNodes[0].setPotential(600.0);
    mNodes[1].setPotential(135.0);
    mArticle->mPotentialVector[0] = 600.0;
    mArticle->mPotentialVector[1] = 135.0;

    mArticle->step(mTimeStep);

    expected_influx = -1*mNodes[1].getPotential()/mNodes[0].getPotential()*mArticle->mSourceFlux;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx,
                                 mArticle->mSourceVector[0],            DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Compute Flows.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testComputeFlows()
{
    std::cout << "\n UtGunnsLosslessSource ..... 06: testComputeFlows ......................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    // HACK_ What should the link do if there's 0V on the output? Seems like there should be 0 current too?


    {
    /// - Test 1: 100V input, 0V output
    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  0.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =   0.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
            mArticle->mPotentialDrop, DBL_EPSILON);

    /// - The power across this link should always be near 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPower, DBL_EPSILON);

    /// - Check flux is transported to/from the nodes
    double expected_influx = -1*mNodes[1].getPotential()/mNodes[0].getPotential()*mInitialDemand;
    double expected_outflux = mInitialDemand;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_outflux, mNodes[1].getInflux(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx, mNodes[0].getOutflux(), 0.0);
    }

    mNodes[0].resetFlows();
    mNodes[1].resetFlows();


    {
    /// - Test 2: 100V input, 13V output
    /// - Initialize the nodes potentials
    mNodes[0].setPotential(100.0);
    mNodes[1].setPotential(  13.0);
    mArticle->mPotentialVector[0] = 100.0;
    mArticle->mPotentialVector[1] =  13.0;
    mArticle->mMalfBlockageFlag = false;

    mArticle->step(mTimeStep);
    mArticle->computeFlows(mTimeStep);

    /// - Check potential drop and power across the link is updated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mNodes[0].getPotential() - mNodes[1].getPotential(),
            mArticle->mPotentialDrop, DBL_EPSILON);

    /// - The power across this link should always be near 0.0
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mPower, DBL_EPSILON);

    /**
     * 0.5A*13V = 6.5W
     * 6.5W/100V = 0.0065A
     * mNodes[0].getOutflux() should give us 0.0065
    */

    /// - Check flux is transported to/from the nodes
    double expected_influx = -1*mNodes[1].getPotential()/mNodes[0].getPotential()*mInitialDemand;
    double expected_outflux = mInitialDemand;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_outflux, mNodes[1].getInflux(),  0.0); // Exact Assignment
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected_influx, mNodes[0].getOutflux(), DBL_EPSILON); // Calculated Value
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests setter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testSetters()
{
    std::cout << "\n UtGunnsLosslessSource ..... 07: testSetters ...........................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Set the source flux demand using the setter method and verify.
    mArticle->setFluxDemand(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, mArticle->mSourceFlux, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests access methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsLosslessSource::testAccessMethods()
{
    std::cout << "\n UtGunnsLosslessSource ..... 08: testAccessMethods .....................";

    /// - Initialize default test article with nominal initialization data
    mArticle->initialize(*mConfigData, *mInputData, mLinks, mPort0, mPort1);

    /// - Get the source flux demand using the getter method and verify.
    mArticle->setFluxDemand(5.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, mArticle->getFluxDemand(), 0.0);

    std::cout << "... Pass";
}
