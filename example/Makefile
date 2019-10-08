CXX=g++
CXXFLAGS=-Wall -g -O0
RM=rm -rf

all:
	@for dir in $(shell find . -type d ! -name .); do \
		cd $$dir; \
		if [ -e Makefile ]; then make; fi; \
		cd -; \
	done

clean:
	@for dir in $(shell find . -type d ! -name .); do \
		cd $$dir; \
		if [ -e Makefile ]; then make clean; fi; \
		cd -; \
	done

.PHONY: clean

