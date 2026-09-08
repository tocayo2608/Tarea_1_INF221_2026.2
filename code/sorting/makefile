CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

TARGET := sorting
SOURCES := sorting.cpp $(wildcard algorithms/*.cpp)
PYTHON ?= python3

.PHONY: all compile run clean clean-data generate generate-1000 generate-100000 generate-10000000

all: compile

compile: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) sorting.exe

clean-data:
	rm -f data/array_input/* data/array_output/* data/measurements/*.csv

generate:
	(cd scripts && $(PYTHON) array_generator.py)

generate-1000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 1000 --tipo aleatorio --dominio D1 --muestras a)

generate-100000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 100000 --tipo aleatorio --dominio D1 --muestras a)

generate-10000000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 10000000 --tipo aleatorio --dominio D1 --muestras a)
