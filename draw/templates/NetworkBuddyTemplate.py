#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, March 2023, --, Initial implementation.}
# @revs_end
#
import os

# This implements a templated output of the buddy class utilities file (.hh)
class NetworkBuddyTemplate:

  def __init__(self, data):
    self.data = data
    self.getters, self.setters = self.getGunnsFuncs('/home/vagrant/ax-sim/lib/gunns')
    return

  # Function to grab all getter and setter functions from GUNNS
  def getGunnsFuncs(self, gunns_home):
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
              if len(lines) <= 0: continue  # print("Could not process link: ", link);
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
      params += 'vecstr[]'+ str(v_index) + '], '
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
        '#include <voscpp/vos.h>\n'
        '#include "' + self.data['networkName'] + '.hh"\n'
        '\n'
        '// Main Class\n'
        'class ' + name + '\n'
        '{\n'
        '    public:\n'
        '        // GUNNS Link Maps\n'
        '        std::map<std::string, std::string> linkTypes;\n'
        '        std::map<std::string, std::vector<std::string>> linkAttribs;\n')
    linkMap = self.linkTypes(self.data)
    for linkType in linkMap.keys():
      linkName = linkType[5].lower() + linkType[6:] if linkType[:5] == 'Gunns' else linkType[0].lower() + linkType[1:]
      r = r + ('        std::map<std::string, ' + linkType + '*> ' + linkName + 's;\n')
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
      r = r + ('            // Initialize link map for ' + linkType + '\n'
        '            linkAttribs["' + linkType + '"] = {')
      for attrib in self.getters[linkType]:
        r = r + ('"' + attrib + '", ')
      r = r + ('};\n') if r[-1] == '{' else r[:-2] + ('};\n')
      for link in linkMap[linkType]:
        linkName = linkType[5].lower() + linkType[6:] if linkType[:5] == 'Gunns' else linkType[0].lower() + linkType[1:]
        r = r + ('            ' + linkName + 's["' + link + '"] = &net->' + link + ';\n')
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
        linkName = linkType[5].lower() + linkType[6:] if linkType[:5] == 'Gunns' else linkType[0].lower() + linkType[1:]
        r = r + ('            if (vecstr[1] == "' + attrib + '") { return std::to_string(' + linkName + 's[vecstr[0]]->' + self.format(self.getters[linkType][attrib]) + '); }\n')
      r = r + ('            return "Attribute cannot be retreived.";\n'
        '        };\n')
    for linkType in linkMap.keys():
      r = r + ('\n'
        '        /** @brief Set attribute from ' + linkType + '\n'
        '         * @param attrib : Attribute to be retreived\n'
        '        *******************************************************************************/\n'
        '        std::string set' + linkType + 'Attrib(const std::vector<std::string> &vecstr) {\n')
      # for attrib in self.setters[linkType].keys():
      #   linkName = linkType[5].lower() + linkType[6:] if linkType[:5] == 'Gunns' else linkType[0].lower() + linkType[1:]
      #   r = r + ('            if (vecstr[1] == "' + attrib + '") { ' + linkName + 's[vecstr[0]]->' + self.format(self.setters[linkType][attrib]) + '; }\n')
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
        '        std::map<std::string, std::string> parseCommand(const std::vector<std::string> &vecstr) {\n'
        '            if (vecstr[0] == "getFlux") return getFlux();\n'
        '            if (vecstr[0] == "getLink") return getLink(vecstr[1]);\n'
        '            typename std::map<std::string, ' + name + '::commandFunc>::const_iterator it;\n'
        '            it = funcMap.find(vecstr[0] + linkTypes[vecstr[1]]);\n'
        '            if (it == funcMap.end()) return {};\n'
        '            commandFunc s = it->second;\n'
        '            return {{vecstr[2], (this->*s)({vecstr.begin()+1, vecstr.end()})}};\n'
        '        };\n'
        '\n')
    # Helper Functions
    r = r + ('        /** @brief Get all information on one link\n'
        '         * @param link : name of link\n'
        '        *******************************************************************************/\n'
        '        std::map<std::string, std::string> getLink(const std::string &link, const std::vector<std::string> &attribs=std::vector<std::string>()) {\n'
        '            std::map<std::string, std::string> result;\n'
        '            typename std::map<std::string, ' + name + '::commandFunc>::const_iterator it;\n'
        '            if (attribs.size() > 0) {\n'
        '                for (std::string attrib : attribs) {\n'
        '                    it = funcMap.find("get" + linkTypes[link]);\n'
        '                    if (it == funcMap.end()) continue;\n'
        '                    commandFunc s = it->second;\n'
        '                    result[attrib] = (this->*s)({link, attrib});\n'
        '                }\n'
        '            } else {\n'
        '                for (std::string attrib : linkAttribs[linkTypes[link]]) {\n'
        '                    it = funcMap.find("get" + linkTypes[link]);\n'
        '                    if (it == funcMap.end()) continue;\n'
        '                    commandFunc s = it->second;\n'
        '                    result[attrib] = (this->*s)({link, attrib});\n'
        '                }\n'
        '            }\n'
        '            return result;\n'
        '        };\n'
        '\n'
        '        /** @brief Get flux from every link in network\n'
        '         * @param links : map of all links, cast to GunnsBasicLink\n'
        '        *******************************************************************************/\n'
        '        std::map<std::string, std::string> getFlux() {\n'
        '            std::map<std::string, std::string> result;\n')
    for link in self.data['links']:
        if 'flux' in self.getters[link[0]]:
          r = r + ('            result["' + link[1] + '"] = net->' + link[1] + '.getFlux();\n')
    r = r + ('            return result;\n'
        '        };\n'
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