#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Authors       : James Blair (National Instruments)
#                 Mike Hoogendyk (National Instruments)
#
# Copyright (c) : 2013-2017 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

NDSLIBDIR:=$(realpath ../../build/lib)
# Path from which the device source file will be obtained.
EXAMPLESDIR:=$(realpath ../../doc/examples/nodesTest)
EXAMPLESHEADERS:=$(wildcard $(EXAMPLESDIR)/*.h))
# Necessary for make to find the source files in a different directory.
VPATH =$(EXAMPLESDIR)


INCLUDES := -I../include -I$(EXAMPLESDIR) -I../include
LIBPATH := -L../lib -Wl,-rpath=$(NDSLIBDIR)


#############################################################################


LIBNAME=nds3-testcontrolsystem

#Sources
SOURCE_DIR = .
SOURCES += $(SOURCE_DIR)/ndsTestFactory.cpp
SOURCES += $(SOURCE_DIR)/ndsTestInterface.cpp
SOURCES += $(SOURCE_DIR)/ndsTestUtils.cpp

TARGET=../../..
LIBRARY_DIR=$(TARGET)/lib
OBJECT_DIR=$(SOURCE_DIR)/.obj
SHAREDLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).so
STATICLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).a

LDLIBS := $(patsubst $(LIBRARY_DIR)/lib%.so, -l%, $(wildcard $(LIBRARY_DIR)/libnds3-Device*.so))

#Objects
OBJECTS:=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))


CXXFLAGS += -MMD -MP -Wall -Wextra -pedantic -fPIC -pthread -fvisibility-inlines-hidden -std=c++11 $(INCLUDES)
LDFLAGS += $(LIBPATH) -lpthread -lnds3 -ldl -lrt -lgtest $(LDLIBS)

ifeq ($(COVERAGE),true)
	#CFLAGS+= -O0 -g --coverage
	CXXFLAGS+= -O0 -g 
	LDFLAGS+= -lgcov 
endif

#TESTS_OBJS := $(patsubst $(EXAMPLESDIR)/%.cpp, %.o, $(wildcard $(EXAMPLESDIR)/*.cpp))
TESTS_OBJS += $(patsubst %.cpp, %.o, $(wildcard *.cpp))

TEST = nds3core-unit-tests

all: $(TEST) $(SHAREDLIBRARY) $(STATICLIBRARY)

$(TEST): $(TESTS_OBJS) 
	$(CXX) $(LDFLAGS) $(DBG_COV) $^ -o $@

$(SHAREDLIBRARY): $(OBJECTS)
	@mkdir -p $(LIBRARY_DIR)
	$(CXX) -shared $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $(SHAREDLIBRARY)

$(STATICLIBRARY): $(OBJECTS)
	@mkdir -p $(LIBRARY_DIR)
	$(AR) rcs $@ $^

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(OBJECT_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(TESTS) *.o *.d
	$(RM) "$(STATICLIBRARY)" "$(OBJECTS)"

