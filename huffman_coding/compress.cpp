#include "huffman.h"
using namespace std;

/*
cur_char: represent the current char which is the next to be dealed with, including the chars and their lengths
cur_byte: represent the current byte which is the next to be written, including the info with cur_char and the previous cur_byte
cur_bits: represent the number of bits used in 8 bits
*/
void HuffCode::write_byte_info(FILE *file, unsigned char read_char, unsigned char &cur_byte, int cur_bits){
    int remaining_bits = 8 - cur_bits;
    unsigned char shift_char;
    shift_char = read_char >> cur_bits;
    cur_byte <<= remaining_bits;
    cur_byte |= shift_char;
    fwrite(&cur_byte, 1, 1, file);
    cur_byte = read_char;
}

void HuffCode::compress(char *filename) {

    FILE *given_file;
    given_file = fopen(filename, "rb");
    if(!given_file){
        cout << "The given file does not exist." << endl;
        return;
    }

    // construct a huffman tree and the coding map
	buildHuffmanTree(given_file);
    given_file = fopen(filename, "rb");
    fseek(given_file, 0, SEEK_SET);

    // get the name of the compressed file.
    string compressed_file_name = string(filename) + ".compress";    
    FILE *compressed_file = fopen(&compressed_file_name[0], "wb");

    // write the size of the original file.
    fwrite(&original_bytes, 8, 1, compressed_file);
    total_bits += 64; // 8 * sizeof(original_bytes);


    // write the number of unique chars apppearing in the file into the compressed file.
    fwrite(&unique_chars, 1, 1, compressed_file);
    total_bits += 8;

    int used_bits = 0;
    unsigned char cur_byte_info, cur_char, cur_char_len;
    char *coding_pointer;

    // write the huffman tree
    for(node* cur_pos = info; cur_pos < (info + unique_chars); cur_pos++){
        cur_char_len = cur_pos->coding.length();
        cur_char = cur_pos->c;
        // write the charachter, occupied one byte
        write_byte_info(compressed_file, cur_char, cur_byte_info, used_bits);
        // write the length of the Huffman coding of the character, occupied one byte
        write_byte_info(compressed_file, cur_char_len, cur_byte_info, used_bits);
        total_bits += 16;

        coding_pointer = &cur_pos->coding[0];
        // write the Huffman coding of the character, occupied (int)cur_char_len bits.
        while(*coding_pointer){
            // write the current byte info into the compressed file when there is 8-bit info
            if(used_bits == 8){
                fwrite(&cur_byte_info, 1, 1, compressed_file);
                used_bits = 0;
            }

            // deal with chars 0/1
            cur_byte_info <<= 1;
            used_bits ++;
            if(*coding_pointer == '1'){
                cur_byte_info |= 1;
            }
            
            coding_pointer ++;
        }
        total_bits += cur_char_len;
        // record the number of bits each char occurs.
        total_bits += (cur_pos->count * cur_char_len); 
    }

    if(used_bits == 8){
        fwrite(&cur_byte_info, 1, 1, compressed_file);
        used_bits = 0;
    }

    unsigned char *read_pos, read_char; 
    char *str_pointer;
    read_pos = &read_char;
    fread(read_pos, 1, 1, given_file);
    for (long int i = 0; i < original_bytes; i++){
        str_pointer = &coding_array[read_char][0];
        while(*str_pointer){
            if(used_bits == 8){
                fwrite(&cur_byte_info, 1, 1, compressed_file);
                used_bits = 0;
            }

            cur_byte_info <<= 1;
            used_bits ++;
            if(*str_pointer == '1'){
                cur_byte_info |= 1;
            }
            str_pointer ++;
        }
        fread(read_pos, 1, 1, given_file);
    }
    fclose(given_file);

    if(used_bits == 8){
        fwrite(&cur_byte_info, 1, 1, compressed_file);
    }else{
        cur_byte_info <<= (8 - used_bits);
        fwrite(&cur_byte_info, 1, 1, compressed_file);
    }
    fclose(compressed_file);

    // compute the compression rate
    long int compressed_bytes = total_bits / 8;
    if(total_bits % 8 != 0){
        compressed_bytes += 1;
        total_bits = compressed_bytes * 8;
    }
    cout << "The size of the original file is: " << original_bytes << " (bytes)" << endl;
    cout << "The size of the compressed file is: " << compressed_bytes << " (bytes)" << endl;
    cout << "The compression rate is: " << (compressed_bytes * 1.0) / original_bytes * 100 << "%." << endl;
    cout << "The compressed file is: " << compressed_file_name << endl << endl;
}


int main(int argc, char *argv[]){
    if (argc == 1){
        cout << "You didn't type the filename. " << endl; 
        cout << "The command is './compress {{file}}'" << endl;
        return 0;
    }
    HuffCode huff;
    huff.compress(argv[1]);
}