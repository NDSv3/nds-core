CXX =g++

SOURCES =$(wildcard ndsDrv/*.cpp)
OBJECTS =$(patsubst ndsDrv/%.cpp, ndsDrv/%.o, $(SOURCES))
HEADERS =$(wildcard ndsDrv/*.h)

STATICOBJ =$(patsubst simDev/%.cpp, simDev/%.o, $(wildcard simDev/*.cpp))

STD =-std=c++11
INCLUDES =-I../../include -I/opt/codac/include -IsimDev
DEFS =-DEPICS -DNDS3_DLL
DBG =-g
CXXFLAGS =$(STD) -Wall -Wextra -pedantic $(DBG) $(INCLUDES)

NOLINK =-c -fPIC -pthread
LINK =-shared -Wl,-as-needed

# Additional linker libraries
LIBS =-lnds3
LDFLAGS =-L../../../../lib -L/opt/codac/lib


TARGETDIR =../../../../lib

STATICLIB =simDev/simulated-signal.a
DYNLIB =$(TARGETDIR)/libnds3-exampleDrv.so

DEPENDENCIES =$(DYNLIB)
DEPENDENCIES +=$(STATICLIB)


all: $(DEPENDENCIES)

$(DYNLIB): $(OBJECTS) $(STATICLIB)
	mkdir -p $(TARGETDIR)
	$(CXX) $^ $(CXXFLAGS) $(LINK) $(LIBS) $(STATICLIB) $(LDFLAGS) -o $@

ndsDrv/%.o: ndsDrv/%.cpp $(HEADERS)
	$(CXX) $< $(CXXFLAGS) $(DEFS) $(NOLINK) -o $@

$(STATICLIB): $(STATICOBJ)
	ar rc $@ $^

simDev/%.o: simDev/%.cpp
	$(CXX) $< $(CXXFLAGS) $(NOLINK) -o $@


.PHONY: clean
clean:
	$(RM) $(DYNLIB) ndsDrv/*.o $(STATICLIB)
