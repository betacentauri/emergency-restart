SRC = emergency_restart.cpp

OBJ = $(SRC:.cpp=.o)

OUT = emergency-restart

CXXFLAGS = -O2 -pthread -std=c++11

.SUFFIXES: .cpp

default: $(OUT)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(OUT)