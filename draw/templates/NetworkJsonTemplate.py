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

  def linkShape(self, name):
    if 'Valve' in name:
      return 'Valve'
    else:
      return 'Process'

  def linkStyle(self, style):
    r = ''
    for s in style.split(';'):
      if 'rotation' in s:
        r = r + ',"angle":"' + s[9:] + '"'
    return r
  
  def linkPaths(self, links):
    link_paths = []
    for link in links:
      for arg in link[4].split(','):
        if 'Node' in arg:
          if ';' in arg:
            link_paths.append((arg[5:arg.index('+')-1], link[1]))
          else:
            link_paths.append((link[1], arg[5:arg.index('+')-1]))
    return link_paths
  
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
      r = r + ('    {"key":"' + link[1] + '","category":"' + self.linkShape(link[0]) + '","pos":"' + link[8]['x'] + ' ' + link[8]['y'] + 
                    '","size":"' + link[8]['width'] + ' ' + link[8]['height'] + '","text":"' + link[1] + '"' + self.linkStyle(link[7]) + '},\n')
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
    # path_dict = self.linkPaths(self.data['links'])
    # for path in path_dict:
    #   r = r + ('    {"from":"' + path[0] + '","to":"' + path[1] + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

