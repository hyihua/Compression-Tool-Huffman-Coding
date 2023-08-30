#include "huffman.h"
using namespace std;


void HuffCode::encode(char *filename) {
    FILE *given_file;
    given_file = fopen(filename, "rb");
    if(!given_file){
        cout << "The given file does not exist." << endl;
        return;
    }

    // construct a huffman tree 
	buildHuffmanTree(given_file);
    given_file = fopen(filename, "rb");
    fseek(given_file, 0, SEEK_SET);

    string encoding_string = "";

    unsigned char *read_pos, read_char; 
    char *str_pointer;
    char st;
    read_pos = &read_char;
    fread(read_pos, 1, 1, given_file);
    for (long int i = 0; i < original_bytes; i++){
        str_pointer = &coding_array[read_char][0];
        while(*str_pointer){
            st = *str_pointer;
            encoding_string += st;
            str_pointer ++;
        }
        fread(read_pos, 1, 1, given_file);
    }
    fclose(given_file);
    cout << encoding_string << endl;
}


int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "You didn't type the filename. " << endl; 
        cout << "The command is './encode {{file}}'" << endl;
        return 0;
    }
    HuffCode huff;
    huff.encode(argv[1]);
}