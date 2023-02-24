#ifndef TemplateSpotter_EXISTS
#define TemplateSpotter_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup 
/// @ingroup  
///
/// @copyright 
///            
///
/// @details  This is a Template Spotter Class, used to bare-bones demonstrate how to create a 
///           spotter for the AxSim.  A How-To Page can be found on Axiom's Network at:
///             https://vault.axiomspace.com/display/SEI/Making+a+Custom+Spotter
///           Else, copy and rename this file to get the boiler-plate needed for a custom spotter
///
/// @author ((Tristan Mansfield) (Feb 2023) (tristan@axiomspace.com))
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "core/GunnsNetworkSpotter.hh"
#include "core/GunnsBasicConductor.hh"
// include all GUNNS components you want your Spotter to interact with

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Template Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class TemplateSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:

        int mMemberVar1;
        int mMemberVar2;

        TemplateSpotterConfigData(const std::string& name,
                                    int MemberVar1,
                                    int MemberVar2);
        virtual ~TemplateSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Template Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class TemplateSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:

        bool mInputBool;

        TemplateSpotterInputData(int inputBool);
        virtual ~TemplateSpotterInputData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Derived Template Spotter Class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TemplateSpotter : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(TemplateSpotter);
    public:
        /// @brief  Enumeration of Template States
        enum TemplateStatus {
            DISABLED        = 0,
            DISCHARGING     = 1,
            CHARGING        = 2,
            TRIPPED         = 3,
        };
        
        TemplateSpotter();
        virtual     ~TemplateSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        virtual void stepPreSolver(const double dt);
        virtual void stepPostSolver(const double dt);

        void exampleFunction1();
        void exampleFunction2();
        
    protected:
        const TemplateSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const TemplateSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
    private:
        int mMemberVar1;
        int mMemberVar2;
        bool mInputBool;
};

///@}

#endif
