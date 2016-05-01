CFLAGS=-c -g -Wall -std=c11
CXXFLAGS=-c -g -O3 -Wall -Wextra -pedantic -std=c++11
LDFLAGS=-pthread
SRCEXT=cpp
HEADEREXT=hpp
SOURCES=$(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS=$(patsubst %.$(SRCEXT),%.o,$(SOURCES))
HEADERS=$(shell find . -type f -name "*.$(HEADEREXT)")
EXECUTABLE=cache-sim
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

ccred:=$(shell echo "\033[0;31m")
ccgreen:=$(shell echo "\033[0;92m")
ccyellow:=$(echo -e "\033[0;33m")
ccend:=$(shell echo "\033[0m")


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN) ./traces/trace1.txt out.txt
	diff out.txt ./correct_outputs/trace1_output.txt
	$(RUN) ./traces/trace2.txt out.txt
	diff out.txt ./correct_outputs/trace2_output.txt
	$(RUN) ./traces/trace3.txt out.txt
	diff out.txt ./correct_outputs/trace3_output.txt
