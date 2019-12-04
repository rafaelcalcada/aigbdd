# And-Inverter Graphs Logical Equivalence Checker

This project consists of two programs, `aiglec` and `aigviz`. The first can fast check the logical equivalence between 2 small AIGs (with up to 10 inputs) using Reduced Ordered Binary Decision Diagrams (BDDs). The second generates graphic visualization for an AIG and its associated BDD.

The AIG files must be in the AIGER format (binary or ASCII).

### Dependencies

GraphViz is needed to generate graphical visualization. You will also need `g++` to compile the source code. To install them in your computer, type in your terminal:
```
sudo apt-get install graphviz g++
```

### Installation

Clone the project in your computer using Git. If you don't have Git installed, run:
```
sudo apt-get install git
```
The command to clone the project is:
```
git clone https://github.com/rafaelcalcada/aigbdd.git
```
To build `aiglec` and `aigviz`, run `make`:
```
cd ./aigbdd
make
```
The programs are now ready to use.

### Usage
To check the logical equivalence between 2 distinct AIGs, type:
```
./aiglec [aig-file-1] [aig-file-2]
```
You can generate graphical visualization for both AIGs (and their associated BDDs) appending `-v` in the command line. For example:
```
./aiglec graph/test1.aag graph/test2.aag -v
```
If you're only interested in generating graphic visualization for an AIG, you can use `aigviz`. Type:
```
./aigviz [aig-file]
```

### Project info
This work was done under the orientation of Professor André Reis, as the final work of his Logic Synthesis course. He gave valuable lessons about logic synthesis algorithms.
