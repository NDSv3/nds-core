# NDS v3 — Nominal Device Support

NDS (Nominal Device Support) is a C++ library for writing device support for a
variety of control systems (CS) while hiding the details of the chosen control
system, so that driver authors can focus on device functionality.

A driver written against NDS is a plugin: it is compiled once against `libnds3`
and then loaded at runtime next to a control-system plugin (EPICS, PVXS, Tango,
…), without the driver source having to know which one it is talking to.

This repository contains **nds-core**, the control-system-agnostic core
library, hosted in the [NDSv3](https://github.com/NDSv3) GitHub organization as
[NDSv3/nds-core](https://github.com/NDSv3/nds-core).

Control-system bindings and device drivers live in separate repositories in the
same organization:

| Repository | Contents |
| --- | --- |
| [nds-core](https://github.com/NDSv3/nds-core) | This library — the NDSv3 core |
| [nds3-epics](https://github.com/NDSv3/nds3-epics) | EPICS control-system support |
| [nds-adq](https://github.com/NDSv3/nds-adq) | Driver for Teledyne ADQ digitizers |
| [nds3-core](https://github.com/NDSv3/nds3-core) | Clone of the original Cosylab NDS3, kept for history |

Further bindings and drivers (`nds-pvxs`, `nds-sdn`, `nds-irio`, `nds-nidaqx`,
`nds-nisync`, …) are maintained separately and not yet published in the
organization.

## Features

- Organizes a device into a tree-like structure of devices, channels,
  attributes (PVs), state machines, etc.
- Supports both data pull mode (passive scanning on EPICS, polling on Tango)
  and push mode (interrupt on EPICS, push on Tango).
- Driver code uses standard C++ types and Unix EPOCH timing to communicate with
  the library.
- On EPICS there is no need to supply separate `.db` files (though you still
  can if you want).
- Supplies a hierarchical state machine.
- Supplies specialized nodes for data acquisition, waveform generation,
  triggering and clocking, timing, routing, digital I/O, firmware handling,
  health monitoring and data multiplexing.
- Guaranteed binary compatibility between minor versions, so existing
  installations can be upgraded without recompiling the device support.

## Repository layout

| Path | Contents |
| --- | --- |
| `include/nds3/` | Public headers (the installed API) |
| `include/nds3/impl/` | Implementation headers, not part of the stable API |
| `src/` | Library implementation |
| `examples/nds-example/` | Reference driver exercising most node types |
| `examples/exampleDriver/` | Driver built on a simulated signal source |
| `examples/nodesTest/` | One small driver per node type, used by the tests |
| `tests/unit/` | GoogleTest suite for the core library |
| `tests/unit-example/` | GoogleTest suite for `examples/nds-example` |
| `tests/testControlSystem/` | Minimal in-process control system used by the tests |
| `doc/` | Doxygen/Sphinx sources, changelog, contribution and maintenance policy |

## Requirements

- **CMake** 3.15 or newer
- A **C++11** compiler (GCC 4.9+ is recommended because of
  [GCC bug 57869](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57869); MSVC on
  Windows — CI builds with the toolchain on GitHub's `windows-latest` image)
- A **threads** implementation (`find_package(Threads REQUIRED)`)
- **GoogleTest** — optional. If it is not installed, the build downloads
  GoogleTest v1.14.0 via CMake `FetchContent`, which requires network access at
  configure time. Only needed when `BUILD_TESTING=ON`.

## Building on Linux

```bash
cmake -B build -S . -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON
cmake --build build -j$(nproc)
```

To produce an optimized build, pass a build type (this is a single-config
generator, so the build type is chosen at configure time):

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

If GoogleTest is not installed and the machine has no network access,
configure with `-DBUILD_TESTING=OFF`.

### Installing

```bash
cmake --install build --prefix /usr/local
```

This installs (for Windows, see [Building on Windows](#building-on-windows)):

- `lib/libnds3.so` — shared library
- `lib/libnds3.a` — static library
- `include/nds3/` — public headers

> **Note:** the build does not yet export a CMake package configuration, so
> `find_package(nds3)` is not available. Link against the library directly with
> `-lnds3` and add the install prefix to your include and library search paths.

## Building on Windows

NDS uses the Win32 API directly on Windows, so apart from Visual Studio and
CMake nothing has to be installed:

```powershell
cmake -B build -S . -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON -A x64
cmake --build build --config Release
```

Notes specific to Windows:

- Visual Studio is a *multi-config* generator, so the configuration is selected
  at build time with `--config`, not at configure time with
  `CMAKE_BUILD_TYPE`. The same applies to `ctest -C Release` and
  `cmake --install build --config Release`.
- `cmake --install` uses the directory layout of an EPICS module, so that
  EPICS-based modules can build against NDS like against any other module:

  | Path | Contents |
  | --- | --- |
  | `bin/windows-x64/nds3.dll` | Shared library |
  | `lib/windows-x64/nds3.lib` | Import library of the DLL |
  | `lib/windows-x64-static/nds3.lib` | Static library |
  | `include/nds3/` | Public headers |

  Both libraries are called `nds3.lib`, as the EPICS build system expects, so
  the separate directories are what keeps them apart. Installing the Debug
  configuration appends `-debug` to the architecture, as EPICS does
  (`bin/windows-x64-debug`, `lib/windows-x64-debug`,
  `lib/windows-x64-static-debug`), so it can be installed next to Release.
  The architecture name defaults to `windows-x64` (`win32-x86` for a 32-bit
  build); set `-DNDS3_EPICS_ARCH=...` to change it.
- The DLL uses the DLL C runtime (`/MD`, `/MDd`), the static library the static
  one (`/MT`, `/MTd`), matching EPICS shared and static builds. Code linking
  the static library has to use the same runtime. To build the static library
  for another runtime, set `-DNDS3_STATIC_MSVC_RUNTIME_LIBRARY=...` to a value
  of CMake's
  [`MSVC_RUNTIME_LIBRARY`](https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html),
  e.g. `MultiThreaded$<$<CONFIG:Debug>:Debug>DLL`.

## Build options

| Option | Default | Effect |
| --- | --- | --- |
| `BUILD_TESTING` | `ON` | Build `tests/` and register the CTest targets |
| `BUILD_EXAMPLES` | `ON` | Build the drivers under `examples/` |

Both are standard CMake cache variables, e.g. `-DBUILD_EXAMPLES=OFF`.

Two library targets are always produced: `nds3` (shared) and `nds3_static`
(static, installed as `libnds3.a` on Linux and as `nds3.lib` in
`lib/windows-x64-static` on Windows).

## Running the tests

Tests are registered with CTest and require `BUILD_TESTING=ON` (the default):

```bash
cd build
ctest --output-on-failure
```

On Windows add the configuration: `ctest --output-on-failure -C Release`.

Two CTest targets are registered:

| Target | Source | Cases |
| --- | --- | --- |
| `nds3-unit-tests` | `tests/unit/` | 129 GoogleTest cases across 22 suites |
| `nds3-unit-example` | `tests/unit-example/` | Tests for the `nds-example` driver |

> **The full suite takes roughly 12–13 minutes**, almost all of it in
> `nds3-unit-tests`, which exercises timing- and thread-sensitive behaviour.
> Do not wrap the run in a short timeout. `ctest -j` does not help much, since
> CTest only sees two targets.

For a faster edit/test loop, run the GoogleTest binaries directly and filter:

```bash
./build/tests/unit/nds3-unit-tests --gtest_list_tests
./build/tests/unit/nds3-unit-tests --gtest_filter='testPVs.*'
./build/tests/unit/nds3-unit-tests --gtest_filter='testStateMachine.*:testThreads.*'
```

`tests/unit/test.py` is a soak-test driver that re-runs each test 200–2000
times to flush out intermittent failures, recording any failure in an
`ErrorFile` next to the script.

## Using NDS in a driver

A driver is a shared library that links against `nds3` and registers itself
with the `NDS_DEFINE_DRIVER` macro:

```cpp
#include <nds3/nds.h>

class MyDevice
{
public:
    MyDevice(nds::Factory& factory,
             const std::string& deviceName,
             const nds::namedParameters_t& parameters);
    // ...
};

NDS_DEFINE_DRIVER(MyDevice, MyDevice)
```

`examples/nds-example/nds-example.cpp` is a complete worked example covering
PVs, a state machine, data acquisition and initializer callbacks.

### Plugin discovery at runtime

At startup NDS scans for control-system and device plugins in, in order:

1. the current directory (`.`)
2. `LD_LIBRARY_PATH`
3. `NDS_CONTROL_SYSTEMS` (control systems) and `NDS_DEVICES` /
   `NDS_PLUGIN_PATH` (devices)

Entries in these variables may be separated by `:`, `;` or spaces, and may be
quoted.

Plugins are recognized by filename: device plugins must be named
`lib<something>NdsDevice.so`, control-system plugins `lib<something>NdsControlSystem.so`
or `lib<something>NdsControlSystem`.

> **Note:** the `.so` filename pattern is matched on every platform, Windows
> included — a Windows plugin is a DLL named like a Linux shared object.

Naming rules for the generated PV names are configurable; see
`examples/naming_rules.txt` for a documented example of the ini-file format.

## Documentation

The NDSv3 project is hosted at <https://github.com/NDSv3>; this library is
[NDSv3/nds-core](https://github.com/NDSv3/nds-core).

There is no published API reference site for the project yet. The legacy
reference at <https://cosylab.github.io/nds3/> describes the original Cosylab
NDS3 and predates this repository, so build the reference locally instead.

The documentation is written as Doxygen comments in the public headers and
as Doxygen pages in `doc/api`. Doxygen turns them into XML, which Sphinx
renders via Breathe into a web site in the Read the Docs style. `conf.py`
runs Doxygen, so Doxygen just has to be installed:

```bash
sudo apt-get install doxygen
pip install -r doc/sphinx/requirements.txt

sphinx-build -b html doc/sphinx doc/_build/html
```

The result is at `doc/_build/html/index.html`. The repository also has a
`.readthedocs.yaml`, so the site can be published on Read the Docs by
importing the project there.

## Continuous integration

`.github/workflows/ci.yml` builds and tests on Linux and Windows and builds the
documentation on every push and on pull requests against `master`/`main`. The
build commands in this README mirror what CI runs.

## Contributing

See [`doc/CONTRIBUTING.md`](doc/CONTRIBUTING.md) for coding style, commit
message conventions and the pull request workflow. The project targets C++11;
please avoid GNU/MSVC-specific extensions and features from later standards.

Versioning follows [semantic versioning](http://www.semver.org) pragmatically —
see [`doc/MAINTENANCE.md`](doc/MAINTENANCE.md). Changes are recorded in
[`doc/CHANGELOG.md`](doc/CHANGELOG.md).

## License

GNU General Public License, version 3. See [`license.txt`](license.txt).
