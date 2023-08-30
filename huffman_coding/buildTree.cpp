#include "huffman.h"
using namespace std;

int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "You didn't type the filename. " << endl; 
        cout << "The command is './buildTree {{file}}'" << endl;
        return 0;
    }

    FILE *given_file;
    given_file = fopen(argv[1], "rb");
    if(!given_file){
        cout << "The given file does not exist." << endl;
        return 0;
    }

    HuffCode huff;
    huff.buildHuffmanTree(given_file);
    // print the tree
    for (node* cur_node = huff.info; cur_node < (huff.info + huff.get_unique_chars()); cur_node++){
        cout << "The character: " << (cur_node->c) << ", its coding: " << (cur_node->coding) << endl;
    }
}