# Make file template. Replace sources as needed.
# If output is shared library add -no-pie to flags
# Lisa Miller
CXX=gcc
CXXFLAGS=-Wall -pedantic-errors -g -std=gnu99
SOURCES=final_heweiron.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=SHA1

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ -lm

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
