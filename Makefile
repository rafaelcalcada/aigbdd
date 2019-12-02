INC=-I./include

all: eqc

eqc: src/main.cpp src/aigreader.cpp src/structures.cpp
	g++ $(INC) -O3 src/main.cpp src/aigreader.cpp src/structures.cpp -o ./bin/eqc

toascii: src/toascii.cpp
	g++ $(INC) -O3 src/toascii.cpp -o ./bin/toascii

clean:
	rm -rf ./bin/eqc ./bin/toascii