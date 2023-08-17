

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
GunnsBasicSourceInputData::GunnsBasicSourceInputData(const bool   malfBlockageFlag,
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
  : GunnsLosslessSource()
{
  
}

GunnsLosslessSource::~GunnsLosslessSource()
{
  GunnsBasicSource::~GunnsBasicSource();
}
