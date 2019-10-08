CXX=g++
CXXFLAGS=-Wall -g -O0
RM=rm -rf
TARGET=epoll

LIBS=-pthread

all: $(TARGET)

$(TARGET): %:%.cpp
	$(CXX) $< $(CXXFLAGS) $(LIBS) -o $@

clean:
	$(RM) $(TARGET)

.PHONY: clean