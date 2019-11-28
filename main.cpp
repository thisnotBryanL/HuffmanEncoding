/*
 * Name: Bryan Lee
 * Assignment: Program 4
 * Date: 11/26/19
 *
 * This program Compresses and Uncompresses files using Huffman's Encoding
 */
#include <iostream>
#include "HuffmanTree.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cout << "ERROR -- Usage: prog4v2.exe <-huff/-unhuff> <source> "
                "<destination>" << endl;
    }else {

        cout << "Command: " << argv[1] << endl;
        cout << "Input Source: " <<argv[2] << endl;
        cout << "Output Source: " << argv[3] << endl;

        //If command is huff
        if (strcmp(argv[1], "-huff") == 0) {
            try{
                HuffmanTree h(argv[2], argv[3]);
                h.createPriorityQueue();
                h.createHuffmanTree();
                h.calcHuffCodes();
                h.encodeSave();
                cout << endl;
            }catch(NoFile e){
                cout << "ERROR -- Cannot find input file\n";
            }

        //If command is unhuff
        } else if (strcmp(argv[1], "-unhuff") == 0) {
            HuffmanTree h(argv[2], argv[3]);
            try {
                h.recreateTree();
                h.decodeSave();
                cout << endl;
            } catch (IncorrectHuffman) {
                cout << "ERROR -- Text was not encoded with this Huffman "
                        "tree\n";
            }catch (NoFile){
                cout << "ERROR -- Cannot find input file\n";
            }
        } else {
            cout << "\nERROR wrong command\n";
        }
    }

    return 0;
}