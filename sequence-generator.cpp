/*
	Automatic random DNA sequence generator.

	@author Tiago Martins
*/

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

// Nucleobases present in DNA
const char NUCLEOBASES[4] = {'A', 'C', 'G', 'T'};

// Function declarations
string sequence_generator(unsigned long size);
char select_nucleobase(int n);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Error: one argument expected. Please specify the length of the sequence." << endl;
		return 1;
	}

	cout << sequence_generator(strtol(argv[1], nullptr, 10)) << endl;

	return 0;
}

/*
 * Random DNA sequence generator.
 *
 * @param size Sequence size.
 * @return Random DNA sequence.
 */
string sequence_generator(unsigned long size) {
	string sequence (size, 0);	// DNA sequence
	unsigned long i = 0;		// Number of characters
	int n = 0;					// Random generated number

	while (i < size) {
		n = rand() % 100 + 1;	// Random number between 1 and 100
		sequence[i] = select_nucleobase(n);
		i++;
	}

	return sequence;
}

/*
 * Selects a nucleobase given a number between 1 and 100.
 *
 * @param n Integer between 1 and 100.
 * @return Nucleobase character.
 */
char select_nucleobase(int n) {
	// Selecting the nucleobase equiprobably
	if (n <= 25)
		return NUCLEOBASES[0];

	else if (n <= 50)
		return NUCLEOBASES[1];

	else if (n <= 75)
		return NUCLEOBASES[2];

	else 	// n <= 100
		return NUCLEOBASES[3];
}