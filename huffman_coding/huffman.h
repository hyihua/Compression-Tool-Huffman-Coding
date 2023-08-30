#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <cstdio>
using namespace std;

struct node{
	unsigned char c; // the character
	long int count; // the number of the character
	node* parent; 
	node* left_c;
	node* right_c;
	string coding; // the encoding string of this char

    node(){
		left_c = nullptr;
		right_c = nullptr;
		parent = nullptr;
    }

	node(int i) {
		count = i;
		parent = nullptr;
		left_c = nullptr;
		right_c = nullptr;
		coding = "";
	}

	node(unsigned char c, int i) {
		this->c = c;
		count = i;
		parent = nullptr;
		left_c = nullptr;
		right_c = nullptr;
		coding = "";
	}

	friend bool operator>(const node left, const node right){
		return left.count > right.count;
	}
	friend int operator+(const node left, const node right){
		return left.count + right.count;
	}
};

struct compare {
	bool operator() (node* left, node* right) {
		return ((left->count) > (right->count));
	}
};


class HuffCode {
private:
    // number of bytes in the file to be compressed.
	long int original_bytes = 0; 
    // number of bits in the compressed file.
	long int total_bits = 0;
    int unique_chars = 0;
	node* root;
	string coding_array[256];

	void write_byte_info(FILE *file, unsigned char cur_char, unsigned char &cur_byte, int cur_bits);
	unsigned char read_byte_info(FILE *file, unsigned char &cur_byte, int cur_bits);
    
public:
    HuffCode(){
		root = nullptr;
	}

	int get_unique_chars(){
		return unique_chars;
	}

	// compress and uncompress a given file
	void compress(char *filename);
	void decompress(char *filename, char *file2);
	void encode(char *filename);
	void decode(char *filename);
	
	long int chars_count[256];
	// 511 = 256 * 2 - 1, probably we don't need so many nodes, but there are at most so many nodes
	node info[511]; 

	// scan the file and get the encoding information
	void get_char_counts(FILE* given_file){

    	// obtain the number of bytes of the file
    	fseek(given_file, 0, SEEK_END);
    	original_bytes = ftell(given_file);
    	// back to the begining of the file
    	fseek(given_file, 0, SEEK_SET);

    	// initialize the array storing the number of each unique character
    	for (int i = 0; i < 256; i++){
        	chars_count[i] = 0;
    	}

    	// read the file and get the number of each unique character
    	unsigned char *cur_pos; 
    	unsigned char cur_c;
    	cur_pos = &cur_c;
    	fread(cur_pos, 1, 1, given_file);
    	for(long int i = 0; i < original_bytes; i++){
        	chars_count[cur_c] ++;
        	fread(cur_pos, 1, 1, given_file);
    	}

    	// get the number of unique chars
    	for (int i = 0; i < 256; i++){
        	// the number of the current char is at least one, meaning that char shows in the file
        	if(chars_count[i] > 0){
            	unique_chars ++;
        	}
    	}
    
    	fclose(given_file);
}

	// construct a huffman tree
	void buildHuffmanTree(FILE* given_file){
		get_char_counts(given_file);
    	// min heap
		priority_queue<node*, vector<node*>, compare> tree;

    	node* cur_node = info;
    
		for (long int *i = chars_count; i < (chars_count + 256); i++){
			// only include characters that appear.
			if (*i > 0){
				cur_node->c = (i - chars_count);
            	cur_node->count = *i;
            	cur_node->parent = nullptr;
            	cur_node->left_c = nullptr;
            	cur_node->right_c = nullptr;
				tree.push(cur_node);
            	cur_node++;
			}
		}

		node* Left;
		node* Right;
    	// build the huffman tree when there are at least two nodes left
		while (tree.size() > 1) {
			// obtain two chars with the smallest frequencies
			Left = tree.top();
			tree.pop();
			Right = tree.top();
			tree.pop();

			node* Parent = new node(Left->count + Right->count);
			Parent->left_c = Left;
			Parent->right_c = Right;
			Left->parent = Parent;
			Right->parent = Parent;

        	// adding the new non-left node into the info array
        	cur_node->count = Parent->count;
        	cur_node->left_c = Parent->left_c;
        	cur_node->right_c = Parent->right_c;
        	cur_node++;

			tree.push(Parent);
		}
		root = tree.top();	
		buildCode(root, "");

		// write the huffman tree -- the char, its coding length and its coding
    	for(node* cur_pos = info; cur_pos < (info + unique_chars); cur_pos++){
        	coding_array[cur_pos->c] = cur_pos->coding;
    	}
	}
private:
	// traverse the huffman tree, generate the code, and store the node information into the object
	void buildCode(node* root, string code){
		if (root == NULL)return;
		// leaf nodes
		if (root->left_c == NULL && root->right_c == NULL) {
			root->coding = code;
        	return;
		}
		buildCode(root->left_c, code + "0");
		buildCode(root->right_c, code + "1");
}
};
