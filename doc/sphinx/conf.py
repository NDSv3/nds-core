# Sphinx configuration for the NDS3 documentation.
#
# The content comes from the Doxygen comments in the public headers and the
# pages in doc/api; Doxygen generates XML from them, which Breathe renders.
# Doxygen is run from here, so a plain sphinx-build is all that is needed
# (locally, in CI and on Read the Docs):
#
#     sphinx-build -b html doc/sphinx doc/_build/html

import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
DOXYGEN_OUT = os.path.join(ROOT, 'doc', '_build', 'doxygen')

sys.path.insert(0, os.path.join(HERE, '_ext'))
import doxyxml  # noqa: E402


def _project_version():
    with open(os.path.join(ROOT, 'CMakeLists.txt')) as f:
        match = re.search(r'project\s*\(\s*\S+\s+VERSION\s+([0-9.]+)', f.read())
    return match.group(1) if match else 'unknown'


def _run_doxygen():
    os.makedirs(DOXYGEN_OUT, exist_ok=True)
    subprocess.run(['doxygen', os.path.join('doc', 'Doxyfile')],
                   cwd=ROOT, check=True)
    return doxyxml.fix_xml(os.path.join(DOXYGEN_OUT, 'xml'))


def _write_example_pages(examples):
    """Generate one page per Doxygen example into examples/."""
    out_dir = os.path.join(HERE, 'examples')
    os.makedirs(out_dir, exist_ok=True)
    wanted = set()
    for refid, name in examples:
        filename = name.replace('/', '_').replace('.', '_') + '.rst'
        wanted.add(filename)
        title = os.path.basename(name)
        content = '%s\n%s\n\n.. doxygenpage:: %s\n   :content-only:\n' % (
            title, '=' * len(title), name)
        path = os.path.join(out_dir, filename)
        # Leave unchanged files alone, so Sphinx doesn't rebuild them
        if not os.path.exists(path) or open(path).read() != content:
            with open(path, 'w') as f:
                f.write(content)
    for filename in os.listdir(out_dir):
        if filename not in wanted:
            os.remove(os.path.join(out_dir, filename))


_write_example_pages(_run_doxygen())

# -- Project information -----------------------------------------------------

project = 'NDS3'
author = 'Cosylab and ITER Organization'
copyright = '2015-2022 Cosylab, 2017-2026 ITER Organization'
release = _project_version()
version = '.'.join(release.split('.')[:2])

# -- General configuration ---------------------------------------------------

extensions = [
    'breathe',
    'sphinx.ext.todo',
    'ndsgroups',
]

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

primary_domain = 'cpp'
highlight_language = 'cpp'

# Some code blocks in the Doxygen pages are ini files or shell sessions
suppress_warnings = ['misc.highlighting_failure']

# Standard types that have no documentation to link to (for sphinx-build -n)
nitpick_ignore = [
    ('cpp:identifier', name)
    for name in ('nds', 'std', 'timespec', 'size_t', 'int32_t', 'FILE')
]

# Breathe
breathe_projects = {'nds3': os.path.join(DOXYGEN_OUT, 'xml')}
breathe_default_project = 'nds3'
breathe_default_members = ('members',)
breathe_show_include = True
breathe_show_define_initializer = True
breathe_order_parameters_first = False

# The export macros are expanded away by Doxygen; these tell the C++ domain
# about the ones it may still see in signatures.
cpp_id_attributes = ['NDS3_API', 'NDS3_DRIVER_API']

# -- HTML output -------------------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_title = 'NDS3 %s documentation' % release
html_static_path = ['_static']
html_css_files = ['custom.css']
html_theme_options = {
    'navigation_depth': 3,
    'collapse_navigation': False,
    'prev_next_buttons_location': 'both',
}
html_context = {
    'display_github': True,
    'github_user': 'NDSv3',
    'github_repo': 'nds-core',
    'github_version': 'main',
    'conf_py_path': '/doc/sphinx/',
}
