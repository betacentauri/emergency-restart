SRC = emergency_restart.cpp

OBJ = $(SRC:.cpp=.o)

OUT = emergency-restart

CFLAGS ?= -O2
CFLAGS += -pthread -std=gnu++0x

CC = g++

.SUFFIXES: .cpp

default: $(OUT)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ) $(OUT)