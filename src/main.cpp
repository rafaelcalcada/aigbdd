//////////////////////////////////////////////////////////////////////////////////
//
// Company: Universidade Federal do Rio Grande do Sul
// Engineer: Rafael de Oliveira Calçada
//
// Create Date: 01.12.2019 14:24:35
// Description: Transforms an AIG into a ROBDD
//
// Revision:
// Revision 0.01 - File created
//
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stack>
#include <unordered_map>
#include "structures.h"

/* GLOBALS
******************************************************************************/

// AIG data
vertex* vertices = NULL;
int* inputs = NULL;
int* outputs = NULL;
int M = 0;
int I = 0;
int L = 0;
int O = 0;
int A = 0;

// Auxiliary stack
stack<int, vector<int>>* stk;

// MK function. See ANDERSEN, H. R. An introduction to binary decision diagrams.
// In: Lecture Notes for Efficient Algorithms and Programs, 1999. p. 15
int mk(vector<triple>& T, unordered_map<triple_key,int>& H, int i, int l, int h)
{
	if(l == h) return l;    
	else
	{   
		triple t = {i, l, h};
		unordered_map<triple_key,int>::iterator it;
		it = H.find(keygen_h(t));
		if(it != H.end()) return it->second;
		else
		{
			int u = add(T, i, l, h);
			insert_h(H, i, l, h, u);
			return u;
		}        
	}
}

// APP function. See ANDERSEN, H. R. An introduction to binary decision diagrams.
// In: Lecture Notes for Efficient Algorithms and Programs, 1999. p. 19
// This is a specialization of APP that applies the AND between 2 BDDs
int app_and(vector<triple>& T, unordered_map<triple_key,int>& H, unordered_map<duo_key,int>& G, int u1, int u2)
{
	duo d = {u1, u2};
	unordered_map<duo_key,int>::iterator it;
	it = G.find(keygen_g(d));
	int u = -1;
	if(it != G.end()) return it->second;
	else if((u1 == 0 || u1 == 1) && (u2 == 0 || u2 == 1))
	{
		if(u1 == 1 && u2 == 1) u = 1;
		else u = 0;
	}
	else if(T[u1].var == T[u2].var)
	{
		u = mk(T, H, T[u1].var, app_and(T, H, G, T[u1].low, T[u2].low), app_and(T, H, G, T[u1].high, T[u2].high));
	}
	else if(T[u1].var < T[u2].var)
	{
		u = mk(T, H, T[u1].var, app_and(T, H, G, T[u1].low, u2), app_and(T, H, G, T[u1].high, u2));
	}
	else if(T[u1].var > T[u2].var)
	{
		u = mk(T, H, T[u2].var, app_and(T, H, G, u1, T[u2].low), app_and(T, H, G, u1, T[u2].high));
	}
	insert_g(G, u1, u2, u);
	return u;
}

// APP function. See ANDERSEN, H. R. An introduction to binary decision diagrams.
// In: Lecture Notes for Efficient Algorithms and Programs, 1999. p. 19
// This is a specialization of APP that applies the NAND between 2 BDDs
int app_nand(vector<triple>& T, unordered_map<triple_key,int>& H, unordered_map<duo_key,int>& G, int u1, int u2)
{
	duo d = {u1, u2};
	unordered_map<duo_key,int>::iterator it;
	it = G.find(keygen_g(d));
	int u = -1;
	if(it != G.end()) return it->second;
	else if((u1 == 0 || u1 == 1) && (u2 == 0 || u2 == 1))
	{
		if(u1 == 1 && u2 == 1) u = 0;
		else u = 1;
	}
	else if(T[u1].var == T[u2].var)
	{
		u = mk(T, H, T[u1].var, app_nand(T, H, G, T[u1].low, T[u2].low), app_nand(T, H, G, T[u1].high, T[u2].high));
	}
	else if(T[u1].var < T[u2].var)
	{
		u = mk(T, H, T[u1].var, app_nand(T, H, G, T[u1].low, u2), app_nand(T, H, G, T[u1].high, u2));
	}
	else if(T[u1].var > T[u2].var)
	{
		u = mk(T, H, T[u2].var, app_nand(T, H, G, u1, T[u2].low), app_nand(T, H, G, u1, T[u2].high));
	}
	insert_g(G, u1, u2, u);
	return u;
}

// APPLY function. See ANDERSEN, H. R. An introduction to binary decision diagrams.
// In: Lecture Notes for Efficient Algorithms and Programs, 1999. p. 19
// This is a specialization of APPLY that applies AND between 2 BDDs
int apply_and(vector<triple>& T, unordered_map<triple_key,int>& H, int u1, int u2)
{
	unordered_map<duo_key,int> G;
	init_g(G);
	return app_and(T, H, G, u1, u2);
}

// APPLY function. See ANDERSEN, H. R. An introduction to binary decision diagrams.
// In: Lecture Notes for Efficient Algorithms and Programs, 1999. p. 19
// This is a specialization of APPLY that applies AND between 2 BDDs
int apply_nand(vector<triple>& T, unordered_map<triple_key,int>& H, int u1, int u2)
{
	unordered_map<duo_key,int> G;
	init_g(G);
	return app_nand(T, H, G, u1, u2);
}

// Apply NOT operation on a BDD swapping 0's and 1's
int bdd_not(vector<triple>& T, unordered_map<triple_key,int>& H, int bdd_root)
{
	stack<int, vector<int>> stk;
	stk.push(bdd_root);
	while(!stk.empty())
	{
		int u = stk.top();
		stk.pop();
		if(T[u].low > 1) stk.push(T[u].low);
		else if(T[u].low == 0)
		{
			T[u].low = 1;
			insert_h(H, T[u].var, T[u].low, T[u].high, u);
		}
		else if(T[u].low == 1)
		{
			T[u].low = 0;
			insert_h(H, T[u].var, T[u].low, T[u].high, u);
		}
		if(T[u].high > 1) stk.push(T[u].high);
		else if(T[u].high == 0)
		{
			T[u].high = 1;
			insert_h(H, T[u].var, T[u].low, T[u].high, u);
		}
		else if(T[u].high == 1)
		{
			T[u].high = 0;
			insert_h(H, T[u].var, T[u].low, T[u].high, u);
		}
	}
	return bdd_root;
}

void create_graph(char* filename, int& M, int& I, int& L, int& O, int& A, vertex*& vertices, int*& outputs);
void print_aig(char* filename, vertex*& vertices, int* outputs, int& M, int& I, int& O);
void print_bdd(char* filename, vector<triple>& T, int bdd_root, int output_index);

/* MAIN FUNCTION: TRANSFORMS AN AIG INTO A ROBDD
******************************************************************************/
int main(int argc, char* argv[])
{    

	// read the path of the AIG file
	if(argc != 3)
	{
		cout << "Usage: " << argv[0] << " [aig-file] [output-file]" << endl;
		return EXIT_FAILURE;
	}
	char* aigfile = argv[1];

	// read the AIG file and creates the graph
	create_graph(aigfile, M, I, L, O, A, vertices, outputs);

	// allocates the auxiliary stack
	stk = new stack<int, std::vector<int>>;

	// creates the ROBDD data structures
	vector<triple> T;
	unordered_map<triple_key,int> H;
	init_t(T, I+1);
	init_h(H);	

	// prints the AIG
	print_aig(argv[2], vertices, outputs, M, I, O);

	for(int i = 0; i < O; i++)
	{

		int vertex_index = (outputs[i] >> 1) - 1;
		vertex* v = &vertices[vertex_index];

		// creates a ROBDD for each input
		for(int j = 0; j < I; j++)
		{
			vertex* w = &vertices[j];
			w->bdd = mk(T, H, j+1, 0, 1);
		}	

		while(v != NULL)
		{
			int left_index = v->left / 2 - 1;
			int right_index = v->right / 2 - 1;
			vertex* left = &vertices[left_index];
			vertex* right = &vertices[right_index];
			if(left != NULL && left->bdd == -1)
			{
				stk->push(vertex_index);
				v = left;
				vertex_index = left_index;
			}
			else if(right != NULL && right->bdd == -1)
			{
				stk->push(vertex_index);
				v = right;
				vertex_index = right_index;
			}
			else
			{   
				int bdd_left = left->bdd;
				int bdd_right = right->bdd;
				if(v->left & 1 == 1) bdd_left = apply_nand(T, H, bdd_left, bdd_left);
				if(v->right & 1 == 1) bdd_right = apply_nand(T, H, bdd_right, bdd_right);
				v->bdd = apply_and(T, H, bdd_right, bdd_left);
				if(stk->empty()) v = NULL;
				else
				{
					vertex_index = stk->top();
					stk->pop();
					v = &vertices[vertex_index];
				}
			}
		}

		vertex_index = (outputs[i] >> 1) - 1;
		v = &vertices[vertex_index];
		if(outputs[i] & 1 == 1) v->bdd = apply_nand(T, H, v->bdd, v->bdd);
		print_bdd(argv[2], T, v->bdd, i);

		// erase the results for the next iteration
		for(int j = I; j < I+A; j++) 
		{
			vertex* w = &vertices[j];
			w->bdd = -1;
		}

	}

	return EXIT_SUCCESS;

}
