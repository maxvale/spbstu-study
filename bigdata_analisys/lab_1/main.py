from xml.etree import ElementTree as Et


def obfuscate(source: str) -> str:
    index = 1
    items = list()
    for elem in source:
        items.append(str(chr(ord(elem) + index)))
        index += 1
    result = ''.join(items)
    return result


def deobfuscate(source: str) -> str:
    index = 1
    items = list()
    for elem in source:
        items.append(str(chr(ord(elem) - index)))
        index += 1
    result = ''.join(items)
    return result


tree = Et.parse('input_data.xml')
root = tree.getroot()

for element in root.iter('employee'):
    for item in element:
        item.text = obfuscate(item.text)

tree.write('obfuscated_data.xml')

tree = Et.parse('obfuscated_data.xml')
root = tree.getroot()

for element in root.iter('employee'):
    for item in element:
        item.text = deobfuscate(item.text)

tree.write('deobfuscated_data.xml')
