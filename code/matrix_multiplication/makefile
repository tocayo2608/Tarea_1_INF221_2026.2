CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

TARGET := matrix_multiplication
SOURCES := matrix_multiplication.cpp $(wildcard algorithms/*.cpp)
PYTHON ?= python3

.PHONY: all compile run plot clean clean-data generate

all: compile

compile: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

run: $(TARGET)
	./$(TARGET)

plot:
	(cd scripts && $(PYTHON) plot_generator.py)

clean:
	rm -f $(TARGET) matrix_multiplication.exe

clean-data:
	rm -f data/matrix_input/* data/matrix_output/* data/measurements/*.csv

generate:
	(cd scripts && $(PYTHON) matrix_generator.py)
