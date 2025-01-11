# Find all .cpp files in the source directory
SOURCES_CPP = $(shell find . -name '*.cpp')

# Create object file paths for the .cpp files
OBJECTS_CPP = $(patsubst ./%, $(OUTPUT)/obj/%, $(SOURCES_CPP:.cpp=.obj))

# Add any required include paths
CXXFLAGS += -I .

# Default target: build the final executable
all: $(OUTPUT)/bin/OX.exe

# Link the final executable
$(OUTPUT)/bin/OX.exe: $(OBJECTS_CPP)
	$(LDXX) $(LDFLAGS) -o $@ $^
	echo "\033[31mCreated -> OX.exe\033[0m"

# Rule for compiling C++ source files into object files
$(OUTPUT)/obj/%.obj: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	echo "\033[31mCompiled -> $<\033[0m"
