#include "huffman.h"
using namespace std;

unsigned char HuffCode::read_byte_info(FILE *file, unsigned char &cur_byte, int cur_bits){
    unsigned char read_byte, shift_byte, get_c;
    int remaining_bits = 8 - cur_bits;
    fread(&read_byte, 1, 1, file);
    shift_byte = read_byte >> cur_bits;
    get_c = cur_byte | shift_byte;
    cur_byte = read_byte << remaining_bits;
    return get_c;
}


void HuffCode::decompress(char *filename, char* new_file){
    FILE *given_file;
    FILE *decoded_file;
    given_file = fopen(filename, "rb");
    if(!given_file){
        cout << "The given file does not exist." << endl;
        return;
    }

    decoded_file = fopen(new_file, "rb");
    if(decoded_file){
        cout << new_file << " exists. Please try to type a different name to avoid overwritting." << endl;
        return;
    }

    long int size_original_file = 0;
    unsigned char s;
    fread(&s, 1, 1, given_file);
    int read_num = 1;
    size_original_file += s;
    long int times = 256;
    while(read_num < 8){
        fread(&s, 1, 1, given_file);
        read_num += 1;
        size_original_file += (s * times);
        times *= 256;
    }

    // this byte stores the number of chars shown in the original file
    fread(&unique_chars, 1, 1, given_file);
    // indicates that the highest 1 is not saved
    if(unique_chars == 0){
        unique_chars = 256;
    }

    unsigned char cur_byte_info = 0;
    unsigned char cur_char;
    int used_bits = 0;
    int cur_char_len;

    node *tmp;
    node *tree_root = new node;
    

    unsigned char one_two_eight = 0b10000000;
    for (int i = 0; i < unique_chars; i++){
        cur_char = read_byte_info(given_file, cur_byte_info, used_bits);
        cur_char_len = read_byte_info(given_file, cur_byte_info, used_bits);

        if(cur_char_len == 0){
            cur_char_len = 256;
        }

       tmp = tree_root;
       for (int j = 0; j < cur_char_len; j++){
            if(used_bits == 0){
               fread(&cur_byte_info, 1, 1, given_file);
               used_bits = 8;
            }
            // indicates the value of the highest bit is 0, should move to the left sub-tree
            if((cur_byte_info & one_two_eight) == 0){
                if(tmp->left_c){
                    tmp = tmp->left_c;
                }
                else{
                    tmp->left_c = new node;
                    tmp = tmp->left_c;
                }
            }
            // indicates the value of the highest bit is 1, should move to the right sub-tree
            else{
                if(tmp->right_c){
                    tmp = tmp->right_c;
                }
                else{
                    tmp->right_c = new node;
                    tmp = tmp->right_c;
                }
            }
            cur_byte_info <<= 1;
            used_bits --;
       }
       
       tmp->c = cur_char;
    }


    decoded_file = fopen(new_file, "wb");
    for(long int i = 0; i < size_original_file; i++){
        if (feof(given_file)){
            break;
        }
        tmp = tree_root;
        while(tmp->left_c || tmp->right_c){
            if(used_bits == 0){
                fread(&cur_byte_info, 1, 1, given_file);
                used_bits = 8;
            }
            // indicates the value of the highest bit is 0, should move to the left sub-tree
            if((cur_byte_info & one_two_eight) == 0){
                tmp = tmp->left_c;
            }
            // indicates the value of the highest bit is 1, should move to the right sub-tree
            else{
                tmp = tmp->right_c;
            }
            cur_byte_info <<= 1;
            used_bits --;
        }
        fwrite((&(tmp->c)), 1, 1, decoded_file);
    }
    fclose(decoded_file);
    fclose(given_file);
}

int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "You didn't type the filenames. " << endl; 
        cout << "The command is './decompress {{file}} {{file}}'" << endl;
        cout << "The first file is the compressed file and the second is the name for the decompressed file." << endl;
        return 0;
    }
    if (argc == 2){
        cout << "You didn't type enough parameters. " << endl; 
        cout << "The command is './decompress {{file}} {{file}}'" << endl;
        cout << "The first file is the compressed file and the second is the name for the decompressed file." << endl;
        return 0;
    }
    HuffCode huff;
    huff.decompress(argv[1], argv[2]);
}