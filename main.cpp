#include <iostream>
#include "HuffmanTree.h"

using namespace std;

int main(int argc, char* argv[]){
    if (argc != 4){
        cout << "ERROR -- Usage: prog4v2.exe <-huff/-unhuff> <source> "
                "<destination>" << endl;
    }else {
        cout << "Command: " << argv[1] << endl;
        cout << "Input Source: " <<argv[2] << endl;
        cout << "Output Source: " << argv[3] << endl;

        //If command is huff
        if (strcmp(argv[1], "-huff") == 0) {
            try{
                HuffmanTree huffmanTree(argv[2], argv[3]);
                huffmanTree.createPriorityQueue();
                huffmanTree.createHuffmanTree();
                huffmanTree.calcHuffCodes();
                huffmanTree.encodeSave();
            }catch(NoFile e){
                cout << "ERROR -- Cannot find input file\n";
            }

        //If command is unhuff
        } else if (strcmp(argv[1], "-unhuff") == 0) {
            HuffmanTree huffmanTree(argv[2], argv[3]);
            try {
                huffmanTree.recreateTree();
                huffmanTree.decodeSave();
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