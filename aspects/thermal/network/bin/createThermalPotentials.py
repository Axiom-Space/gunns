# createThermalPotentials.py 
# Author: Logan Daigle (Axiom Space) logan.daigle@axiomspace.com
# Purpose: Includes generic method to find all GUNNS thermal links containing a specific submodel along with a method to add a GunnsThermalPotential to all nodes linked to a specified submodel
# Dependencies: Depends on up-to-date paths into the AxSim thermal models
#########################################################################################
import xml.etree.ElementTree as ET
import os

# Finds all links containing specified submodel, except those with itself
def find_links(link_path, submodel):
    links = []
    root = ET.parse(link_path).getroot()
    for link in root:
        for node in link:
            if submodel in node.text:
                if link in links:
                    links.remove(link)
                else:
                    links.append(link)
    print(len(links), "links found")
    return links

# Returns a list of all nodes connected to the specified submodel 
def extract_other_node(links, submodel):
    nodes = []
    for link in links:
        node0 = link.find('node0').text
        node1 = link.find('node1').text
        if submodel not in node0 and node0 not in nodes:
            nodes.append(node0)
        elif submodel not in node1 and node1 not in nodes:
            nodes.append(node1)
    print(len(nodes), "nodes added")
    return nodes

def indent(elem, level=0):
  i = "\n" + level*"    "
  if len(elem):
    if not elem.text or not elem.text.strip():
      elem.text = i + "    "
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
    for elem in elem:
      indent(elem, level+1)
    if not elem.tail or not elem.tail.strip():
      elem.tail = i
  else:
    if level and (not elem.tail or not elem.tail.strip()):
      elem.tail = i

# Finds the capacitance and initial temperature of the specified nodes and stores them in a dict
def find_cap_temp(td_path, nodes):
  td_cap_temp = dict()
  root = ET.parse(td_path).getroot()
  node_elem = root.findall('node')
  for node in nodes:
    found = 0
    for child in node_elem:
      if child.find('name').text == node:
        td_cap_temp[node] = [child.find('capacitance').text, child.find('temperature').text] 
        found = 1
    if found == 0:
       print(node, "cap not found in", td_path)
  return td_cap_temp

# Creates element tree with a GunnsThermalPotential for each node specified
def create_potential(registry_path, nodes, td_cap_temp):
    tree = ET.parse(registry_path)
    registry = tree.getroot()
    for node in nodes:
        child = ET.SubElement(registry, "node", {"name": node})

        des = ET.SubElement(child, "des")
        des.text = node + " potential"

        capacitor = ET.SubElement(child, "capacitor")
        capacitance = ET.SubElement(capacitor, "capacitance", {"units":"J/K"})
        capacitance.text = td_cap_temp[node][0]        

        potential = ET.SubElement(child, "potential")
        name = ET.SubElement(potential, "name")
        name.text = "pot_" + node
        reg_node = ET.SubElement(potential, "node")
        reg_node.text = node
        pot_temp = ET.SubElement(potential, "temperature", {"units": "K"})
        pot_temp.text = str(293.15)
        cond = ET.SubElement(potential, "conductivity", {"units": "W/K"})
        cond.text = str(0.0)
        
    indent(registry)
    # comment = ET.Comment("Copyright 2019 United States Government as represented by the Administrator of the\nNational Aeronautics and Space Administration.  All Rights Reserved.")
    # registry.addprevious(comment)
    tree.write(registry_path)
    with open(registry_path, 'r') as f:
       content = f.read()
    with open(registry_path, 'w') as f:
       header = '<?xml version="1.0" ?>\n<!-- Copyright 2019 United States Government as represented by the Administrator of the\n     National Aeronautics and Space Administration.  All Rights Reserved. -->'
       f.write(header + content)
    return
    


if __name__ == "__main__":
    rad_link_path = os.getenv("MODELS_HOME") + "/gunns/ptcs/therm/sm/ThermLinksRad_sm.xml"
    registry_path = os.getenv("MODELS_HOME") + "/gunns/ptcs/therm/sm/aspect_registry/ThermRegistry_sm.xml"
    td_path = os.getenv("MODELS_HOME") + "/gunns/ptcs/therm/sm/aspect_registry/TdNetworkConfig_sm.xml"
    copv_links = find_links(rad_link_path, "AXH1_D_COPVTANK")
    copv_other_nodes = extract_other_node(copv_links, "AXH1_D_COPVTANK")
    sm_pot_cap_temp = find_cap_temp(td_path, copv_other_nodes)
    create_potential(registry_path, copv_other_nodes, sm_pot_cap_temp)