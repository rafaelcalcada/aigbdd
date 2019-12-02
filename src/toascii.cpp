//////////////////////////////////////////////////////////////////////////////////
//
// Company: Universidade Federal do Rio Grande do Sul
// Engineer: Rafael de Oliveira Calçada
//
// Create Date: 19.09.2019 16:12:57
// Description: Converts a binary AIG to ASCII format
//
// Revision:
// Revision 0.01 - File created
//
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include <string.h>
#include <vector>
using namespace std;

// structures that forms the AIG graph
typedef struct v {
	int i1;
	int i2;
	int fanout;
	int layer;
} vertex;

// 'lists' that describes the AIG
vertex* vertices;
int* outputs;

// global variables that describes the AIG
int num_variables = 0;
int num_inputs = 0;
int num_latches = 0;
int num_outputs = 0;
int num_ands = 0;

// get a char from a file in the AIGER binary format
unsigned char getnoneofch(ifstream& input_file)
{
	int ch = input_file.get();
	if(ch != EOF) return ch;
	cerr << "*** decode: unexpected EOF" << endl;
	exit(-1);
}

// decodes a delta encoding from a file in the AIGER binary format
unsigned int decode(ifstream& input_file)
{
	unsigned x = 0, i = 0;
	unsigned char ch;
	while ((ch = getnoneofch(input_file)) & 0x80)
	{
		x |= (ch & 0x7f) << (7 * i++);
	}
	return x | (ch << (7 * i));
}


// convert to ASCII format
void convert_to_ascii_format(ifstream& input_file, ofstream& output_file)
{

	char* token;
	char buffer[256];

	// check for latches
	if(num_latches != 0)
	{
		cerr << "This graph contains latches. The current version of this implementation do not support them." << endl;
		exit(-1);
	}

	// integrity check #1
	if(num_variables != num_inputs + num_latches + num_ands)
	{
		cerr << "This graph is invalid. M != I + L + A." << endl;
		exit(-1);
	}

	// Memory allocation
	outputs = new int[num_outputs];
	vertices = new vertex[num_variables];

	// initialization of output list
	for(int i = 0; i < num_outputs; i++) outputs[i] = -1;

	// creates the input vertices
	for(int i = 1; i <= num_inputs; i++)
	{
		output_file << i*2 << endl;
	}

	// save the label of the output vertices
	for(int i = 0; i < num_outputs; i++)
	{
		input_file.getline(buffer, sizeof(buffer));
		if(strlen(buffer) < 1)
		{
			cerr << "The input file reached the end before expected." << endl;
			exit(-1);
		}
		token = strtok(buffer, " ");
		int label = atoi(token);

		// integrity check #5
		if(label < 0)
		{
			cerr << "The graph contains an invalid (negative) output index: " << label << "." << endl;
			exit(-1);
		}

		// integrity check #6
		for(int j = 0; j < num_outputs; j++)
			if(outputs[j] == label)
			{
				cerr << "The graph contains an output declared twice: " << label << "." << endl;
				exit(-1);
			}

		// if reached here, everything is OK, so adds the label in the outputs list
		output_file << label << endl;
	}

	// creates the vertices and its edges
	for(int i = 0; i < num_ands; i++)
	{
		
		unsigned int delta0 = decode(input_file);
		unsigned int delta1 = decode(input_file);

		int label = (num_inputs+i+1)*2;
		int i1 = label - delta0;
		int i2 = i1 - delta1;

		// integrity check #7
		if(label < 0)
		{
			cerr << "The graph contains an invalid (negative) vertex index: " << label << "." << endl;
			exit(-1);
		}

		// integrity check #8
		if(label != ((i+num_inputs+1)*2))
		{
			cerr << "The AIG format states that the label of a vertex must be twice its index, but the vertex with index " << i+1 << " has the label " << label << "." << endl;
			exit(-1);
		}

		// integrity check #9
		if(i1 < i2)
		{
			cerr << "The AIG format states that the label of the first input of a vertex must be greater than the second." << endl;
			cerr << "Found i1=" << i1 << " and i2=" << i2 << " for the label " << label << "." << endl;
			exit(-1);			
		}

		// integrity check #10
		if(i1 < 0 || i2 < 0)
		{
			cerr << "The vertex has an invalid value for its inputs." << endl;
			cerr << "Found i1=" << i1 << " and i2=" << i2 << " for the label " << label << "." << endl;
			exit(-1);			
		}

		// integrity check #11
		if(label <= i1 || label <= i2)
		{
			cerr << "The AIG format states that the label must be greater than the value of its inputs." << endl;
			cerr << "Found i1=" << i1 << " and i2=" << i2 << " for the label " << label << "." << endl;
			exit(-1);			
		}

		output_file << label << " " << i1 << " " << i2 << endl;

	}

}


int main(int argc, char* argv[])
{

	// check for correct usage
	if(argc < 3)
	{
		cout << "Usage: " << argv[0] << " <file.aig> <file.aag>" << endl;
		exit(-1);
	}

	// opens the input file
	ifstream input_file;
	ofstream output_file;
	input_file.open(argv[1], ios::binary | ios::in);
	output_file.open(argv[2], ios::out);

	if(!input_file.is_open())
	{
		cerr << "Failed to open the input file." << endl;
		exit(-1);
	}

	if(!output_file.is_open())
	{
		cerr << "Failed to create the output file." << endl;
		exit(-1);
	}

	// process the 1st line
	char buffer[256];
	buffer[0] = '\0';
	input_file.getline(buffer, sizeof(buffer));

	// split into tokens, saving the values in variables
	char* token = strtok(buffer," ");
	token = strtok(NULL, " ");
	num_variables = atoi(token);
	token = strtok(NULL, " ");
	num_inputs = atoi(token);
	token = strtok(NULL, " ");
	num_latches = atoi(token);
	token = strtok(NULL, " ");
	num_outputs = atoi(token);
	token = strtok(NULL, " ");
	num_ands = atoi(token);

	// file format check
	if(strlen(buffer) > 2 && buffer[0] == 'a' && buffer[1] == 'a' && buffer[2] == 'g')
	{
		cout << endl <<  "Input file is already an AIG in the ASCII format." << endl;
		exit(-1);
	}
	else if(strlen(buffer) > 2 && buffer[0] == 'a' && buffer[1] == 'i' && buffer[2] == 'g')
	{
		output_file << "aag " << num_variables << " " << num_inputs << " " << num_latches
			 << " " << num_outputs << " " << num_ands << endl;
	}
	else {
		cerr << "Failed to process the input file. Wrong, invalid or unknown format." << endl;
		exit(-1);
	}

	// convert
	convert_to_ascii_format(input_file, output_file);
	input_file.close();
	output_file.close();

	return 0;

}
