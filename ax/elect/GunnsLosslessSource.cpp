

#include "GunnsLosslessSource.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Constructs the Lossless Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::GunnsLosslessSourceConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes)
    : GunnsBasicSourceConfigData(name, nodes)
{
    // nothing to do
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy Constructs the Lossless Source Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::GunnsLosslessSourceConfigData(const GunnsLosslessSourceConfigData& that)
    : GunnsBasicSourceConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Lossless Source Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceConfigData::~GunnsLosslessSourceConfigData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] sourceFlux        (--) Initial demanded flux of the link
///
/// @details  Default constructs this Basic Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::GunnsLosslessSourceInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const double sourceFlux)
    : GunnsBasicSourceInputData(malfBlockageFlag, malfBlockageValue, sourceFlux)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Lossless Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::GunnsLosslessSourceInputData(const GunnsLosslessSourceInputData& that)
    : GunnsBasicSourceInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Lossless Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLosslessSourceInputData::~GunnsLosslessSourceInputData()
{
    // nothing to do
}



GunnsLosslessSource::GunnsLosslessSource()
  : GunnsBasicSource()
{
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Link Config Data
/// @param[in]     inputData    (--) Link Input Data
/// @param[in,out] networkLinks (--) Reference to the Solver Links
/// @param[in]     port0        (--) Port 0 Node Mapping
/// @param[in]     port1        (--) Port 1 Node Mapping
///
/// @throws   TsInitializationException
///
/// @details This initializes the link and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::initialize(const GunnsLosslessSourceConfigData& configData,
                                  const GunnsLosslessSourceInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
  GunnsBasicSource::initialize(configData, inputData, networkLinks, port0, port1);
}

/// @brief Computes mPower for the aspect-specific implementation
void GunnsLosslessSource::computePower() {
    // mPotentialVector should always be (+)
    double powerOut = mPotentialVector[1]*mSourceVector[1]; // mSourceVector[1] should always be (+)
    double powerIn  = mPotentialVector[0]*mSourceVector[0]; // mSourceVector[0] should always be (-)
    mPower = powerIn + powerOut;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Makes calls to accumulate flux in the input & output terms of the receiving and
///           sending nodes, respectively.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::transportFlux()
{
    /**
     * mNodes[0].flux = mNodes[1].flux*mNodes[1].potential/mNodes[0].potential
    */
    double fluxOnInputNode = mSourceFlux*mNodes[1]->getPotential()/mNodes[0]->getPotential();
    if (mSourceFlux > 0.0) {
        mNodes[0]->collectOutflux(fluxOnInputNode);
        mNodes[1]->collectInflux (mSourceFlux);

    } else if (mSourceFlux < 0.0) {
        mNodes[1]->collectOutflux(-mSourceFlux);
        mNodes[0]->collectInflux (-1*fluxOnInputNode);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Computes the flows across the link
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLosslessSource::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();
    computePower();
    transportFlux();
}

GunnsLosslessSource::~GunnsLosslessSource()
{
 // nothing to do
}
