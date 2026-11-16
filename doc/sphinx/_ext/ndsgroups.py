"""Sphinx directive listing the members of a Doxygen group as links.

Breathe's ``doxygengroup`` renders full declarations of the group members.
In NDS3 the groups are cross-cutting topics whose members are already
documented with their classes in the API reference, so rendering them again
creates duplicate C++ declarations. This directive instead emits a list of
cross references (with the brief descriptions) to those declarations:

    .. ndsgroupmembers:: logging
"""

import os
import xml.etree.ElementTree as ET

from docutils import nodes
from docutils.statemachine import StringList
from sphinx.util.docutils import SphinxDirective

ROLES = {
    'function': 'cpp:func',
    'enum': 'cpp:enum',
    'typedef': 'cpp:type',
    'variable': 'cpp:var',
    'define': 'c:macro',
}


def _text(element):
    if element is None:
        return ''
    return ' '.join(''.join(element.itertext()).split())


class NdsGroupMembers(SphinxDirective):
    required_arguments = 1

    def run(self):
        config = self.env.config
        xml_dir = config.breathe_projects[config.breathe_default_project]
        path = os.path.join(xml_dir, 'group__%s.xml' % self.arguments[0])
        self.env.note_dependency(path)
        compound = ET.parse(path).getroot().find('compounddef')

        lines = []
        for inner in compound.findall('innerclass'):
            lines.append('* :cpp:class:`%s`' % inner.text)
        for member in compound.iter('memberdef'):
            role = ROLES.get(member.get('kind'))
            if role is None:
                continue
            name = member.findtext('qualifiedname') or member.findtext('name')
            if role == 'c:macro':
                name = member.findtext('name')
            brief = _text(member.find('briefdescription'))
            line = '* :%s:`%s`' % (role, name)
            if brief:
                line += ' — ' + brief
            lines.append(line)

        container = nodes.container()
        self.state.nested_parse(StringList(lines, source=path), 0, container)
        return container.children


def setup(app):
    app.add_directive('ndsgroupmembers', NdsGroupMembers)
    return {'parallel_read_safe': True, 'parallel_write_safe': True}
