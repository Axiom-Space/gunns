/************************** TRICK HEADER ***********************************************************

AUTHOR:
 ((Tristan Mansfield) (Feb 2023) (tristan@axiomspace.com))


LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "ax/TemplateSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

TemplateSpotterConfigData::TemplateSpotterConfigData(const std::string& name,
                                                    int memberVar1,
                                                    int memberVar2)
    :
    GunnsNetworkSpotterConfigData(name),
    mMemberVar1(memberVar1),
    mMemberVar2(memberVar2)
{
    // nothing to do
}

TemplateSpotterInputData::TemplateSpotterInputData(int inputBool)
    : GunnsNetworkSpotterInputData()
{
    // Can have data verification in constructors
    if (inputBool > 0) {
        mInputBool = true;
    } else {
        mInputBool = false;
    }
}

TemplateSpotter::TemplateSpotter()
    : GunnsNetworkSpotter()
{

}

void TemplateSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                       const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const TemplateSpotterConfigData* config = validateConfig(configData);
    const TemplateSpotterInputData*  input  = validateInput(inputData);

    /// - Initialize with validated config & input data.
    mMemberVar1 = config->mMemberVar1;
    mMemberVar2 = config->mMemberVar2;
    mInputBool = input->mInputBool;

    /// - Set the init flag.
    mInitFlag = true;
}

const TemplateSpotterConfigData* TemplateSpotter::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const TemplateSpotterConfigData* result = dynamic_cast<const TemplateSpotterConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    /// - Do your other data validation as appropriate.
    
    return result;
}

const TemplateSpotterInputData* TemplateSpotter::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const TemplateSpotterInputData* result = dynamic_cast<const TemplateSpotterInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }

    /// - Do your other data validation as appropriate.

    return result;
}

void TemplateSpotter::stepPreSolver(const double dt) {
    std::cout << "This happens in the PreSolver" << std::endl;
    exampleFunction1();
}

void TemplateSpotter::stepPostSolver(const double dt) {
    std::cout << "This happens in the PostSolver" << std::endl;
    exampleFunction2();

}

void TemplateSpotter::exampleFunction1() {
    std::cout << "You can have arbitrary member functions" <<
        "to incorporate complex logic or actuate GUNNS links" << std::endl;
}

void TemplateSpotter::exampleFunction2() {
    std::cout << "True" << std::endl;
}