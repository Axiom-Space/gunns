#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, March 2023, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the buddy class utilities file (.hh)
class NetworkBuddyTemplate:

  genericLinkAttribs = {
    'GunnsBasicConductor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicCapacitor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicPotential': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicSource': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicSocket': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicJumper': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicExternalSupply': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicExternalDemand': 
        {
          'flux': ('getFlux()', '')
        },
  }
  fluidLinkAttribs = {
    'GunnsFluidConductor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidCapacitor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidPotential': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSource': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidTank': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidBalloon': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidAccum': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidAccumGas': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidCheckValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluid3WayValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluid3WayCheckValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidBalancedPrv': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidRegulatorValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidReleifValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidLeak': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidDistributedIf': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidExternalSupply': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidExternalDemand': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSelectiveMembrane': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHeatExchanger': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsGasFan': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsLiquidCentrifugalPump': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsGasDisplacementPump': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsLiquidDisplacementPump': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidCondensingHxSeparator': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSimpleQd': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidPipe': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSourceBoundary': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidReactor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHotReactor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidPhaseChangeConductor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidPhaseChangeSource': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidEvaporation': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHeater': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidFlowController': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSensor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidLiquidWaterSensor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSocket': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidJumper': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidShadow': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidAdsorber': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHotAdsorber': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidMultiAdsorber': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSorptionBed': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidMetabolic': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidMetabolic2': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidEqConductor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHatch': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHiFiOrifice': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidHiFiValve': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidFireSource': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidMultiSeparator': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSeparatorGas': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSeparatorLiquid': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSimpleRocket': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSublimator': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsGasTurbine': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidTypeChangeConductor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsFluidSimpleH2Redox': 
        {
          'flux': ('getFlux()', '')
        },
  }
  thermalLinkAttribs = {
    'GunnsThermalRadiation': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalCapacitor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalPotential': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalSource': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalHeater': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalPanel': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalMultiPanel': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermalPhaseChangeBattery': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsThermoElectricDevice': 
        {
          'flux': ('getFlux()', '')
        },
  }
  electricLinkAttribs = {
    'GunnsElectBattery': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectPvArray': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectPvRegShunt': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectPvRegConv': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectRealDiode': 
        {
          'flux': ('getFlux()', '')
        },
    'SwitchElect': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectUserLoadSwitch': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectUserLoadSwitch2': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectSelector': 
        {
          'flux': ('getFlux()', '')
        },
    'ConverterElect': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectricalResistor': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsResistorPowerFunction': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsResistiveLoad': 
        {
          'flux': ('getFlux()', '')
        },
    'EpsConstantPowerLoad': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsBasicFlowController': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectIps': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectShort': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectConverterInput': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectConverterOutput': 
        {
          'flux': ('getFlux()', '')
        },
    'GunnsElectDistributedIf': 
        {
          'flux': ('getFlux()', '')
        },
  }
  
  linkAttribs = {**genericLinkAttribs, **fluidLinkAttribs, **thermalLinkAttribs, **electricLinkAttribs}

  data = {}

  def __init__(self, data):
    self.data = data
    return
  
  def linkTypes(self, data):
    linkMap = {}
    for link in data['links']:
      if link[0] in linkMap.keys():
        linkMap[link[0]].append(link[1])
      else:
        linkMap[link[0]] = [link[1]]
    return linkMap
  
  def render(self):
    name = self.data['networkName'].split('_')[0] + 'Bud_' + self.data['networkName'].split('_')[-1]
    r =('#ifndef ' + name + '_EXISTS\n'
        '#define ' + name + '_EXISTS\n'
        '\n'
        '/**\n')
    for notice in self.data['doxNotices']:
      r = r + notice + '\n'
    r = r + (
        '@file  ' + name + '.hh\n'
        '@brief ' + name + ' GUNNS ' + self.data['networkType'] + ' Network Buddy declarations.\n'
        '\n')
    for copyright in self.data['doxCopyrights']:
      r = r + copyright + '\n'
    for license in self.data['doxLicenses']:
      r = r + license + '\n'
    for dox in self.data['doxData']:
      r = r + dox + '\n'
    r = r + (
        '\n'
        '@details\n'
        'PURPOSE: (Provides classes for the ' + name + ' GUNNS ' + self.data['networkType'] + ' Network Buddy.)\n'
        '\n'
        'REFERENCES:\n'
        '  (')
    for reference in self.data['doxReferences'][:1]:
      r = r + reference
    for reference in self.data['doxReferences'][1:]:
      r = r + '\n   ' + reference
    r = r + (')\n'
        '\n'
        'ASSUMPTIONS AND LIMITATIONS:\n'
        '  (')
    for assumption in self.data['doxAssumptions'][:1]:
      r = r + assumption
    for assumption in self.data['doxAssumptions'][1:]:
      r = r + '\n   ' + assumption
    r = r + (')\n'
        '\n'
        'LIBRARY DEPENDENCY:\n'
        '  ((' + name + '.o))\n'
        '\n'
        'PROGRAMMERS:\n'
        + self.data['revline'] + '\n'
        '\n'
        '@{\n'
        '*/\n'
        '\n'
        '#include <iostream>\n'
        '#include <map>\n'
        '#include "' + self.data['networkName'] + '.hh"\n'
        '\n'
        '// Main Class\n'
        'class ' + name + '\n'
        '{\n'
        '    public:\n'
        '        // GUNNS Link Maps\n'
        '        std::map<std::string, std::string> linkTypes;\n')
    linkMap = self.linkTypes(self.data)
    for linkType in linkMap.keys():
      r = r + ('        std::map<std::string, ' + linkType + '*> ' + linkType[5].lower() + linkType[6:] + 's;\n')
    r = r + ('\n'
        '        // Default Constructor\n'
        '        ' + name + '() {}\n'
        '\n'
        '        // Initialize initialize\n'
        '        void initialize(' + self.data['networkName'] + ' *n) {\n'
        '            // Initialize pointer to GUNNS network\n'
        '            net = n;\n'
        '\n'
        '            // Initialize link type map\n')
    for link in self.data['links']:
      r = r + ('            linkTypes["' + link[1] + '"] = "' + link[0] + '";\n')
    r = r + ('\n')
    for linkType in linkMap.keys():
      r = r + ('            // Initialize link map for ' + linkType + '\n')
      for link in linkMap[linkType]:
        r = r + ('            ' + linkType[5].lower() + linkType[6:] + 's["' + link + '"] = &net->' + link + ';\n')
      r = r + '\n'
    r = r[:-1] + ('        }\n'
        '\n')
    # Define attribute getter/setter typedef for maps
    r = r + ('        /** @brief Get attribute of a link\n'
        '         * @param attribute : name of attribute to retreive\n'
        '        *******************************************************************************/\n'
        '        typedef std::string (' + name + '::* getAttrib)(std::string link, std::string attrib);\n'
        '\n'
        '        /** @brief Set attribute of a link\n'
        '         * @param attribute : name of attribute to set\n'
        '         * @param value : value to set\n'
        '        *******************************************************************************/\n'
        '        typedef std::string (' + name + '::* setAttrib)(std::string link, std::string attrib, std::string value);\n')
    # Getter functions
    for linkType in linkMap.keys():
      r = r + ('\n'
        '        /** @brief Get attribute from ' + linkType + '\n'
        '         * @param attrib : Attribute to be retreived\n'
        '        *******************************************************************************/\n'
        '        std::string get' + linkType + 'Attrib(std::string link, std::string attrib) {\n')
      for attrib in self.linkAttribs[linkType]:
        if self.linkAttribs[linkType][attrib][0] == '': continue
        r = r + ('            if (attrib == "' + attrib + '") { return std::to_string(' + linkType[5].lower() + linkType[6:] + 's[link]->' + self.linkAttribs[linkType][attrib][0] + '); }\n')
      r = r + ('            return "Attribute cannot be retreived.";\n'
        '        };\n')
    r = r + ('\n'
        '        /** @brief Get map for programmatic interaction\n'
        '        *******************************************************************************/\n'
        '        const std::map<std::string, ' + name + '::getAttrib> getAttribMap {\n')
    for linkType in linkMap.keys():
      r = r + ('            {"' + linkType + '", &' + name + '::get' + linkType + 'Attrib},\n')
    r = r[:-2] + ('\n'
        '        };\n'
        '\n'
        '        /** @brief Get attribute from a link\n'
        '        *******************************************************************************/\n'
        '        std::string get(const std::string &link, const std::string &attrib) {\n'
        '            typename std::map<std::string, ' + name + '::getAttrib>::const_iterator it;\n'
        '            it = getAttribMap.find(linkTypes[link]);\n'
        '            if (it == getAttribMap.end()) return 0;\n'
        '            getAttrib g = it->second;\n'
        '            return (this->*g)(link, attrib);\n'
        '        };\n')
    # Setter functions
    for linkType in linkMap.keys():
      r = r + ('\n'
        '        /** @brief Set attribute from ' + linkType + '\n'
        '         * @param attrib : Attribute to be retreived\n'
        '        *******************************************************************************/\n'
        '        std::string set' + linkType + 'Attrib(std::string link, std::string attrib, std::string value) {\n')
      for attrib in self.linkAttribs[linkType].keys():
        if self.linkAttribs[linkType][attrib][1] == '': continue
        r = r + ('            if (attrib == "' + attrib + '") { return ' + linkType[5].lower() + linkType[6:] + 's[link]->' + self.linkAttribs[linkType][attrib][1] + '; }\n')
      r = r + ('            return "Attribute cannot be set.";\n'
        '        };\n')
    r = r + ('\n'
        '        /** @brief Set map for programmatic interaction\n'
        '        *******************************************************************************/\n'
        '        const std::map<std::string, ' + name + '::setAttrib> setAttribMap {\n')
    for linkType in linkMap.keys():
      r = r + ('            {"' + linkType + '", &' + name + '::set' + linkType + 'Attrib},\n')
    r = r[:-2] + ('\n'
        '        };\n'
        '\n')
    # Helper Functions
    r = r + ('        /** @brief Get all information on one link\n'
        '         * @param link : name of link\n'
        '        *******************************************************************************/\n'
        '        std::string getLink(std::string link) {\n'
        '            std::cout << "PLACEHOLDER";\n')
    r = r + ('        };\n'
        '\n'
        '        /** @brief Get flux from every link in network\n'
        '         * @param links : map of all links, cast to GunnsBasicLink\n'
        '        *******************************************************************************/\n'
        '        std::string getFlux() {\n')
    for link in self.data['links']:
        r = r + ('            std::cout << net->' + link[1] + '.getFlux() << std::endl;\n')
    r = r + ('        };\n'
        '\n'
        '        /** @brief Set attribute of a link\n'
        '        *******************************************************************************/\n'
        '        std::string set(const std::string &link, const std::string &attrib, const std::string &value) {\n'
        '            typename std::map<std::string, ' + name + '::setAttrib>::const_iterator it;\n'
        '            it = setAttribMap.find(linkTypes[link]);\n'
        '            if (it == setAttribMap.end()) return 0;\n'
        '            setAttrib s = it->second;\n'
        '            return (this->*s)(link, attrib, value);\n'
        '        };\n'
        '\n'
        '    private:\n'
        '        ' + name + ' (const ' + name + '&);\n'
        '        ' + name + '& operator=(const ' + name  + '&);\n'
        '        ' + self.data['networkName'] + ' *net;\n'
        '};\n'
        '\n'
        '#endif')

    return r.encode('ascii', 'ignore').decode('ascii')