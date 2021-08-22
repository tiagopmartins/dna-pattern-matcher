/*
	MIT License

	Copyright (c) 2021 Tiago Martins

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	@author Tiago Martins
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function declarations
string readFile(ifstream *file);
bool checkPattern(string *pattern);
bool checkSequence(string *sequence);
bool checkDNA(string *sequence);
bool checkNucleobase(char nucleobase);
long boyer_moore_horspool(string *P, string *T);

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "Error: Please specify the pattern and the name of the file containing the DNA sequence, respectively." << endl;
		return 1;
	}

	string pattern (argv[1]);		// Nucleobases pattern to search for
	string sequence;				// DNA sequence to be searched on
	ifstream dna_file (argv[2]);	// File to read

	// Checking if the DNA file exists
	if (!dna_file) {
		cerr << "Error: File not found." << endl;
		return 1;
	}

	cout << "DNA matcher initializing...\n" << endl;
	
	if (!checkPattern(&pattern)) {
		cerr << "Error: Invalid pattern given. Only 'A', 'C', 'G' and 'T' nucleobases are accepted." << endl;
		return 1;
	}

	// Reading the DNA file
	sequence = readFile(&dna_file);
	dna_file.close();
	
	if (!checkSequence(&sequence)) {
		cerr << "Error: Invalid sequence given. Only 'A', 'C', 'G' and 'T' nucleobases are accepted." << endl;
		return 1;
	}

	//cout << boyer_moore_horspool(&pattern, &sequence) << endl;

	return 0;
}

/*
 * Reads the given file into a string.
 *
 * @param file Pointer to the input file stream.
 * @return String containing the contents of the file.
 */
string readFile(ifstream *file) {
	string str, line;

	while (getline(*file, line))
		str += line;

	return str;
}

/*
 * Checks if the DNA pattern given is valid. A pattern is valid if it uses only
 * the nucleobases 'A', 'C', 'G' and 'T'.
 *
 * @param pattern Pattern to check.
 * @return Boolean.
 */
bool checkPattern(string *pattern) {
	// Checking if the pattern given is valid
	cout << "Checking DNA pattern..." << endl;
	
	if (!checkDNA(pattern))
		return false;
		
	cout << "Pattern OK!\n" << endl;
	return true;
}

/*
 * Checks if the DNA sequence given is valid. A sequence is valid if it uses only
 * the nucleobases 'A', 'C', 'G' and 'T'.
 *
 * @param sequence Sequence to check.
 * @return Boolean.
 */
bool checkSequence(string *sequence) {
	// Checking if the sequence given is valid
	cout << "Checking DNA sequence..." << endl;
	
	if (!checkDNA(sequence))
		return false;
		
	cout << "Sequence OK!\n" << endl;
	return true;
}

/*
 * Checks if the given DNA sequence is valid. A valid sequence contains only the
 * nucleobases 'A', 'C', 'G' and 'T'.
 * 
 * @param sequence Pointer to the sequence to check.
 * @return Boolean.
 */
bool checkDNA(string *sequence) {
	for (unsigned long i = 0; i < (*sequence).length(); i++) {
		if (!checkNucleobase((*sequence)[i]))
			return false;
	}

	return true;
}

/*
 * Checks if the given character is one of the 4 nucleobases constituting the DNA.
 *
 * @param nucleobase Character to check.
 * @return Boolean.
 */
bool checkNucleobase(char nucleobase) {
	switch (nucleobase) {
		case 'A':
			return true;

		case 'C':
			return true;

		case 'G':
			return true;

		case 'T':
			return true;

		default:
			return false;
	}
}

/*
 * Boyer-Moore-Horspool algorithm optimized implementation. Returns the first
 * match found of the pattern 'P' inside the string 'T'.
 *
 * @param P Pointer to the pattern to search for.
 * @param T Pointer to the string to search 'P' on.
 * @returns Index of the first match found.
 */
long boyer_moore_horspool(string *P, string *T) {

}