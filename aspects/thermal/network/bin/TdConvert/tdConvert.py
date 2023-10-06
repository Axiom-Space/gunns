# tdConvert.py 
# Author: Logan Daigle (Axiom Space) logan.daigle@axiomspace.com
# Purpose: Takes node information from SINDA input files (*.k and *.cc), converts them into a GUNNS *.xml format, and saves in the appropriate model location 
# Dependencies: Must convert input files into .txt to be parsed properly. 
#########################################################################################
import os

# Reads lines of specified *.k file and extracts radiation link information
def parse_k(k_path):
    rad_links = []
    # Parses file and stores radiation links ([node0, node1, rad coeff]) in a list
    with open(k_path, 'r', encoding='cp1252') as file:
        for line in file:
            if line[0] != "C":
                rad_info = line.split(',')  
                if len(rad_info) == 5 or len(rad_info) == 4:
                    rad_links.append([rad_info[1].strip(' ').replace('.', '_'), rad_info[2].strip(' ').replace('.', '_'), float(rad_info[3].split('$')[0].strip(' '))])                  
    
    temp_rad_links = []
    for link in rad_links:
        if 'MAIN' not in link[0] and 'MAIN' not in link[1]:
            temp_rad_links.append(link)
    rad_links = temp_rad_links

    # Filter out links lower than specified coefficient
    temp_rad_links = []
    for link in rad_links:
        if link[2] > 0:
            temp_rad_links.append(link)
    rad_links = temp_rad_links
    return rad_links

# Reads lines of specified *.cc file and extracts node information and conduction links 
def parse_cc(cc_path):
    nodes = []
    cond_links = []
    submodel_nodes = dict()
    node_flag = 0
    cond_flag = 0
    submodel = ""
    # Parses file and stores node information ([submodel_num, init_temp, capacitance]) and conduction links ([node0, node1, conductivity]) in respective lists
    with open(cc_path, 'r', encoding='cp1252') as file:
        for line in file:
            if node_flag == 0 and cond_flag == 0:
                if "HEADER NODE DATA" in line:
                    node_flag = 1
                    submodel = line.split(',')[1].strip('\n').strip(' ')
                    node_info = []
                    node_ct = 0
                elif "HEADER CONDUCTOR DATA" in line or "C Contact" in line:
                    cond_flag = 1
                    cond_info = []
            elif node_flag == 1:
                if "HEADER CONDUCTOR DATA" in line or "C Contact" in line:
                    node_flag = 0
                    cond_flag = 1
                    cond_info = []
                    submodel_nodes[submodel] = node_ct
                    node_ct = 0
                elif "HEADER NODE DATA" in line:
                    cond_flag = 0
                    node_flag = 1
                    node_info = []
                    submodel_nodes[submodel] = node_ct
                    submodel = line.split(',')[1].strip('\n').strip(' ')
                    node_ct = 0
                elif "HEADER" in line:
                    cond_flag = 0
                    node_flag = 0
                    submodel_nodes[submodel] = node_ct
                else:
                    node_info = line.split(',')
                    if "SIV" in node_info[0]:
                        nodes.append([submodel + "_" + node_info[0].replace('SIV', '').strip(' '), float(node_info[1].strip(' ')), float(node_info[3].strip(' '))])
                        node_ct += 1                    
                    else:
                        if 'AXH1_D_StartTemperature' in node_info[1]:
                            node_info[1] = eval(node_info[1].replace('AXH1_D_StartTemperature', '70'))
                        else:
                            node_info[1] = float(node_info[1].strip(' '))
                        nodes.append([submodel + "_" + node_info[0].strip(' '), node_info[1], float(node_info[2].strip(' '))])
                        node_ct += 1
            elif cond_flag == 1:
                if "HEADER CONDUCTOR DATA" in line or "C Contact" in line:
                    cond_flag = 1
                    cond_info = []
                elif "HEADER NODE DATA" in line:
                    cond_flag = 0
                    node_flag = 1
                    node_info = []
                    submodel = line.split(',')[1].strip('\n').strip(' ')
                    node_ct = 0
                else: 
                    cond_info = line.split(',')
                    if len(cond_info) == 4:
                        if 'PropPanel_ProPanelBoltR' in cond_info[3]:
                            cond_info[3] = eval(cond_info[3].replace('PropPanel_ProPanelBoltR', '112.4'))
                        cond_links.append([cond_info[1].strip(' ').replace('.', '_'), cond_info[2].strip(' ').replace('.', '_'), float(cond_info[3])])
                    elif len(cond_info) == 5:
                        cond_links.append([cond_info[1].strip(' ').replace('.', '_'), cond_info[2].strip(' ').replace('.', '_'), float(cond_info[4])])
    # Removing negative node numbers and capacitance
    ct = 0
    for n in nodes:
        if '-' in n[0]:
            nodes[ct][0] = n[0].replace('-', '')
        if n[2] == -1.0:
            nodes[ct][2] = 0.0
        ct += 1

    # Purge "MAIN" submodel
    temp_nodes = []
    for n in nodes:
        if 'MAIN' not in n[0]:
            temp_nodes.append(n)
    nodes = temp_nodes
    temp_cond_links = []
    for link in cond_links:
        if 'MAIN' not in link[0] and 'MAIN' not in link[1]:
            temp_cond_links.append(link)
    cond_links = temp_cond_links
    if 'MAIN' in submodel_nodes:
        submodel_nodes.pop('MAIN')

    # Filter out links lower than specified coefficient
    temp_cond_links = []
    for link in cond_links:
        if link[2] > 0:
            temp_cond_links.append(link)
    cond_links = temp_cond_links
    return nodes, cond_links, submodel_nodes

# Reformats collected data lists to GUNNS .xml
def format_xml(nodes, cond_links, rad_links):
    xml_str = """<?xml version="1.0" ?>\n<network>"""

    for n in nodes:
        xml_str += f"""
    <node>
        <name>{n[0]}</name>
        <temperature units = "K">{n[1]}</temperature>
        <capacitance units="J/K">{n[2]}</capacitance>
    </node>"""

    for link in rad_links:
        xml_str += f"""
    <radiation>
        <node0>{link[0]}</node0>
        <node1>{link[1]}</node1>
        <coefficient units="m2">{link[2]:.9f}</coefficient>
    </radiation>"""

    for link in cond_links:
        xml_str += f"""
    <conduction>
        <node0>{link[0]}</node0>
        <node1>{link[1]}</node1>
        <conductivity units="W/K">{link[2]:.9f}</conductivity>
    </conduction>"""

    xml_str += "\n</network>"
  
    return xml_str

# Saves .xml string to file in correct model folder
def save_xml(save_path, xml_str, model):
    save_path_file = save_path + "/TdNetworkConfig_%s.xml" % model
    with open(save_path_file, "w") as f:
        f.write(xml_str)

# Main
if __name__ == "__main__":
    
    script_path = os.path.realpath(os.path.dirname(__file__))
    model_list = ["h1", "sm"]

    for model in model_list:
        model_path = script_path + "/tdFiles/%s" % model
        k_path = model_path + "/C36A2.txt"
        cc_path = model_path + "/case36.txt"
        print(cc_path)
        save_path = os.getenv('GUNNS_EXT_PATH').replace('-I', '') + "/Ptcs/therm/%s/aspect_registry" % model

        rad_links = parse_k(k_path)
        [nodes, cond_links, submodel_nodes] = parse_cc(cc_path)
        xml_str = format_xml(nodes, cond_links, rad_links)
        save_xml(save_path, xml_str, model)

        tot = 0
        print("Converting", model, "TD model")
        for n in submodel_nodes:
            tot += submodel_nodes[n]
            print(n + ': ' + str(submodel_nodes[n]))
        print("________________________________")
        print("Total Nodes:", len(nodes))
        print("Total Cond Links:", len(cond_links))
        print("Total Rad Links:", len(rad_links))