# And-Inverter Graphs Logical Equivalence Checker

This project consists of two programs, `aiglec` and `aigviz`. The first can fast check the logical equivalence between 2 small AIGs (with up to 10 inputs) using Reduced Ordered Binary Decision Diagrams (BDDs). The second generates graphic visualization of an AIG and its associated BDD.

### Dependencies

To generate the visualization files, you will need GraphViz. You will also need `g++` to compile the source code. To install them in your computer, type:
```
sudo apt-get install graphviz g++
```

### How to install

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


