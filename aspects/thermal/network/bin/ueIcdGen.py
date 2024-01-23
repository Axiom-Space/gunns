# ueIcdGen.py 
# Author: Logan Daigle (Axiom Space) logan.daigle@axiomspace.com
# Purpose: Creates the UNREAL_FLUX_ICD from imported thermal panel files
# Dependencies: Thermal panel files in the models 
#########################################################################################
import os
import pandas as pd

# Parses PanelRegistry
def parse_panel(panel_path):
    name_list = []
    with open(panel_path, 'r') as f:
        for line in f:
            if 'name=' in line:
                name = line.replace("    <panel name='", '').replace("'>", '').replace('\n','')
                name_list.append(name)
    return name_list

# Formats text for UNREAL_FLUX_ICD 
def format_icd(model, name_list):
    icd_str = ""
    icd_str += f"\n#{model} shadow\n"
    for name in name_list:
        icd_str += f"xxxx    gunns    simbus    test    {name}_shadowFlag    WRITE    DYNAMICS    UnrealEngine_SIM_SimObj,AxH1_sim_unreal_engine    Unreal.isShadowed_{model}[{name}]    int    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_shadowFlag    READ    DYNAMICS    Flux_SimObj,flux_SimObj    flux.{model}.shadow_flag[{name}]    int    (--)\n"

    icd_str += f"\n#{model} flux positions\n"
    for name in name_list:
        icd_str += f"xxxx    gunns    simbus    test    {name}_xpos    WRITE    DYNAMICS    Flux_SimObj,flux_SimObj    flux.{model}.surface_posx[{name}]    double    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_xpos    READ    DYNAMICS    UnrealEngine_SIM_SimObj,AxH1_sim_unreal_engine    Unreal.input.{model}_flux_posx[{name}]    double    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_ypos    WRITE    DYNAMICS    Flux_SimObj,flux_SimObj    flux.{model}.surface_posy[{name}]    double    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_ypos    READ    DYNAMICS    UnrealEngine_SIM_SimObj,AxH1_sim_unreal_engine    Unreal.input.{model}_flux_posy[{name}]    double    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_zpos    WRITE    DYNAMICS    Flux_SimObj,flux_SimObj    flux.{model}.surface_posz[{name}]    double    (--)\n"
        icd_str += f"xxxx    gunns    simbus    test    {name}_zpos    READ    DYNAMICS    UnrealEngine_SIM_SimObj,AxH1_sim_unreal_engine    Unreal.input.{model}_flux_posz[{name}]    double    (--)\n"

    return icd_str


# Saves icd string to file
def save_icd(icd_str):
    print("Saving ICD")
    save_path = os.environ["ICD_HOME"] + f"/flux_icds/UNREAL_FLUX_ICD.txt"
    with open(save_path, "w") as f:
        f.write(icd_str)

    return

# Main
if __name__ == "__main__":
    model_list = ["h1", "sm"]
    icd_str = "PUI SYS BUS SUBSYS  VAR_NAME    ACTION  RATE        SIM_OBJECT      SIM_VAR_NAME                TYPE    UNITS   COMMENTS\n"
    for model in model_list:
        panel_path = os.environ["MODELS_HOME"] + f"/gunns/ptcs/therm/{model}/aspect_registry/PanelRegistry_{model}.xml"
        name_list = parse_panel(panel_path)
        print(len(name_list))
        icd_str += format_icd(model, name_list)
    save_icd(icd_str)