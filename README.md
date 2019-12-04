# And-Inverter Graphs Logical Equivalence Checker

This project consists of two programs, `aiglec` and `aigviz`. The first can fast check the logical equivalence between 2 small AIGs (with up to 10 inputs) using Reduced Ordered Binary Decision Diagrams (BDDs). The second generates graphic visualization for an AIG and its associated BDD.

The AIGs files must be in the AIGER format (binary or ASCII).

### Dependencies

To generate the visualization files, you will need GraphViz. You will also need `g++` to compile the source code. To install them in your computer, type:
```
sudo apt-get install graphviz g++
```

### Installation

Clone the project in your computer using Git. If you don't have Git installed, run:
```
sudo apt-get install git
```

To clone the project (with Git already installed), type:
```
git clone https://github.com/rafaelcalcada/aigbdd.git
cd ./aigbdd
make
```
The programs are now ready to use.

### Usage
To check the logical equivalence between 2 distinct AIGs, type:
```
./aiglec [aig-file-1] [aig-file-2]
```
You can generate a graphical visualization for both AIGs and their associated BDDs appending `-v` in the command line. For example:
```
./aiglec graph/test1.aag graph/test2.aag -v
```
If you're interested only in generating a graphic visualization for a single AIG, you can use `aigviz`. Type:
```
./aigviz [aig-file]
```

### Project info
This work was done under the orientation of Professor André Reis, as the final work of his Logic Synthesis course. He gave valuable lessons about logic synthesis algorithms.
