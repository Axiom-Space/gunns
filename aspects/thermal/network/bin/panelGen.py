# tdConvert.py 
# Author: Logan Daigle (Axiom Space) logan.daigle@axiomspace.com
# Purpose: Takes node normal vectors exported from Thermal Desktop, creates panels for external surfaces, and converts them into a GUNNS *.xml format
# Dependencies: Needs an exported normal vector list and external node list from TD
#########################################################################################
import os

class panelGen:
    def __init__(self, model):
        self.parse_path = os.path.realpath(os.path.dirname(__file__))
        nodes_path = self.parse_path + '/' + f'TdConvert/tdFiles/{model}/nodelist_{model}.txt'
        vect_path = self.parse_path + '/' + f'TdConvert/tdFiles/{model}/vectors_{model}.txt'
        self.parse_nodes(nodes_path)
        self.parse_vect(vect_path)
        self.xml_str = self.format_xml()

    # Parses specified node list file
    def parse_nodes(self, nodes_path):
        self.nodes = dict()
        # Parses file and stores node info in a dict (nodes[node_name] = [x_pos, y_pos, z_pos, area])
        with open(nodes_path, 'r') as file:
            for line in file:
                content = line.split()
                self.nodes[content[0].replace('.', '_')] = [content[1], content[2], content[3], content[4]]
        return

    # Parses specified normal vector file and filters for specified nodes
    def parse_vect(self, vect_path):
        self.normals = dict()

        # Parses file and stores normal vector info in a dict (nodes[node_name] = [x, y, z])
        with open(vect_path, 'r') as file:
            for n in self.nodes:
                found = 0
                for line in file:
                    content = line.split()
                    if found != 1:
                        if n == content[0].replace('.', '_'):
                            self.normals[content[0].replace('.', '_')] = [content[1], content[2], content[3]]
                            found = 1
                if found == 0:
                    print(n, f"not found in {vect_path}. Not adding to panel registry.")
                file.seek(0)
        return
    
    # Reformats collected data lists to GUNNS .xml
    def format_xml(self):
        xml_str = """<?xml version="1.0" ?>\n<list>"""

        for n in self.normals:
            xml_str += f"""
    <panel name='pan_{n}'>
        <node>{n}</node>
        <area>{self.nodes[n][3]}</area>
        <absorp>1.0</absorp>
        <position>
            <x>{self.nodes[n][0]}</x>
            <y>{self.nodes[n][1]}</y>
            <z>{self.nodes[n][2]}</z>
        </position>
        <surfaceNormal>
            <x>{self.normals[n][0]}</x>
            <y>{self.normals[n][1]}</y>
            <z>{self.normals[n][2]}</z>
        </surfaceNormal>
        <incidentFlux isReadyForIcd='true'>
            <rate>PTCS_RATE</rate>
            <simObject>Flux_SimObj flux_SimObj</simObject>
            <simVarName>flux.solar_fluxes[pan_{n}]</simVarName>
        </incidentFlux>
    </panel>"""

        xml_str += "\n</list>"
    
        return xml_str

    # Saves .xml string to file
    def save_xml(self, model):
        print("Generating", len(self.normals), "thermal panels")
        print("___________________________")
        save_path = os.environ["MODELS_HOME"] + f"/gunns/Ptcs/therm/{model}/aspect_registry/PanelRegistry_{model}.xml"
        with open(save_path, "w") as f:
            f.write(self.xml_str)

# Main
if __name__ == "__main__":
    model_list = ["h1", "sm"]
    for model in model_list:
        panels = panelGen(model)
        panels.save_xml(model)