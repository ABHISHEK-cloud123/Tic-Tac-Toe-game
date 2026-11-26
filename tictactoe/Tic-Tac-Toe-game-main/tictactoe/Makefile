CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRCS = main.cpp board.cpp logic.cpp display.cpp sounds.cpp
OUT = tictactoe

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS)

clean:
	rm -f $(OUT)
