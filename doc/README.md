# NDS v3 documentation

The project overview, build instructions for Linux and Windows, test
instructions and usage notes now live in the top-level
[`README.md`](../README.md).

This directory contains:

| File | Contents |
| --- | --- |
| [`CHANGELOG.md`](CHANGELOG.md) | Release history |
| [`CONTRIBUTING.md`](CONTRIBUTING.md) | Coding style, commit conventions, PR workflow |
| [`MAINTENANCE.md`](MAINTENANCE.md) | Versioning and maintenance policy |
| `Doxyfile` | Doxygen configuration, generating XML into `_build/doxygen` |
| `sphinx/` | Sphinx sources rendering the Doxygen XML via Breathe (`conf.py` runs Doxygen) |
| `sphinx/_ext/` | Sphinx extensions adapting the Doxygen XML to Breathe |
| `api/` | Hand-written pages included in the generated API reference |
| `images/` | Diagrams referenced by the documentation |

See [Documentation](../README.md#documentation) in the top-level README for how
to build the API reference.
