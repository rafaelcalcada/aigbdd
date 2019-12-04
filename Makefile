INC=-I./include

all: aiglec

aiglec: src/aiglec.cpp src/aigreader.cpp src/structures.cpp
	g++ $(INC) -O3 src/aiglec.cpp src/aigreader.cpp src/structures.cpp -o aiglec

clean:
	rm -rf *.png

distclean:
	rm -rf aiglec