#!/bin/bash
# Exit on error
set -e
# Print each line
set -v


if [ ! -f /etc/redhat-release ]; then
    # Required build tools
    apt-get -y install cmake
else
    yum install -y cmake
    yum install -y dpkg-dev
    yum install -y lcov
fi

# get installation directory from command line argument
DEFAULT_TARG_ARCH=x86_64-linux
if [ -z "$*" ]; then TARG_ARCH=$DEFAULT_TARG_ARCH; else TARG_ARCH=$1;fi

# Build nds
NDS_PATH=$PWD

if [ "$TARG_ARCH" == "x86_64-w64-mingw32" ]; then
    # see https://github.com/randombit/botan/issues/2039
    update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix
    # CROSS_COMPILE used by dlfcn-win32/tools/ci-build.sh
    export CROSS_COMPILE=$TARG_ARCH
    TOOLCHAIN_FILE=$NDS_PATH/CMake/TC-mingw64.cmake
    if [ ! -d ./dlfcn-win32/ci-build-shared-x86_64-w64-mingw32 ]; then
        pushd ./dlfcn-win32
        chmod +x ./tools/ci-build.sh
        ./tools/ci-build.sh
        popd
    fi
elif [ "$TARG_ARCH" == "i686-w64-mingw32" ]; then
    # see https://github.com/randombit/botan/issues/2039
    update-alternatives --set i686-w64-mingw32-g++ /usr/bin/i686-w64-mingw32-g++-posix
    export CROSS_COMPILE=$TARG_ARCH
    TOOLCHAIN_FILE=$NDS_PATH/CMake/TC-mingw32.cmake
    if [ ! -d ./dlfcn-win32/ci-build-shared-i686-w64-mingw32 ]; then
        pushd ./dlfcn-win32
        chmod +x ./tools/ci-build.sh
        ./tools/ci-build.sh
        popd
    fi
fi


NDS_BUILD_PATH=$NDS_PATH/build/$TARG_ARCH
NDS_EXAMPLES_BUILD_PATH=$NDS_PATH/doc/examples/build/$TARG_ARCH
NDS_TESTS_BUILD_PATH=$NDS_PATH/tests/build/$TARG_ARCH

# Build nds
mkdir -p $NDS_BUILD_PATH
pushd $NDS_BUILD_PATH
cmake ../../CMake -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE
if [ "$TARG_ARCH" == "x86_64-linux" ]; then
    make install
else
    make
fi
popd

#build nds examples code - it's required by nds-epics
mkdir -p $NDS_EXAMPLES_BUILD_PATH
pushd $NDS_EXAMPLES_BUILD_PATH
cmake ../../CMake -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DLIBRARY_LOCATION=$NDS_BUILD_PATH
if [ "$TARG_ARCH" == "x86_64-linux" ]; then
    make install
else
    make
fi
popd

#build nds tests
#mkdir -p $NDS_TESTS_BUILD_PATH
#pushd $NDS_TESTS_BUILD_PATH
#cmake ../../CMake -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DLIBRARY_LOCATION=$NDS_BUILD_PATH
#make
#popd

#lcov --directory . --capture --output-file coverage.info
#lcov --remove coverage.info '/usr/*' 'googletest-src/*' 'tests/*' --output-file coverage.info
#lcov --list coverage.info
#coveralls-lcov --repo-token "${COVERALLS_TOKEN}" coverage.info
