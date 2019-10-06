
CXXFLAGS=-I..

source_files = $(wildcard *.cpp)
executables = $(patsubst %.cpp, %, $(source_files))
all: $(patsubst %.cpp, %.exe, $(source_files))

%.cpp:

%.exe: %.cpp
	$(LINK.cpp) $^ $(LDFLAGS) -o $@


.PHONY: clean $(executables)

$(executables) : % : %.exe
clean:
	del *.exe
