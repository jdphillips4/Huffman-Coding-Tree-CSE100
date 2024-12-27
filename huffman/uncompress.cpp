#include "HCTree.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
		error("Too many/little args");
	}
    //Open the input file for reading
    FancyInputStream inFile(argv[1]);
	/*if(!inFile.good()) {
		error("Input file not good");
	}*/

    //Read the file header at the beginning of the input file, and use it to reconstruct the Huffman coding tree
    //Start w 256*4 version
    vector<int> freqs (256,0);
    FancyOutputStream outFile(argv[2]);
    HCTree* hct = new HCTree();
    /*for (long unsigned int i =0; i< 256; i++) {
        freqs[i] = inFile.read_int();
    }*/
    
    //New version:
    if (!(inFile.filesize() < 256*3)) {
        for (long unsigned int i =0; i< 256; i++) {
            freqs[i] = 65536*(inFile.read_byte()) + 256*(inFile.read_byte()) + inFile.read_byte(); //2^16 and 2^8 and 2^1 for digits
        }
        hct->build(freqs);
    }
    else {
        int j = inFile.read_byte();
        while (j != -1) {
            outFile.write_byte(j);
            j = inFile.read_byte();
        }
        delete hct;
        return 0;
    }
    

    //Open the output file for writing
    //FancyOutputStream outFile(argv[2]);
	/*if (!outFile.good()) {
		error("Outfile not good");
	}*/

    //Using the Huffman coding tree, decode the bits from the input file into the appropriate sequence of bytes, writing them to the output file
    unsigned char r = hct->decode(inFile); 
	while (r != (unsigned char)256) {
		outFile.write_byte(r); 
        r = hct->decode(inFile); //unsigned char
	}

    //Close the input and output files (note that this is handled for you; see Design Notes)
    delete hct;
}
