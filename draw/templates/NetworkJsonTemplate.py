#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, February 2023, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network display config (.json) file for GUNNS networks.
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
          if 'Prv' in link[0]: print(arg)
          if ';' in arg:
            link_paths.append((arg[5:arg.index('+')-1], link[1]))
          else:
            link_paths.append((link[1], arg[5:arg.index('+')-1]))
    return link_paths
  
  def render(self):
    r =('{ "class": "go.GraphLinksModel",\n'
        '  "nodeDataArray": [\n')
    for spotter in self.data['spotters']:
      r = r + ('')
    for link in self.data['links']:
      if 'Prv' in link[0]: print(link[4])
      r = r + ('    {"key":"' + link[1] + '","category":"' + self.linkShape(link[0]) + '","pos":"' + link[8]['x'] + ' ' + link[8]['y'] + 
                    '","size":"' + link[8]['width'] + ' ' + link[8]['height'] + '","text":"' + link[1] + '"' + self.linkStyle(link[7]) + '},\n')
    for node in self.data['nodes']:
      r = r + ('    {"key":"' + node[0] + '","category":"Circle","pos":"' + node[2]['x'] + ' ' + node[2]['y'] + '","text":"' + node[0] + '"},\n')
    r = r[:-2]
    r = r + ('\n],\n'
             '  "linkDataArray": [\n')
    path_dict = self.linkPaths(self.data['links'])
    for path in path_dict:
      r = r + ('    {"from":"' + path[0] + '","to":"' + path[1] + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

