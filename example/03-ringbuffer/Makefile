CXX=g++
CXXFLAGS=-Wall -g -O0
RM=rm -rf
TARGET=ringbuffer ringbuffer-unlock

LIBS=-pthread

all: $(TARGET)

$(TARGET): %:%.cpp
	$(CXX) $< $(CXXFLAGS) $(LIBS) -o $@

clean:
	$(RM) $(TARGET)

.PHONY: clean
