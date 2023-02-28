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

  # Use link data to build a dict of json info
  def linkStyle(self, link):
    style_dict = {}
    # Text
    style_dict['text'] = link[1]
    # Size
    style_dict['size'] = link[8]['width'] + ' ' + link[8]['height']
    # Position
    pos = [float(link[8]['x']), float(link[8]['y'])]
    style_dict['pos'] = str(pos[0] + float(link[8]['width'])/2) + ' ' + str(pos[1] + float(link[8]['height'])/2)
    # Convert style string
    for s in link[7].split(';'):
      if 'rotation' in s: style_dict['angle'] = s[9:]
      else: style_dict['angle'] = '0'
    # Convert shape
    style_dict['shape'] = self.convertShape(link[9])
    # Get connection points
    style_dict['connections'] = self.getConnections(link[9])
    return style_dict
  
  # Transform link shape data into GoJS geometry string
  def convertShape(self, shape):
    geom_strs = []
    index = 0
    for layer in ['background', 'foreground']:
      if not shape.find('./' + layer): continue
      if len(geom_strs) > 0: geom_strs[-1] += 'z'
      for obj in shape.find('./' + layer):
        if obj.tag == 'stroke' or obj.tag == 'fontfamily' or obj.tag == 'text': continue
        elif obj.tag == 'fill' or obj.tag == 'fillstroke': geom_strs[index-1] = 'F ' + geom_strs[index-1] + 'x '; continue
        elif obj.tag == 'rect': geom_strs.append('M' + self.formatAttribs(obj.attrib, ['x', 'y']) + 'L' + self.formatAttribs(obj.attrib, ['rect']))
        else:
          geom_strs.append('')
          for item in obj:
            attribs = item.attrib
            if item.tag == 'move': geom_strs[index] += 'M' + self.formatAttribs(attribs, ['x', 'y'])
            elif item.tag == 'line': geom_strs[index] += 'L' + self.formatAttribs(attribs, ['x', 'y'])
            elif item.tag == 'curve': geom_strs[index] += 'C' + self.formatAttribs(attribs, ['x', 'y', 'x1', 'y1', 'x2', 'y2'])
            elif item.tag == 'arc': geom_strs[index] += 'A' + self.formatAttribs(attribs, ['rx', 'ry', 'x-axis-rotation', 'large-arc-flag', 'sweep-flag', 'x', 'y'])
            elif item.tag == 'close': geom_strs[index] = geom_strs[index][:-1] + 'z '; continue
            else: print('Tag not recognized: ' + item.tag)
        while geom_strs[index][-1] == ' ': geom_strs[index] = geom_strs[index][:-1]
        # if geom_strs[index][-1] != 'z': geom_strs[index] += 'z'
        index += 1
    final = ''
    for s in geom_strs: final += s + ' '
    return final
  
  def formatAttribs(self, attribs, keys):
    s = ''
    if keys[0] == 'rect':
      s += str(int(attribs['x']) + int(attribs['w'])) + ' ' + attribs['y'] + ' '
      s += str(int(attribs['x']) + int(attribs['w'])) + ' ' + str(int(attribs['y']) + int(attribs['h'])) + ' '
      s += attribs['x'] + ' ' + str(int(attribs['y']) + int(attribs['h'])) + ' '
      s += attribs['x'] + ' ' + attribs['y'] + ' '
      return s
    for key in keys:
      try: s += attribs[key] + ' '
      except: print('Key not found: ' + key)
    return s
  
  def getConnections(self, shape):
    connections = '['
    for item in shape.find('./connections'):
      connections += '{"portId":"' + item.attrib['name'] + '",'
      connections += '"x":"' + item.attrib['x'] + '",'
      connections += '"y":"' + item.attrib['y'] + '"},'
    connections = connections[:-1] + ']'
    return connections

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
    # Extract connection point from style string
    style_str = port[3]
    style_str = style_str[style_str.find(e_str + 'X=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Y=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dx=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dy=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    for constraint in obj[9].find('./connections').iter('constraint'):
      if constraint.attrib['x'] == port_pt[0] and constraint.attrib['y'] == port_pt[1]: 
        return is_from, constraint.attrib['name']
    return is_from, '0'

  def render(self):
    r =('{ "class": "go.GraphLinksModel",\n'
        '  "linkFromPortIdProperty": "fromPort",\n'
        '  "linkToPortIdProperty": "toPort",\n'
        '  "nodeDataArray": [\n')
    for spotter in self.data['spotters']:
      r = r + ('')
    for link in self.data['links']:
      link_style = self.linkStyle(link)
      r = r + ('    {"key":"' + link_style['text'] + '","category":"Link","pos":"' + link_style['pos'] + 
                    '","size":"' + link_style['size'] + '","text":"' + link_style['text'] + '","angle":"' + link_style['angle'] + 
                    '","geometryString":"' + link_style['shape'] + '","itemArray":' + link_style['connections'] + '},\n')
    for node in self.data['nodes']:
      r = r + ('    {"key":"' + node[0] + '","category":"Node","pos":"' + node[2]['x'] + ' ' + node[2]['y'] + '","text":"Node' + node[0] + '"},\n')
    r = r[:-2]
    r = r + ('\n],\n'
             '  "linkDataArray": [\n')
    for port in self.data['ports']:
      if float(port[0]) > 1: continue
      is_from, port_name = self.portPos(port)
      r = r + ('    {"label":"' + port[0] + '","from":"')
      if is_from: r = r + (port[1] + '","fromPort":"' + port_name + '","to":"' + port[2] + '","toPort":"' + port[0] + '"},\n')
      else: r = r + (port[1] + '","fromPort":"' + port[0] + '","to":"' + port[2] + '","toPort":"' + port_name + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

