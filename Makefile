CXX := g++
#CXXFLAGS := -std=c++14
CXXFLAGS := -std=c++14 -DBENCHMARK
TARGET := graph-isomorphism
OBJECTS := objs/main.o objs/graph.o objs/utils.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJECTS): objs/%.o: srcs/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm $(TARGET) $(OBJECTS)

