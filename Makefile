all : frequency buildTree encode decode compress decompress

frequency : frequency.cpp
	g++ -std=c++14 frequency.cpp -o frequency

buildTree : buildTree.cpp
	g++ -std=c++14 buildTree.cpp -o buildTree

encode : encoding.cpp
	g++ -std=c++14 encoding.cpp -o encode

decode : decoding.cpp
	g++ -std=c++14 decoding.cpp -o decode

compress : compress.cpp
	g++ -std=c++14 compress.cpp -o compress

decompress : decompress.cpp
	g++ -std=c++14 decompress.cpp -o decompress
