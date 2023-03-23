#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, March 2023, --, Initial implementation.}
# @revs_end
#
import os

# Function to grab all getter and setter functions from GUNNS
def getGunnsFuncs(gunns_home):
  getters = {}
  setters = {}
  link_structure = {}
  for folder in ['/core', '/aspects']:
    for root, dirs, files in os.walk(gunns_home + folder):
      for file in files:
        if '.h' in file:
          link = file[:file.index('.')]
          getters[link]= {}
          setters[link]= {}
          with open(root + '/' + file, 'r') as gfile:
            lines = gfile.readlines()
            while len(lines) > 0: 
              if 'class ' + link in lines[0] and 'InputData' not in lines[0] and 'ConfigData' not in lines[0]: break
              lines.pop(0)
            if len(lines) <= 0: print("Could not process link: ", link); continue
            tmp = lines[0][lines[0].find(':')+1:].strip().split(' ') if ':' in lines[0] else ['', '', '']
            link_structure[link] = tmp[2] if tmp[1] == '' else tmp[1]
            for line in lines[1:]:
              if 'protected:' in line or 'private:' in line: break
              line = line.strip()
              if 'get' in line and line[-1] == ';' and '(' in line and '*' not in line:
                func = line[line.index('get'):line.index(')')+1]
                attr = func[3].lower() + func[4:func.find('(')]
                getters[link][attr] = func
              if 'set' in line and line[-1] == ';' and '(' in line and '*' not in line and 'reset' not in line:
                func = line[line.index('set'):line.index(')')+1]
                attr = func[3].lower() + func[4:func.find('(')]
                setters[link][attr] = func
  visited = []
  for link in link_structure.keys():
    if link_structure[link] == '': visited.append(link)
  while len(visited) < len(link_structure.keys()):
    for link in link_structure.keys():
      if link_structure[link] in visited and link_structure[link] in link_structure.keys():
        for attr in getters[link_structure[link]].keys(): getters[link][attr] = getters[link_structure[link]][attr]
        for attr in setters[link_structure[link]].keys(): 
          setters[link][attr] = setters[link_structure[link]][attr]
          if attr == 'lastMinorStep': print(link_structure[link])
        visited.append(link)
  return getters, setters

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

  def __init__(self, data):
    self.data = data
    self.getters, self.setters = getGunnsFuncs('/home/vagrant/ax-sim/lib/gunns')
    return
  
  def format(self, func):
    func_out = func[:func.index('(')+1]
    if func[:3] == 'get': return func_out + ')'
    params = ''
    v_index = 2
    for param in [p.strip() for p in func[func.index('(')+1:func.index(')')].split(',')]:
      if param == '': continue
      tmp = param.split(' ')
      args = []
      for i in range(len(tmp)):
        if tmp[i] != '' and 'const' not in tmp[i]: args.append(tmp[i])        
      params += 'vecstr.at('+ str(v_index) + '), '
      v_index += 1
    return func_out + params[:-2] + ')'
  
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
    # Getter and Setter functions
    r = r + ('        /** @brief Type definition for link interaction functions\n'
        '         * @param vecstr : name of link, name of attribute, and optionally value to set\n'
        '        *******************************************************************************/\n'
        '        typedef std::string (' + name + '::*commandFunc)(const std::vector<std::string> &vecstr);\n')
    for linkType in linkMap.keys():
      r = r + ('\n'
        '        /** @brief Get attribute from ' + linkType + '\n'
        '         * @param vecstr : Attribute to be retreived\n'
        '        *******************************************************************************/\n'
        '        std::string get' + linkType + 'Attrib(const std::vector<std::string> &vecstr) {\n')
      for attrib in self.getters[linkType]:
        r = r + ('            if (vecstr.at(1) == "' + attrib + '") { return std::to_string(' + linkType[5].lower() + linkType[6:] + 's[vecstr.at(0)]->' + self.format(self.getters[linkType][attrib]) + '); }\n')
      r = r + ('            return "Attribute cannot be retreived.";\n'
        '        };\n')
    for linkType in linkMap.keys():
      r = r + ('\n'
        '        /** @brief Set attribute from ' + linkType + '\n'
        '         * @param attrib : Attribute to be retreived\n'
        '        *******************************************************************************/\n'
        '        std::string set' + linkType + 'Attrib(const std::vector<std::string> &vecstr) {\n')
      # for attrib in self.setters[linkType].keys():
      #   r = r + ('            if (vecstr.at(1) == "' + attrib + '") { ' + linkType[5].lower() + linkType[6:] + 's[vecstr.at(0)]->' + self.format(self.setters[linkType][attrib]) + '; }\n')
      r = r + ('            return "Attribute set.";\n'
        '        };\n')
    r = r + ('\n'
        '        /** @brief Set map for programmatic interaction\n'
        '        *******************************************************************************/\n'
        '        const std::map<std::string, ' + name + '::commandFunc> funcMap {\n')
    for linkType in linkMap.keys():
      r = r + ('            {"get' + linkType + '", &' + name + '::get' + linkType + 'Attrib},\n')
      r = r + ('            {"set' + linkType + '", &' + name + '::set' + linkType + 'Attrib},\n')
    r = r[:-2] + ('\n'
        '        };\n'
        '\n'
        '        /** @brief Parse command \n'
        '        *******************************************************************************/\n'
        '        std::string parseCommand(const std::vector<std::string> &vecstr) {\n'
        '            if (vecstr.at(0) == "getFlux") return getFlux();\n'
        '            if (vecstr.at(0) == "getLink") return getLink(vecstr.at(1));\n'
        '            typename std::map<std::string, ' + name + '::commandFunc>::const_iterator it;\n'
        '            it = funcMap.find(vecstr.at(0) + linkTypes[vecstr.at(1)]);\n'
        '            if (it == funcMap.end()) return "";\n'
        '            commandFunc s = it->second;\n'
        '            return (this->*s)({vecstr.begin()+1, vecstr.end()});\n'
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
        '\n'
        '    private:\n'
        '        ' + name + ' (const ' + name + '&);\n'
        '        ' + name + '& operator=(const ' + name  + '&);\n'
        '        ' + self.data['networkName'] + ' *net;\n'
        '};\n'
        '\n'
        '#endif')

    return r.encode('ascii', 'ignore').decode('ascii')