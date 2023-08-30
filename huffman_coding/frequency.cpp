#include "huffman.h"
using namespace std;

int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "You didn't type the filename. " << endl; 
        cout << "The command is './frequency {{file}}'" << endl;
        return 0;
    }

    FILE *given_file;
    given_file = fopen(argv[1], "rb");
    if(!given_file){
        cout << "The given file does not exist." << endl;
        return 0;
    }
    HuffCode huff;
    huff.get_char_counts(given_file);

    cout << "Unique characters: " << huff.get_unique_chars() << endl;
    for(int i = 0; i < 256; i++){
        if(huff.chars_count[i]){
            cout << "ASCII CODE: " << i << ", the character: " << (unsigned char)i << ", its frequency: " << huff.chars_count[i] << endl;
        } 
    }
}