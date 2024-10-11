CXX = g++
CXXFLAGS = -I/usr/include -lcurl -ljsoncpp
OBJ = main.o

install:
	sudo apt-get install -y libcurl4-openssl-dev libjsoncpp-dev

all: install main

bot: $(OBJ)
	$(CXX) -o main $(OBJ) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f *.o main
