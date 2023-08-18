#ifndef GunnsLosslessSource_EXISTS
#define GunnsLosslessSource_EXISTS

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
   (GunnsLosslessSource.o)
  )

PROGRAMMERS:
- (
  (Tristan Mansfield)
  )

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLosslessSourceConfigData : public GunnsBasicSourceConfigData
{
  public:
    /// @brief Default constructs this Lossless Source configuration data.
    GunnsLosslessSourceConfigData(const std::string& name  = "",
                                  GunnsNodeList*     nodes = 0);

    /// @brief Default destructs this Lossless Source configuration data.
    virtual ~GunnsLosslessSourceConfigData();

    /// @brief Copy constructs this Lossless Source configuration data.
    GunnsLosslessSourceConfigData(const GunnsLosslessSourceConfigData& that);
  protected:

  private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSourceConfigData& operator =(const GunnsLosslessSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLosslessSourceInputData : public GunnsBasicSourceInputData
{
  public:
    /// @brief    Default constructs this Lossless Source input data.
    GunnsLosslessSourceInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0,
                                const double sourceFlux        = 0.0);

    /// @brief    Default destructs this Lossless Source input data.
    virtual ~GunnsLosslessSourceInputData();

    /// @brief    Copy constructs this Lossless Source input data.
    GunnsLosslessSourceInputData(const GunnsLosslessSourceInputData& that);
  protected:

  private:
    /// @details  Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSourceInputData& operator =(const GunnsLosslessSourceInputData&);

};

class GunnsLosslessSource : public GunnsBasicSource
{
  TS_MAKE_SIM_COMPATIBLE(GunnsLosslessSource);

  public:
    /// @brief Default Constructor
    GunnsLosslessSource();

    /// @brief Default Destructor
    virtual ~GunnsLosslessSource();

    /// @brief Initializes the link
    void initialize(const GunnsLosslessSourceConfigData& configData,
                    const GunnsLosslessSourceInputData&  inputData,
                    std::vector<GunnsBasicLink*>&     networkLinks,
                    const int                         port0,
                    const int                         port1);

  protected:

  private:
    /// @details Define the number of ports this link class has.  All objects of the same link
    ///          class always have the same number of ports.  We use an enum rather than a
    ///          static const int so that we can reuse the NPORTS name and allow each class to
    ///          define its own value.
    enum {NPORTS = 2};
    /// @brief Builds the source vector terms of the links contribution to the network
    virtual void buildSource();

    /// @brief Copy constructor unavailable since declared private and not implemented.
    GunnsLosslessSource(const GunnsLosslessSource& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSource& operator =(const GunnsLosslessSource& that);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the links contribution to the network.  This sign
///          convention creates positive flow from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsLosslessSource::buildSource()
{
    mSourceVector[1] =  mFlux;
    if (mPotentialVector[0] == 0.0) {
      mPotentialVector[0] = __DBL_EPSILON__;
      // mOverrideVector[0] = true;
    }
    mSourceVector[0] =  -1*(mPotentialVector[1]/mPotentialVector[0])*mFlux;
}

#endif