CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

TARGET := sorting
SOURCES := sorting.cpp $(wildcard algorithms/*.cpp)
PYTHON ?= python3

.PHONY: all compile run plot clean clean-data generate generate-reduced generate-1000 generate-100000 generate-10000000 prepare-1000 prepare-100000 prepare-10000000

all: compile

compile: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

run: $(TARGET)
	./$(TARGET)

plot:
	(cd scripts && $(PYTHON) plot_generator.py)

clean:
	rm -f $(TARGET) sorting.exe

clean-data:
	rm -f data/array_input/* data/array_output/* data/measurements/*.csv

generate:
	(cd scripts && $(PYTHON) array_generator.py)

generate-reduced:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 10 1000 100000)
	(cd scripts && $(PYTHON) array_generator.py --n 10000000 --tipo aleatorio --dominio D7 --muestras a)

generate-1000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 1000 --tipo aleatorio --dominio D1 --muestras a)

generate-100000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 100000 --tipo aleatorio --dominio D1 --muestras a)

generate-10000000:
	$(MAKE) clean-data
	(cd scripts && $(PYTHON) array_generator.py --n 10000000 --tipo aleatorio --dominio D1 --muestras a)

prepare-1000:
	rm -f data/array_input/* data/array_output/*
	(cd scripts && $(PYTHON) array_generator.py --n 1000 --tipo aleatorio --dominio D1 --muestras a)

prepare-100000:
	rm -f data/array_input/* data/array_output/*
	(cd scripts && $(PYTHON) array_generator.py --n 100000 --tipo aleatorio --dominio D1 --muestras a)

prepare-10000000:
	rm -f data/array_input/* data/array_output/*
	(cd scripts && $(PYTHON) array_generator.py --n 10000000 --tipo aleatorio --dominio D1 --muestras a)
