#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, February 2023, --, Initial implementation.}
# @revs_end
#
import math
import xml.etree.ElementTree as ET
import modules.compression as compression

# Map of native drawio shapes to their shape data'
shapeMap = {
  'mxgraph.electrical.miscellaneous.monocell_battery': '<shape aspect="variable" h="60" name="Monocell Battery" strokewidth="inherit" w="100"><connections><constraint name="in" perimeter="0" x="0" y="0.5"/><constraint name="out" perimeter="0" x="1" y="0.5"/></connections><foreground><path><move x="0" y="30"/><line x="45" y="30"/><move x="55" y="0"/><line x="55" y="60"/><move x="55" y="30"/><line x="100" y="30"/></path><stroke/><rect h="30" w="4" x="41" y="15"/><fillstroke/></foreground></shape>',
  'mxgraph.electrical.electro-mechanical.make_contact': '<shape aspect="variable" h="16" name="Make Contact" strokewidth="inherit" w="75"><connections><constraint name="W" perimeter="0" x="0" y="1"/><constraint name="E" perimeter="0" x="1" y="1"/></connections><foreground><path><move x="60" y="16"/><line x="75" y="16"/><move x="0" y="16"/><line x="18.5" y="16"/><line x="57" y="0"/></path><stroke/></foreground></shape>',
  'mxgraph.electrical.resistors.resistor_2': '<shape aspect="variable" h="20" name="Resistor 2" strokewidth="inherit" w="100"><connections><constraint name="in" perimeter="0" x="0" y="0.5"/><constraint name="out" perimeter="0" x="1" y="0.5"/></connections><foreground><path><move x="0" y="10"/><line x="18" y="10"/><line x="22" y="0"/><line x="30" y="20"/><line x="38" y="0"/><line x="46" y="20"/><line x="54" y="0"/><line x="62" y="20"/><line x="70" y="0"/><line x="78" y="20"/><line x="82" y="10"/><line x="100" y="10"/></path><stroke/></foreground></shape>',
  'mxgraph.signs.sports.campfire': '<shape name="Campfire" h="98.88" w="92.95" aspect="variable" strokewidth="inherit"><connections><constraint x="0.41" y="0" perimeter="0" name="N"/><constraint x="0.5" y="1" perimeter="0" name="S"/><constraint x="0.195" y="0.54" perimeter="0" name="W"/><constraint x="0.8" y="0.53" perimeter="0" name="E"/><constraint x="0.03" y="0.97" perimeter="0" name="SW"/><constraint x="0.97" y="0.97" perimeter="0" name="SE"/></connections><background><path><move x="8.32" y="73.66"/><line x="0" y="73.66"/><line x="0" y="89.86"/><curve x1="0" y1="94.82" x2="4.51" y2="98.88" x3="10.06" y3="98.88"/><line x="82.89" y="98.88"/><curve x1="88.43" y1="98.88" x2="92.95" y2="94.83" x3="92.95" y3="89.87"/><line x="92.95" y="73.66"/><line x="84.62" y="73.66"/><line x="84.62" y="88.14"/><curve x1="84.62" y1="89.96" x2="82.97" y2="91.43" x3="80.94" y3="91.43"/><line x="11.99" y="91.43"/><curve x1="9.95" y1="91.43" x2="8.32" y2="89.95" x3="8.32" y3="88.14"/><line x="8.32" y="73.66"/><close/><move x="74.47" y="84.49"/><line x="74.47" y="71.73"/><line x="18.4" y="53.21"/><line x="18.4" y="65.99"/><close/><move x="31.34" y="53.23"/><curve x1="29.41" y1="51.08" x2="16.59" y2="36.43" x3="29.52" y3="23.76"/><curve x1="40.3" y1="13.18" x2="41.87" y2="4.31" x3="38.24" y3="0"/><curve x1="38.24" y1="0" x2="65.67" y2="11.15" x3="49.54" y3="37"/><curve x1="46.78" y1="41.39" x2="39.88" y2="46.57" x3="43.49" y3="57.26"/><line x="31.34" y="53.23"/><close/><move x="48.18" y="56.74"/><curve x1="47.59" y1="54.57" x2="44.96" y2="49.85" x3="52.98" y3="40.45"/><curve x1="57.27" y1="35.44" x2="58.88" y2="28.02" x3="59.27" y3="25.23"/><curve x1="59.27" y1="25.23" x2="73.48" y2="33.28" x3="59.34" y3="52.99"/><line x="48.18" y="56.74"/><close/><move x="51.2" y="59.76"/><line x="74.47" y="52.08"/><line x="74.47" y="64.84"/><line x="70.52" y="66.13"/><close/><move x="41.68" y="77.95"/><line x="18.4" y="85.62"/><line x="18.4" y="72.85"/><line x="22.36" y="71.58"/><close/></path></background><foreground><fillstroke/></foreground></shape>',
  'ellipse': '<shape h="20" w="20" aspect="variable" strokewidth="inherit"><connections><constraint x="0.5" y="0" perimeter="0" name="N"/><constraint x="0.5" y="1" perimeter="0" name="S"/><constraint x="0" y="0.5" perimeter="0" name="W"/><constraint x="1" y="0.5" perimeter="0" name="E"/><constraint name="NW" perimeter="0" x="0.145" y="0.145"/><constraint name="SW" perimeter="0" x="0.145" y="0.855"/><constraint name="NE" perimeter="0" x="0.855" y="0.145"/><constraint name="SE" perimeter="0" x="0.855" y="0.855"/></connections><background><ellipse x="0" y="0" w="98" h="98"/></background></shape>',
}

# This implements a templated output of the network display config (.json) file for GUNNS networks.
class NetworkJsonTemplate:

  data = {}

  def __init__(self, data):
    self.data = data
    return

  # Use spotter data to build a dict of json info
  def spotterStyle(self, spotter):
    style_dict = {}
    # Key
    style_dict['key'] = spotter.attrib['id']
    # Text
    style_dict['text'] = spotter.attrib['label']
    # Size
    style_dict['size'] = spotter.find('./mxCell/mxGeometry').attrib['width'] + ' ' + spotter.find('./mxCell/mxGeometry').attrib['height']
    # Position
    style_dict['pos'] = spotter.find('./mxCell/mxGeometry').attrib['x'] + ' ' + spotter.find('./mxCell/mxGeometry').attrib['y']
    return style_dict

  # Use link data to build a dict of json info
  def linkStyle(self, link):
    style_dict = {}
    # Key
    style_dict['key'] = link.attrib['id']
    # Text
    style_dict['text'] = link.attrib['label']
    # Size
    style_dict['size'] = link.find('./mxCell/mxGeometry').attrib['width'] + ' ' + link.find('./mxCell/mxGeometry').attrib['height']
    # Convert style string
    angle = 0
    for s in link.find('./mxCell').attrib['style'].split(';'):
      if 'rotation' in s: angle += float(s[9:])
      elif 'direction' in s: 
        if 'south' in s: angle += 90
        elif 'west' in s: angle += 180
        elif 'north' in s: angle += 270
    # Angle
    style_dict['angle'] = str(angle)
    # Position
    angle = math.radians(angle)
    pos = [float(link.find('./mxCell/mxGeometry').attrib['x']), float(link.find('./mxCell/mxGeometry').attrib['y'])]
    pos[0] = round(pos[0] - (math.cos(angle) * float(link.find('./mxCell/mxGeometry').attrib['width'])/2 - math.sin(angle) * float(link.find('./mxCell/mxGeometry').attrib['height'])/2), 1)
    pos[1] = round(pos[1] - (math.sin(angle) * float(link.find('./mxCell/mxGeometry').attrib['width'])/2 + math.cos(angle) * float(link.find('./mxCell/mxGeometry').attrib['height'])/2), 1)
    style_dict['pos'] = str(pos[0] + float(link.find('./mxCell/mxGeometry').attrib['width'])/2) + ' ' + str(pos[1] + float(link.find('./mxCell/mxGeometry').attrib['height'])/2)
    # Convert shape
    shape = self.getShape(link.find('./mxCell').attrib['style'])
    style_dict['shape'], texts = self.convertShape(shape)
    style_dict['shapeText'] = '['
    for text in texts: style_dict['shapeText'] += text + ','
    if style_dict['shapeText'][-1] == ',': style_dict['shapeText'] = style_dict['shapeText'][:-1]
    style_dict['shapeText'] += ']'
    # Get connection points
    style_dict['connections'] = self.getConnections(shape)
    return style_dict
  
  # Use node data to build a dict of json info
  def nodeStyle(self, node):
    style_dict = {}
    # Key
    style_dict['key'] = node.attrib['id']
    # Text
    style_dict['text'] = node.attrib['label']
    # Key
    style_dict['size'] = node.find('./mxCell/mxGeometry').attrib['width'] + ' ' + node.find('./mxCell/mxGeometry').attrib['height']
    # Convert style string
    angle = 0
    for s in node.find('./mxCell').attrib['style'].split(';'):
      if 'rotation' in s: angle += float(s[9:])
      elif 'direction' in s: 
        if 'south' in s: angle += 90
        elif 'west' in s: angle += 180
        elif 'north' in s: angle += 270
    # Angle
    style_dict['angle'] = str(angle)
    # Position
    style_dict['pos'] = node.find('./mxCell/mxGeometry').attrib['x'] + ' ' + node.find('./mxCell/mxGeometry').attrib['y']
    # Port connection points
    style_dict['connections'] = '[{"portId":"N","x":"0.5","y":"0"}]' if style_dict['text'] == '' else \
                                '[{"portId":"N","x":"0.5","y":"0"},{"portId":"NE","x":"0.855","y":"0.145"},{"portId":"E","x":"1","y":"0.5"},{"portId":"SE","x":"0.855","y":"0.855"}, ' \
                                '{"portId":"S","x":"0.5","y":"1"},{"portId":"SW","x":"0.145","y":"0.855"},{"portId":"W","x":"0","y":"0.5"},{"portId":"NW","x":"0.145","y":"0.145"}]'
    return style_dict
  
  # Get the xml shape data from a style string
  def getShape(self, s):
    if 'ellipse' in s: return ET.fromstring(shapeMap['ellipse'])
    style_str = s[s.find('shape'):]
    style_str = style_str[:style_str.find(';')]
    if style_str[style_str.find('=')+1:] in shapeMap.keys(): shape = ET.fromstring(shapeMap[style_str[style_str.find('=')+1:]])
    elif 'stencil' in style_str: shape = ET.fromstring(compression.decompress(style_str[style_str.find('('):-1]))
    else: shape = None; print('Could not process style: ' + style_str)
    return shape
  
  # Transform link shape data into GoJS geometry string
  def convertShape(self, shape):
    geom_strs = []
    index = 0
    text = [{}]
    for layer in ['background', 'foreground']:
      if not shape.find('./' + layer): continue
      if len(geom_strs) > 0: geom_strs[-1] += 'z'
      for obj in shape.find('./' + layer):
        if obj.tag == 'stroke' or obj.tag == 'save' or obj.tag == 'restore': continue
        elif obj.tag == 'fontstyle': text[-1]['fontstyle'] = obj.attrib['style']; continue
        elif obj.tag == 'fontvariant': text[-1]['fontvariant'] = obj.attrib['variant']; continue
        elif obj.tag == 'fontweight': text[-1]['fontweight'] = obj.attrib['weight']; continue
        elif obj.tag == 'fontfamily': text[-1]['fontfamily'] = obj.attrib['family']; continue
        elif obj.tag == 'fontsize': text[-1]['fontsize'] = obj.attrib['size']; continue
        elif obj.tag == 'text': 
          text[-1]['tText'] = obj.attrib['str']
          text[-1]['tPos'] = obj.attrib['x'] + ' ' + obj.attrib['y']
          if 'rotation' in obj.attrib.keys(): text[-1]['tAngle'] = obj.attrib['rotation']
          if 'align' in obj.attrib.keys(): text[-1]['tAlign'] = obj.attrib['align']
          if 'valign' in obj.attrib.keys(): text[-1]['vAlign'] = obj.attrib['valign']
          text.append({})
          continue
        elif obj.tag == 'alpha' or obj.tag == 'fillcolor' or obj.tag == 'dashpattern' or obj.tag == 'dashed' or obj.tag == 'strokewidth' or obj.tag == 'linejoin': continue
        elif obj.tag == 'fill' or obj.tag == 'fillstroke': geom_strs[index-1] = 'F ' + geom_strs[index-1] + 'x '; continue
        elif obj.tag == 'rect' or obj.tag == 'roundrect' or obj.tag == 'ellipse': geom_strs.append(self.formatAttribs(obj.tag, obj.attrib))
        else:
          geom_strs.append('')
          for item in obj:
            if item.tag == 'move' or item.tag == 'line' or item.tag == 'curve' or item.tag == 'arc': geom_strs[index] += self.formatAttribs(item.tag, item.attrib)
            elif item.tag == 'close': geom_strs[index] = geom_strs[index][:-1] + 'z '; continue
            else: print('Tag not recognized: ' + item.tag)
        while len(geom_strs[index]) > 0 and geom_strs[index][-1] == ' ': geom_strs[index] = geom_strs[index][:-1]
        # if geom_strs[index][-1] != 'z': geom_strs[index] += 'z'
        index += 1
    final = ''
    for s in geom_strs: final += s + ' '
    text_strs = []
    for t in text[:-1]:
      text_strs.append('{"tText":"' + t['tText'] + '","tPos":"' + t['tPos'] + '"')
      if 'tAngle' in t.keys(): 
        if '-' in t['tAngle']: text_strs[-1] += ',"tAngle":"' + t['tAngle'][1:] + '"'
        else: text_strs[-1] += ',"tAngle":"-' + t['tAngle'][1:] + '"'
      if 'tAlign' in t.keys(): text_strs[-1] += ',"tAlign":"' + t['tAlign'] + '"'
      if 'vAlign' in t.keys(): text_strs[-1] += ',"vAlign":"' + t['vAlign'] + '"'
      tmp_str = ''
      if 'fontstyle' in t.keys(): tmp_str += t['fontstyle'] + ' '
      if 'fontvariant' in t.keys(): tmp_str += t['fontvariant'] + ' '
      if 'fontweight' in t.keys(): tmp_str += t['fontweight'] + ' '
      if 'fontsize' in t.keys(): tmp_str += t['fontsize'] + ' '
      if 'fontfamily' in t.keys(): tmp_str += t['fontfamily'] + ' '
      if tmp_str != '': text_strs[-1] += ',"tFont":"' + tmp_str[:-1] + '"'
      text_strs[-1] += '}'
    return final, text_strs
  
  def formatAttribs(self, svg, attribs):
    s = ''
    # SVG commands
    if svg == 'move':
      s += 'M' + attribs['x'] + ' ' + attribs['y'] + ' ' 
      return s
    elif svg == 'line':
      s += 'L' + attribs['x'] + ' ' + attribs['y'] + ' ' 
      return s
    elif svg == 'curve':
      try: s += 'C' + attribs['x'] + ' ' + attribs['y'] + ' ' + attribs['x1'] + ' ' + attribs['y1'] + ' ' + attribs['x2'] + ' ' + attribs['y2'] + ' '
      except: s += 'C' + attribs['x1'] + ' ' + attribs['y1'] + ' ' + attribs['x2'] + ' ' + attribs['y2'] + ' ' + attribs['x3'] + ' ' + attribs['y3'] + ' '
    elif svg == 'arc':
      try: s += 'A' + attribs['rx'] + ' ' + attribs['ry'] + ' ' + attribs['x-axis-rotation'] + ' ' + attribs['large-arc-flag'] + ' ' + attribs['sweep-flag'] + ' ' + attribs['x'] + ' ' + attribs['y'] + ' '
      except: s += 'A' + attribs['rx'] + ' ' + attribs['ry'] + ' 0 0 ' + attribs['sweep-flag'] + ' ' + attribs['x'] + ' ' + attribs['y'] + ' '
    # Shape aliases
    elif svg == 'rect':
      s += 'M' + attribs['x'] + ' ' + attribs['y'] + ' ' 
      s += 'L' + str(float(attribs['x']) + float(attribs['w'])) + ' ' + attribs['y'] + ' '
      s += str(float(attribs['x']) + float(attribs['w'])) + ' ' + str(float(attribs['y']) + float(attribs['h'])) + ' '
      s += attribs['x'] + ' ' + str(float(attribs['y']) + float(attribs['h'])) + ' '
      s += attribs['x'] + ' ' + attribs['y'] + 'z '
      return s
    elif svg == 'roundrect':
      s += 'M' + str(float(attribs['x']) + float(attribs['arcsize'])/2) + ' ' + attribs['y'] + ' ' 
      s += 'L' + str(float(attribs['x']) + float(attribs['w']) - float(attribs['arcsize'])/2) + ' ' + attribs['y'] + ' '
      s += 'A' + attribs['arcsize'] + ' ' + attribs['arcsize'] + ' 0 0 0 ' + str(float(attribs['x']) + float(attribs['w'])) + ' ' + str(float(attribs['y']) + float(attribs['arcsize'])/2) + ' '
      s += 'L' + str(float(attribs['x']) + float(attribs['w'])) + ' ' + str(float(attribs['y']) + float(attribs['h']) - float(attribs['arcsize'])/2) + ' '
      s += 'A' + attribs['arcsize'] + ' ' + attribs['arcsize'] + ' 0 0 0 ' + str(float(attribs['x']) + float(attribs['w']) - float(attribs['arcsize'])/2) + ' ' + str(float(attribs['y']) + float(attribs['h'])) + ' '
      s += 'L' + str(float(attribs['x']) + float(attribs['arcsize'])/2) + ' ' + str(float(attribs['y']) + float(attribs['h'])) + ' '
      s += 'A' + attribs['arcsize'] + ' ' + attribs['arcsize'] + ' 0 0 0 ' + attribs['x'] + ' ' + str(float(attribs['y']) + float(attribs['h']) - float(attribs['arcsize'])/2) + ' '
      s += 'L' + attribs['x'] + ' ' + str(float(attribs['y']) + float(attribs['arcsize'])/2) + ' '
      s += 'A' + attribs['arcsize'] + ' ' + attribs['arcsize'] + ' 0 0 0 ' + str(float(attribs['x']) + float(attribs['arcsize'])/2) + ' ' + attribs['y'] + 'z '
      return s
    elif svg == 'ellipse':
      rx = float(attribs['w'])/2
      ry = float(attribs['h'])/2
      s += 'M' + attribs['x'] + ' ' + str(float(attribs['y']) + ry) + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 1 ' + str(float(attribs['x']) + rx) + ' ' + attribs['y'] + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 1 ' + str(float(attribs['x']) + 2*rx) + ' ' + str(float(attribs['y']) + ry) + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 1 ' + str(float(attribs['x']) + rx) + ' ' + str(float(attribs['y']) + 2*ry) + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 1 ' + attribs['x'] + ' ' + str(float(attribs['y']) + ry) + 'z '
      return s
    else: print('SVG UNRECOGNIZED: ' + svg)
    return s
  
  def getConnections(self, shape):
    connections = '['
    for item in shape.find('./connections'):
      connections += '{"portId":"' + item.attrib['name'] + '","x":"' + item.attrib['x'] + '","y":"' + item.attrib['y'] + '"},'
    connections = connections[:-1] + ']'
    return connections

  # Use port style data to determine which link connector it should be attached to
  def portPos(self, port):
    compass = {'0.5,0':'N','0.855,0.145':'NE','1,0.5':'E','0.855,0.855':'SE','0.5,1':'S','0.145,0.855':'SW','0,0.5':'W','0.145,0.145':'NW'}
    from_obj = None
    to_obj = None
    # Determine which end of port is a link
    for link in self.data['links']:
      if link[-1].attrib['id'] == port[1]: from_obj = link[-1]
      elif link[-1].attrib['id'] == port[2]: to_obj = link[-1]
    is_from = True if from_obj else False
    link = from_obj if from_obj else to_obj
    # Determine node type
    is_gnd = False
    for gnd in self.data['gndNodes']:
      if is_from and gnd[-1].attrib['id'] == port[2]: is_gnd = True
    # Extract connection points from style string
    style_map = {}
    for s in port[3].find('./mxCell').attrib['style'].split(';'): 
      if '=' in s: style_map[s[:s.index('=')]] = s[s.index('=')+1:]
    # Get node connection point
    node_str = 'entry' if from_obj else 'exit'
    node_pt = 'N' if is_gnd else compass[style_map[node_str+'X']+','+style_map[node_str+'Y']]
    # Get link connection point
    link_shape = self.getShape(link.find('./mxCell').attrib['style'])
    link_str = 'exit' if from_obj else 'entry'
    for constraint in link_shape.find('./connections').iter('constraint'):
      if constraint.attrib['x'] == style_map[link_str+'X'] and constraint.attrib['y'] == style_map[link_str+'Y']: 
        return is_from, constraint.attrib['name'], node_pt
    return is_from, '0', node_pt

  def render(self):
    r =('{ "class": "go.GraphLinksModel",\n'
        '  "linkFromPortIdProperty": "fromPort",\n'
        '  "linkToPortIdProperty": "toPort",\n'
        '  "nodeDataArray": [\n')
    for spotter in self.data['spotters']:
      spotter_style = self.spotterStyle(spotter[-1])
      r = r + ('    {"key":"' + spotter_style['key'] + '","category":"Spotter","pos":"' + spotter_style['pos'] + '","text":"' + spotter_style['text'] + '","size":"' + spotter_style['size'] + '"},\n')
    for link in self.data['links']:
      link_style = self.linkStyle(link[-1])
      r = r + ('    {"key":"' + link_style['key'] + '","category":"Link","pos":"' + link_style['pos'] + 
                    '","size":"' + link_style['size'] + '","text":"' + link_style['text'] + '","angle":"' + link_style['angle'] + 
                    '","geometryString":"' + link_style['shape'] + '","connections":' + link_style['connections'] + 
                    ',"shapeText":' + link_style['shapeText'] + '},\n')
    for node in self.data['nodes']:
      node_style = self.nodeStyle(node[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Node","pos":"' + node_style['pos'] + 
               '","text":"' + node_style['text'] + '","size":"' + node_style['size'] + '","connections":' + 
               node_style['connections'] + '},\n')
    for gnd in self.data['gndNodes']:
      node_style = self.nodeStyle(gnd[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Ground","pos":"' + node_style['pos'] + 
               '","text":"","size":"' + node_style['size'] + '","angle":"' + node_style['angle'] + '","connections":' + 
               node_style['connections'] + '},\n')
    for ref in self.data['refNodes']:
      node_style = self.nodeStyle(ref[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Ref","pos":"' + node_style['pos'] + 
               '","text":"' + node_style['text'] + '","size":"' + node_style['size'] + '","connections":' + 
               node_style['connections'] + '},\n')
    r = r[:-2]
    r = r + ('\n],\n'
             '  "linkDataArray": [\n')
    for port in self.data['ports']:
      if port[1] == 'Ground' or port[2] == 'Ground': continue
      elif 'Ref' in port[1] or 'Ref' in port[2]: continue
      elif float(port[0]) > 1: continue
      is_from, link_port, node_port = self.portPos(port)
      r = r + ('    {"label":"' + port[0] + '","from":"')
      if is_from: r = r + (port[1] + '","fromPort":"' + link_port + '","to":"' + port[2] + '","toPort":"' + node_port + '"},\n')
      else: r = r + (port[1] + '","fromPort":"' + node_port + '","to":"' + port[2] + '","toPort":"' + link_port + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

