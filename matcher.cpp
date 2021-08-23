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
#include <vector>

using namespace std;

// Number of existing characters - (A, C, G, T).
const short CHARS_N = 4;

const short BAD_A_POSITION = 0;		// Position of the 'A' nucleobase on the bad character skip table
const short BAD_C_POSITION = 1;		// Position of the 'C' nucleobase on the bad character skip table
const short BAD_G_POSITION = 2;		// Position of the 'G' nucleobase on the bad character skip table
const short BAD_T_POSITION = 3;		// Position of the 'T' nucleobase on the bad character skip table

// Function declarations

string readFile(ifstream *file);
bool checkPattern(string *pattern);
bool checkSequence(string *sequence);
bool checkDNA(string *sequence);
bool checkNucleobase(char nucleobase);
bool boyer_moore_horspool(string *pattern, string *sequence, vector<size_t> *index);
void preprocessBMH(string *pattern, size_t *bad_character_skip);
bool compareBMH(string str1, string str2, size_t len);
size_t selectNucleobasePos(char nucleobase);
void printProgress(float progress);

int main(int argc, char *argv[]) {
	if (argc < 4) {
		cerr << "Error: Please specify the pattern, the name of the file containing the DNA sequence and the name of the file to write to, respectively." << endl;
		return 1;
	}

	vector<size_t> results;				// Pointer to the position of at which the pattern begins
	string pattern (argv[1]);			// Nucleobases pattern to search for
	string sequence;					// DNA sequence to be searched on
	ifstream dna_file (argv[2]);		// File to read

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

	if (boyer_moore_horspool(&pattern, &sequence, &results)) {
		cout << "Matches successfully found." << endl;
		ofstream matches_file (argv[3]);	// File to write to
		
		matches_file << "Match found @ indexes [";

		for (size_t i = 0; i < results.size() - 1; i++)
			matches_file << results[i] << ", ";

		matches_file << results[results.size() - 1] << "]" << endl;
		matches_file.close();
	}

	else
		cout << "No match found." << endl;

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
	for (size_t i = 0; i < (*sequence).length(); i++) {
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
 * Boyer-Moore-Horspool algorithm optimized implementation. Returns the matches
 * found for the pattern inside the sequence.
 *
 * @param pattern Pointer to the pattern to search for.
 * @param sequence Pointer to the string to search the pattern on.
 * @param matches Pointer to the vector with the position of the matches.
 * @returns Boolean according to if the match was found.
 */
bool boyer_moore_horspool(string *pattern, string *sequence, vector<size_t> *matches) {
	// Number of characters possible to skip
	size_t skip = 0, pattern_len = pattern->length(), sequence_len = sequence->length();
	// Bad character heuristic skip table. Order convention: [A, C, G, T]
	size_t *bad_character_skip = (size_t *) malloc(sizeof(size_t) * CHARS_N);
	preprocessBMH(pattern, bad_character_skip);

	while (pattern_len <= (sequence_len - skip)) {
		printProgress((float) skip / sequence_len);

		if (compareBMH(sequence->substr(skip), *pattern, pattern_len))
			matches->push_back(skip);

		skip += bad_character_skip[selectNucleobasePos((*sequence)[skip + pattern_len - 1])];
	}

	// Fully done
	printProgress(1);
	cout << "\n" << endl;

	free(bad_character_skip);
	return !matches->empty();
}

/*
 * Preprocessing using the "bad" character heuristic for the Boyer-Moore-Horspool
 * algorithm. Sets a table with the number of characters to skip when encountering
 * a specific character.
 *
 * @param pattern Pattern to preprocess.
 * @param bad_character_skip Table containing the number of characters to skip when
 * 							 encountering a character.
 */
void preprocessBMH(string *pattern, size_t *bad_character_skip) {
	size_t len = (*pattern).length();

	// Populating the table with the default values
	for (size_t k = 0; k < CHARS_N; k++)
		bad_character_skip[k] = len;

	// Populate the table with the correct values
	for (size_t i = 0; i < (len - 1); i++)
		bad_character_skip[selectNucleobasePos((*pattern)[i])] = len -i - 1;
}

/*
 * Compares two strings according to the Boyer-Moore-Horspool algorithm.
 *
 * @param str1 First string.
 * @param str2 Second string.
 * @param len Size of the strings.
 * @return Boolean.
 */
bool compareBMH(string str1, string str2, size_t len) {
	size_t i = 0, middle = len / 2;

	if (str1[len - 1] == str2[len - 1] && str1[middle] == str2[middle]) {
		while (str1[i] == str2[i]) {
			if (i == (len - 2))
				return true;

			i++;
		}
	}

	return false;
}

/*
 * Given a nucleobase, selects the correct position of it in the bad character
 * heuristic table.
 * 
 * @param nucleobase Character corresponding to the nucleobase.
 * @return Position of the nucleobase in the bad character heuristic table.
 */
size_t selectNucleobasePos(char nucleobase) {
	switch (nucleobase) {
		case 'A':
			return BAD_A_POSITION;

		case 'C':
			return BAD_C_POSITION;

		case 'G':
			return BAD_G_POSITION;

		case 'T':
			return BAD_T_POSITION;

		default:
			return false;
	}
}

/*
 * Prints a progress bar according to the progress made.
 *
 * @param progress Percentage of the analysis done.
 */
void printProgress(float progress) {
	// Progress bar size
	const short WIDTH = 70;
	// Position for the progress to stop at
	short stop_pos = progress * WIDTH;

	cout << '[';
	for (int i = 0; i < WIDTH; i++) {
		if (i < stop_pos)
			cout << "=";
		
		else if (i == stop_pos)
			cout << ">";
		
		else
		 	cout << " ";
	}
	
	cout << "] " << (int) (progress * 100) << "%\r";
	fflush(stdout);
}