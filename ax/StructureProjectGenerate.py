#!/usr/bin/python
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
import os,sys

## Set the sys.path to the generation utilities.
gunnsHome = os.environ["GUNNS_HOME"]
sys.path.append(os.path.join(os.path.dirname(__file__), gunnsHome + '/aspects/thermal/network/bin/ThermAspectGenerate'))

## Include all necessary classes.
from ThermSupport import ThermError
from ThermAspectConfiguring import ThermAspectConfig
from ThermAspectBuilding import ThermAspectBuilder
from IndivNetworkConfiguring import IndivNetworkConfig
from IndivNetworkBuilding import IndivNetworkBuilder

## Name of this file
thisScript = os.path.basename(__file__)

## All networks that are eligible to be built.
tNetworks = ['simplifiedStructureTherm', 'smStructureTherm', 'h1StructureTherm']

## Upper-level paths.
tStructureHome = os.environ["SIM_HOME"] + "/models/gunns/" + "Structure/"
tAspectRegistry = tStructureHome + "aspect_registry/"
tPtcsModules = tStructureHome + "therm/"

## File name specifics
tRegisPath = tAspectRegistry + "ThermRegistry_%s.xml"
tTdPath = tStructureHome + "therm/TdNetworkConfig_%s.xml"
tNodePath = tPtcsModules + "ThermNodes_%s.xml"
tCondPath = tPtcsModules + "ThermLinksCond_%s.xml"
tRadPath = tPtcsModules + "ThermLinksRad_%s.xml"
tHtrPath = tAspectRegistry + "HtrRegistry_%s.xml"
tPanPath = tAspectRegistry + "PanelRegistry_%s.xml"
tEtcPath = tPtcsModules + "ThermLinksEtc_%s.xml"

## Path and names of symbols files, used to create a dictionary of symbols used in the ThermRegistry
tSymPath = tStructureHome + "aspect_registry/symbols/"
tSymFiles = [tSymPath + "symbolsCap.xml", tSymPath + "symbolsCond.xml", tSymPath + "symbolsMisc.xml"]

## Name and path of ptcs enumeration header to generate
tEnumFile = os.environ["ICD_HOME"] + "/enum/StructureEnum.hh"

## Name of icd file and path of trickView file
tIcdFile = os.environ["ICD_HOME"] + "/STRUCTURE_ICD.txt"
tTvPath = os.environ["SIM_HOME"] + "/SIM_AxH1/Modified_data/trick_tv_files/structure/TV_icd_%s.tv"

## Uniform ICD variables
tIcdSys = "gunns"
tIcdBus = "simbus"
tIcdTrickSeq = ""

#==================================================================================================
# Main script
#==================================================================================================
if __name__ == "__main__":
    print "\n======================================================================"
    print thisScript
    
    ## Instantiate ThermAspectConfig object.
    thermAspectConfig = ThermAspectConfig()
    
    ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
    thermAspectConfig.cCallingScript = thisScript
    ## Path to repository home.
    thermAspectConfig.cHome = tStructureHome
    ## Assumption to use for any specific heat values (Cp) not provided directly.
    thermAspectConfig.cAssumedCp = 896.0
    ## Set ICD file
    thermAspectConfig.cIcdFile = tIcdFile
    ## Set enumeration header file.
    thermAspectConfig.cEnumFile = tEnumFile   
    ## Set symbol_loading module and files to read.
    thermAspectConfig.cSymFiles = tSymFiles
    
    ## Loop through all individual networks.
    for network in tNetworks:
        
        ## Instantiate IndivNetworkConfig object.
        indivNetworkConfig = IndivNetworkConfig()
        
        ## Uniform ICD variables
        ## Name of sim system
        indivNetworkConfig.cIcdSettings.mSys = tIcdSys
        ## Name of icd bus
        indivNetworkConfig.cIcdSettings.mBus = tIcdBus
        ## Name of icd trick sequence, or P-value.
        indivNetworkConfig.cIcdSettings.mTrickSeq = tIcdTrickSeq
        
        ## Name (abbreviation) of network to be built
        indivNetworkConfig.cNetwork = network
        
        ## Boolean to determine if the mass of structural nodes should be adjusted to match a
        ## total-mass expected value.
        indivNetworkConfig.cIsMassAdjustable = True
    
        #Files to read ............................................
        ## Path and file name of Thermal Aspect Registry file.
        indivNetworkConfig.cRegisFile = tRegisPath % network
        ## Path and file name of Thermal Desktop file (TdNetworkConfig).
        indivNetworkConfig.cTdFile = tTdPath % network


        ## Path and file name of HtrRegistry file.
        indivNetworkConfig.cHtrFile = tHtrPath % network
        ## Path and file name of ThermalPanel registry/config file.
        indivNetworkConfig.cPanFile = tPanPath % network     
        
        #Files to generate ........................................
        ## Path and file name of Node configuration file.
        indivNetworkConfig.cNodeFile = tNodePath % network
        ## Path and file name of Conduction configuration file.
        indivNetworkConfig.cCondFile = tCondPath % network
        ## Path and file name of Radiation configuration file.
        indivNetworkConfig.cRadFile = tRadPath % network
        ## Path and file name of Et.Cetera (just potentials, really) configuration file.
        indivNetworkConfig.cEtcFile = tEtcPath % network
        ## Path and file name of TrickView file. Each network produces its own TrickView file.
        indivNetworkConfig.cTvFile = tTvPath % network
            
        ## Append the newly-constructed mIndivNetworkConfig to the list of network configs.
        thermAspectConfig.cListNetworkConfigs.append(indivNetworkConfig)
    
    try:
        ## Instantiate, initialize, and execute the ThermAspectBuilder.
        builder = ThermAspectBuilder()
        builder.initialize(thermAspectConfig)
        builder.execute()
    
    except Exception, e:
        ## Build error message, checking first to see if colors are enabled.
        if sys.stdout.isatty():
            # print in red
            print "\033[31mERROR!!!!!!!!!!!!!!!\033[0m",
        else:
            # print normal
            print "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!",
        print e
        print "Building of PTCS networks has FAILED.\n"
        
    print "======================================================================"
