"""Post-processing of the Doxygen XML before Breathe reads it.

Doxygen moves members tagged with ``@ingroup`` out of their class or
namespace into the group's XML file, leaving only a ``<member refid=...>``
reference behind. Breathe does not follow those references, so the members
would be missing from the class documentation. NDS3 uses the groups as
cross-cutting topics, so the members are copied back into their sections
here, and the topic pages link to them (see ndsgroups.py).

The namespace's inner classes are also removed from the namespace XML, so
that ``doxygennamespace`` renders just the namespace-level members; the
classes are documented on their own pages.

Breathe cannot render Doxygen examples (``@example``), so they are turned
into pages, which ``doxygenpage`` can render. References to header files,
which have no pages of their own, become plain text, and only the HTML
variants of images are kept.
"""

import copy
import glob
import os
import xml.etree.ElementTree as ET


def _write(tree, path):
    tree.write(path, encoding='UTF-8', xml_declaration=True)


def fix_xml(xml_dir):
    grouped = {}
    for path in glob.glob(os.path.join(xml_dir, 'group__*.xml')):
        for memberdef in ET.parse(path).getroot().iter('memberdef'):
            grouped[memberdef.get('id')] = memberdef

    for pattern in ('class*.xml', 'struct*.xml', 'namespace*.xml', '*_8h.xml'):
        for path in glob.glob(os.path.join(xml_dir, pattern)):
            tree = ET.parse(path)
            compound = tree.getroot().find('compounddef')
            changed = False

            for sectiondef in compound.findall('sectiondef'):
                for index, child in enumerate(list(sectiondef)):
                    if child.tag == 'member' and child.get('refid') in grouped:
                        sectiondef.remove(child)
                        sectiondef.insert(
                            index, copy.deepcopy(grouped[child.get('refid')]))
                        changed = True

            if compound.get('kind') == 'namespace':
                for inner in compound.findall('innerclass'):
                    compound.remove(inner)
                    changed = True

            if changed:
                _write(tree, path)

    examples = _examples_to_pages(xml_dir)
    _fix_refs_and_images(xml_dir)
    return examples


def _examples_to_pages(xml_dir):
    """Turn the example compounds into pages; return their (id, name)."""
    examples = []
    index_path = os.path.join(xml_dir, 'index.xml')
    index = ET.parse(index_path)
    for compound in index.getroot().findall('compound'):
        if compound.get('kind') != 'example':
            continue
        compound.set('kind', 'page')
        refid = compound.get('refid')
        examples.append((refid, compound.findtext('name')))

        path = os.path.join(xml_dir, refid + '.xml')
        tree = ET.parse(path)
        compounddef = tree.getroot().find('compounddef')
        compounddef.set('kind', 'page')
        title = ET.Element('title')
        title.text = compounddef.findtext('compoundname')
        compounddef.insert(1, title)
        _write(tree, path)
    _write(index, index_path)
    return examples


def _fix_refs_and_images(xml_dir):
    """Unlink references to header files; drop images not meant for HTML.

    Breathe renders every ``@image`` regardless of its output format, so the
    LaTeX variants would show up next to the HTML ones.
    """
    for path in glob.glob(os.path.join(xml_dir, '*.xml')):
        tree = ET.parse(path)
        parents = {child: parent for parent in tree.getroot().iter()
                   for child in parent}
        refs = [ref for ref in tree.getroot().iter('ref')
                if ref.get('kindref') == 'compound'
                and ref.get('refid', '').endswith('_8h')]
        for ref in refs:
            _unwrap(parents[ref], ref)
        images = [image for image in tree.getroot().iter('image')
                  if image.get('type') != 'html']
        for image in images:
            image.text = None
            _unwrap(parents[image], image)
        if refs or images:
            _write(tree, path)


def _unwrap(parent, element):
    """Replace element by its text, keeping it in the parent's text flow."""
    index = list(parent).index(element)
    text = (element.text or '') + (element.tail or '')
    if index == 0:
        parent.text = (parent.text or '') + text
    else:
        previous = parent[index - 1]
        previous.tail = (previous.tail or '') + text
    parent.remove(element)
