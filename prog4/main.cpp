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

        if (strcmp(argv[1], "-huff") == 0) {
            try{
                HuffmanTree h(argv[2], argv[3]);
                h.create_pq();
                h.create_huffman_tree();
                h.calculate_huffman_codes();
                h.coding_save();
                cout << endl;
            }catch(NoFile e){
                cout << "ERROR -- Cannot find input file\n";
            }

        } else if (strcmp(argv[1], "-unhuff") == 0) {

            HuffmanTree h(argv[2], argv[3]);
            try {
                h.recreate_huffman_tree();
                h.decoding_save();
                cout << endl;
            } catch (IncorrectHuffman e) {
                cout << "ERROR -- Text was not encoded with this Huffman "
                        "tree\n";
            }catch (NoFile e){
                cout << "ERROR -- Cannot find input file\n";
            }
        } else {
            cout << "\nERROR wrong command\n";
        }
    }

    return 0;
}