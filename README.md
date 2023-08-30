# Project introduction
This is a project of designing and implementation of data compression tool based on Huffman Code. There are a total of six functions/tools in this project and the compilation rules for these six functions are describled in the file Makefile. After making any modifications to the files written in the Makefile, you need to run *make all* first in the terminal.

# Files introduction
* For the below mentioned commands, the given files must be in the same directory as the .cpp files.

## huffman.h
This header file includes the struct Node, compare and class HuffCode.

## frequency.cpp & frequency
File frequency is the executable file of file frequency.cpp. To run the tool to get (byte) symbol frequency stats from a file, use command *./frequency {filename}*. 

## buildTree.cpp & buildTree
File buildTree is the executable file of file buildTree.cpp. To run the tool to construct a Huffman encoding table and its matching decoding table from a given symbol frequency stats, use command *./buildTree {filename}*. 

## encoding.cpp & encode
File encode is the executable file of file encoding.cpp. To run the tool to encode a file for a given Huffman encoding table, use command *./encode {filename}*. 

## decoding.cpp & decode
File decode is the executable file of file decoding.cpp. To run the tool to decode a Huffman encoded file, use command *./decode {filename}*. 

*Note: the file given here needs to be a compressed file.*

## compress.cpp & compress
File compress is the executable file of file compress.cpp. To run the tool to compress a file, use command *./compress {filename}*. 

## decompress.cpp & decompress
File decompress is the executable file of file decompress.cpp. To run the tool to decompress a compressed file, use command *./compress {compressed filename} {filename for decompressed file}*. The first file is a compressed file and the second filename is the name of the decompressed file to avoid overwritting an existing file with the same name in the directory.