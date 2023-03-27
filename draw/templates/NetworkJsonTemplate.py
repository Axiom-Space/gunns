#!/usr/bin/python
#
# @revs_title
# @revs_begin
# @rev_entry(Kyle Fondon, Axiom Space, GUNNS, February 2023, --, Initial implementation.}
# @revs_end
#
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
    pos = [float(spotter.find('./mxCell/mxGeometry').attrib['x']), float(spotter.find('./mxCell/mxGeometry').attrib['y'])]
    style_dict['pos'] = str(pos[0] + float(spotter.find('./mxCell/mxGeometry').attrib['width'])/2) + ' ' + str(pos[1] + float(spotter.find('./mxCell/mxGeometry').attrib['height'])/2)
    print(spotter)
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
    # Position
    pos = [float(link.find('./mxCell/mxGeometry').attrib['x']), float(link.find('./mxCell/mxGeometry').attrib['y'])]
    style_dict['pos'] = str(pos[0] + float(link.find('./mxCell/mxGeometry').attrib['width'])/2) + ' ' + str(pos[1] + float(link.find('./mxCell/mxGeometry').attrib['height'])/2)
    # Convert style string
    for s in link.find('./mxCell').attrib['style'].split(';'):
      if 'rotation' in s: style_dict['angle'] = s[9:]
      else: style_dict['angle'] = '0'
    # Convert shape
    shape = self.getShape(link.find('./mxCell').attrib['style'])
    style_dict['shape'] = self.convertShape(shape)
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
    # Position
    style_dict['pos'] = node.find('./mxCell/mxGeometry').attrib['x'] + ' ' + node.find('./mxCell/mxGeometry').attrib['y']
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
    for layer in ['background', 'foreground']:
      if not shape.find('./' + layer): continue
      if len(geom_strs) > 0: geom_strs[-1] += 'z'
      for obj in shape.find('./' + layer):
        if obj.tag == 'stroke' or obj.tag == 'save' or obj.tag == 'restore': continue
        elif obj.tag == 'fontfamily' or obj.tag == 'fontsize' or obj.tag == 'fontstyle' or obj.tag == 'text': continue
        elif obj.tag == 'alpha' or obj.tag == 'fillcolor' or obj.tag == 'dashpattern' or obj.tag == 'dashed' or obj.tag == 'strokewidth' or obj.tag == 'linejoin': continue
        elif obj.tag == 'fill' or obj.tag == 'fillstroke': geom_strs[index-1] = 'F ' + geom_strs[index-1] + 'x '; continue
        elif obj.tag == 'rect' or obj.tag == 'roundrect' or obj.tag == 'ellipse': geom_strs.append(self.formatAttribs(obj.tag, obj.attrib))
        else:
          geom_strs.append('')
          for item in obj:
            if item.tag == 'move' or item.tag == 'line' or item.tag == 'curve' or item.tag == 'arc': geom_strs[index] += self.formatAttribs(item.tag, item.attrib)
            elif item.tag == 'close': geom_strs[index] = geom_strs[index][:-1] + 'z '; continue
            else: print('Tag not recognized: ' + item.tag)
        while geom_strs[index][-1] == ' ': geom_strs[index] = geom_strs[index][:-1]
        # if geom_strs[index][-1] != 'z': geom_strs[index] += 'z'
        index += 1
    final = ''
    for s in geom_strs: final += s + ' '
    return final
  
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
      try: s += 'A' + attribs['x'] + ' ' + attribs['y'] + ' ' + attribs['x-axis-rotation'] + ' ' + attribs['large-arc-flag'] + ' ' + attribs['sweep-flag'] + ' ' + attribs['x'] + ' ' + attribs['y'] + ' '
      except: s += 'A' + attribs['x'] + ' ' + attribs['y'] + ' 0 0 ' + attribs['sweep-flag'] + ' ' + attribs['x'] + ' ' + attribs['y'] + ' '
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
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 0 ' + str(float(attribs['x']) + rx) + ' ' + attribs['y'] + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 0 ' + str(float(attribs['x']) + 2*rx) + ' ' + str(float(attribs['y']) + ry) + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 0 ' + str(float(attribs['x']) + rx) + ' ' + str(float(attribs['y']) + 2*ry) + ' '
      s += 'A' + str(rx) + ' ' + str(ry) + ' 0 0 0 ' + attribs['x'] + ' ' + str(float(attribs['y']) + ry) + 'z '
      return s
    else: print('SVG UNRECOGNIZED: ' + svg)
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
      if link[-1].attrib['id'] == port[1]: from_obj = link[-1]
      elif link[-1].attrib['id'] == port[2]: to_obj = link[-1]
    is_from = True if from_obj else False
    obj = from_obj if from_obj else to_obj
    e_str = 'exit' if from_obj else 'entry'
    port_pt = []
    # Extract connection point from style string
    style_str = port[3].find('./mxCell').attrib['style']
    style_str = style_str[style_str.find(e_str + 'X=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Y=')+len(e_str)+2:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dx=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    style_str = style_str[style_str.find(e_str + 'Dy=')+len(e_str)+3:]
    port_pt.append(style_str[:style_str.find(';')])
    obj_shape = self.getShape(obj.find('./mxCell').attrib['style'])
    for constraint in obj_shape.find('./connections').iter('constraint'):
      if constraint.attrib['x'] == port_pt[0] and constraint.attrib['y'] == port_pt[1]: 
        return is_from, constraint.attrib['name']
    return is_from, '0'

  def render(self):
    r =('{ "class": "go.GraphLinksModel",\n'
        '  "linkFromPortIdProperty": "fromPort",\n'
        '  "linkToPortIdProperty": "toPort",\n'
        '  "nodeDataArray": [\n')
    for spotter in self.data['spotters']:
      print("SPOTTER")
      spotter_style = self.spotterStyle(spotter[-1])
      r = r + ('    {"key":"' + spotter_style['key'] + '","category":"Spotter","pos":"' + spotter_style['pos'] + '","text":"' + spotter_style['text'] + '","size":"' + spotter_style['size'] + '"},\n')
    for link in self.data['links']:
      link_style = self.linkStyle(link[-1])
      r = r + ('    {"key":"' + link_style['key'] + '","category":"Link","pos":"' + link_style['pos'] + 
                    '","size":"' + link_style['size'] + '","text":"' + link_style['text'] + '","angle":"' + link_style['angle'] + 
                    '","geometryString":"' + link_style['shape'] + '","itemArray":' + link_style['connections'] + '},\n')
    for node in self.data['nodes']:
      node_style = self.nodeStyle(node[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Node","pos":"' + node_style['pos'] + '","text":"' + node_style['text'] + '","size":"' + node_style['size'] + '"},\n')
    for gnd in self.data['gndNodes']:
      node_style = self.nodeStyle(gnd[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Ground","pos":"' + node_style['pos'] + '","text":"","size":"' + node_style['size'] + '"},\n')
    for ref in self.data['refNodes']:
      node_style = self.nodeStyle(ref[-1])
      r = r + ('    {"key":"' + node_style['key'] + '","category":"Ref","pos":"' + node_style['pos'] + '","text":"' + node_style['text'] + '","size":"' + node_style['size'] + '"},\n')
    r = r[:-2]
    r = r + ('\n],\n'
             '  "linkDataArray": [\n')
    for port in self.data['ports']:
      if port[1] == 'Ground' or port[2] == 'Ground': continue
      elif 'Ref' in port[1] or 'Ref' in port[2]: continue
      elif float(port[0]) > 1: continue
      is_from, port_name = self.portPos(port)
      r = r + ('    {"label":"' + port[0] + '","from":"')
      if is_from: r = r + (port[1] + '","fromPort":"' + port_name + '","to":"' + port[2] + '","toPort":"' + port[0] + '"},\n')
      else: r = r + (port[1] + '","fromPort":"' + port[0] + '","to":"' + port[2] + '","toPort":"' + port_name + '"},\n')
    r = r[:-2]
    r = r + ('\n  ]\n'
             '}')

    return r.encode('ascii', 'ignore').decode('ascii')

