#include "HCTree.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	//Parse the command line arguments and throw an 
	//error message if the user runs your program incorrectly
	if (argc != 3) {
		error("Too many/little args");
	}


	// Open the input file for reading
	FancyInputStream inFile(argv[1]);
	/*if(!inFile.good()) {
		error("Input file not good");
	}*/

	//Read bytes from the file. Count the number of occurrences of each byte value
	vector<int> freqs (256,0);
	int r = inFile.read_byte();
	int totBytes; //might need later
	while (r != -1) {
		++freqs[r];
		++totBytes;
		r = inFile.read_byte();
	}	

	//Use the byte counts to construct a Huffman coding tree. 
	//Each unique byte with a non-zero count will be a leaf node in the Huffman tree
	HCTree* hct = new HCTree();
	hct->build(freqs);

	//Open the output file for writing
	FancyOutputStream outFile(argv[2]);
	/*if (!outFile.good()) {
		error("Outfile not good");
	}*/
	//Write enough information (a "file header") to the output file
	//Start with the 256*4 version
	/*for (long unsigned int i=0; i<freqs.size(); i++) {
		outFile.write_int(freqs[i]);
	}*/
	//New version: start w array of 1s and 0 for 256 bits to tell if that is present or not then just list out frequencies (doesn't work)
	//int is 32 bits, only need 24 to contain 10 mil aka 3 bytes instead of 4
	//TODO:
	if (!(inFile.filesize() < 3*256)) {
		int count;
		int byte2;
		int byte3;
		for (long unsigned int i=0; i<freqs.size(); i++) {
			count = freqs[i];

			byte3 = count%256;//last 8 bits
			byte2 = (count/256) % 256; //middle 8
			count = count - byte2*256 - byte3; //first 8
			
			outFile.write_byte(count);
			outFile.write_byte(byte2);
			outFile.write_byte(byte3);
		}
	}
	else {
		int j = inFile.read_byte();
        while (j!=-1) {
            outFile.write_byte(j);
			j = inFile.read_byte();
        }
        delete hct;
		return 0;
	}
	

	//Move back to the beginning of the input file to be able to read it, again
	inFile.reset();

	//Using the Huffman coding tree, translate each byte from the input file into its code, 
	//and append these codes as a sequence of bits to the output file, after the header
	r = inFile.read_byte(); 
	while (r != -1) {
		hct->encode((unsigned char)r,outFile);
		r = inFile.read_byte();
	}
	delete hct;
}
