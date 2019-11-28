/*
 * Name: Bryan Lee
 * Assignment: Program 4
 * Date: 11/26/19
 *
 * This program Compresses and Uncompresses files using Huffman's Encoding
 */
#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
using namespace std;

class IncorrectHuffman{};
class NoFile {};

struct huffman_node
{
    char id;
    //frequency of the character
    int freq;
    //HuffmanTree code for the character
    string code;
    huffman_node* left;
    huffman_node* right;
    huffman_node()
    {
        left = right = NULL;
    }
};

typedef huffman_node* node_ptr;

class HuffmanTree
{
protected:
    //array for 128characters in the Ascii Table
    node_ptr nodeArray[128];
    fstream inputFile, outputFile;
    node_ptr child, parent, root;
    char id;
    string in_file_name, out_file_name;

    //Comparator used for queue
    class compare{
    public:
        bool operator()(const node_ptr& c1, const node_ptr& c2) const
        {
            return c1->freq > c2->freq;
        }
    };

    //priority queue of frequency from high to low
    priority_queue<node_ptr, vector<node_ptr>, compare> pq;
    void createNodeArray();
    void traverse(node_ptr, string);
    int binary_to_decimal(basic_string<char>);
    string decimal_to_binary(int);
    inline void build_tree(string&, char);

public:
    HuffmanTree(string, string);
    void createPriorityQueue();
    void createHuffmanTree();
    void calcHuffCodes();
    void encodeSave();
    void decodeSave();
    void recreateTree();
};

#endif
