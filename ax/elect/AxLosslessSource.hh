#ifndef AxLosslessSource_EXISTS
#define AxLosslessSource_EXISTS

/**
@file
@brief    GUNNS Lossless Source Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_SOURCE    GUNNS Lossless Source Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Lossless Source Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- (
   (AxLosslessSource.o)
  )

PROGRAMMERS:
- (
  (Tristan Mansfield)
  )

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class AxLosslessSourceConfigData : public GunnsBasicSourceConfigData
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class AxLosslessSourceInputData : public GunnsBasicSourceInputData
{

};

class AxLosslessSource : public GunnsBasicSource
{
  TS_MAKE_SIM_COMPATIBLE(AxLosslessSource);

  protected:
    /// @brief Builds the source vector terms of the links contribution to the network
    virtual void buildSource();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the links contribution to the network.  This sign
///          convention creates positive flow from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void AxLosslessSource::buildSource()
{
    mSourceVector[1] =  mFlux;
    mSourceVector[0] =  -1*(mPotentialVector[1]/mPotentialVector[0])*mFlux;
}

#endif