#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, February 2023, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network display config (.json) file for GUNNS networks.
import math
import numpy as np
from scipy.spatial import Voronoi, voronoi_plot_2d
from scipy.spatial.distance import cdist
import matplotlib.pyplot as plt

class NetworkJsonTemplate:

  data = {}

  def __init__(self, data):
    self.data = data
    return

  def linkStyle(self, link):
    style_dict = {}
    # Text
    style_dict['text'] = link[1]
    # Category
    if 'Valve' in link[0]:
      style_dict['category'] = 'Valve'
    else:
      style_dict['category'] = 'Process'
    # Size
    style_dict['size'] = link[8]['width'] + ' ' + link[8]['height']
    # Position
    pos = [float(link[8]['x']), float(link[8]['y'])]
    style_dict['pos'] = str(pos[0] + dims[0]/2) + ' ' + str(pos[1] + dims[1]/2)
    # Convert style string
    for s in link[7].split(';'):
      if 'rotation' in s:
        style_dict['angle'] = s[9:]
      
    return style_dict
  
  # Use port style data to determine which link connector it should be attached to
  def portPos(self, port):
    from_obj = None
    to_obj = None
    for link in self.data['links']:
      if link[1] == port[1]: from_obj = link
      elif link[1] == port[2]: to_obj = link
    is_from = True if from_obj else False
    obj = from_obj if from_obj else to_obj
    e_str = 'exit' if from_obj else 'entry'
    port_pt = []
    style_str = port[3]
    # Extract connection point from style string
    style_str = style_str[style_str.find(e_str + 'X=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Y=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dx=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dy=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    port_index = 0
    for constraint in obj[9].find('./connections').iter('constraint'):
      if constraint.attrib['x'] == port_pt[0] and constraint.attrib['y'] == port_pt[1]: break
      port_index += 1
    return is_from, port_index

  def render(self):
    r =('{ "class": "go.GraphLinksModel",\n'
        '  "linkFromPortIdProperty": "fromPort",\n'
        '  "linkToPortIdProperty": "toPort",\n'
        '  "nodeDataArray": [\n')
    for spotter in self.data['spotters']:
      r = r + ('')
    for link in self.data['links']:
      link_style = linkStyle(link)
      r = r + ('    {"key":"' + link_style['text'] + '","category":"' + link_style['category'] + '","pos":"' + link_style['pos'] + 
                    '","size":"' + link_style['size'] + '","text":"' + link_style['text'] + '","angle":"' + link_style['angle'] + '"},\n')
    for node in self.data['nodes']:
      r = r + ('    {"key":"' + node[0] + '","category":"Circle","pos":"' + node[2]['x'] + ' ' + node[2]['y'] + '","text":"Node' + node[0] + '"},\n')
    r = r[:-2]
    r = r + ('\n],\n'
             '  "linkDataArray": [\n')
    for port in self.data['ports']:
      is_from, port_num = self.portPos(port)
      r = r + ('    {"label":"' + port[0] + '","from":"')
      if is_from: r = r + (port[1] + '","fromPort":"' + str(port_num) + '","to":"' + port[2] + '","toPort":"' + port[0] + '"},\n')
      else: r = r + (port[1] + '","fromPort":"' + port[0] + '","to":"' + port[2] + '","toPort":"' + str(port_num) + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

