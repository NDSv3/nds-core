CXX = g++
 
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I../../include -I/opt/codac/include -g
LDFLAGS = -L../../../../lib -L/opt/codac/lib

TARGETDIR = ../../../../lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed
 
# Name of the nds library
TARGET = libnds3-dev-nds-example.so
 
# Additional linker libraries
LIBS = -lnds3
 
# Source code files used in this project
SRCS = nds-example.cpp 
 
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETDIR)/$(TARGET)

# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	mkdir -p $(TARGETDIR)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)
 
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
 
