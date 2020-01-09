#include "HuffmanTree.h"
#include <iostream>

//Creates each Node
void HuffmanTree::createNodeArray()
{
    for (int i = 0; i < 128; i++)
    {
        nodeArray[i] = new HuffmanNode;
        nodeArray[i]->id = i;
        nodeArray[i]->freq = 0;
    }
}

//Convert a 8-bit 0/1 string of binary code to a decimal integer
int HuffmanTree::binary_to_decimal(basic_string<char> in)
{
    int result = 0;
    for (int i = 0; i < in.size(); i++)
        result = result * 2 + in[i] - '0';
    return result;
}

//Convert a decimal integer to a 8-bit 0/1 string of binary code
string HuffmanTree::decimal_to_binary(int in)
{
    string temp = "";
    string result = "";
    while (in)
    {
        temp += ('0' + in % 2);
        in /= 2;
    }

    //Appends '0' to get length of 8 bits
    result.append(8 - temp.size(), '0');

    for (int i = temp.size() - 1; i >= 0; i--)
    {
        result += temp[i];
    }
    return result;
}

//Traverses the tree
void HuffmanTree::traverse(node_ptr node, string code)
{
    if (node->left == NULL && node->right == NULL){
        node->code = code;
    }
    else{
        traverse(node->left, code + '0');
        traverse(node->right, code + '1');
    }
}

//build the HuffmanTree tree according to information from file
inline void HuffmanTree::build_tree(string& path, char a_code){
    node_ptr current = root;
    for (int i = 0; i < path.size(); i++){
        if (path[i] == '0'){
            if (current->left == NULL)
                current->left = new HuffmanNode;
            current = current->left;
        }
        else if (path[i] == '1'){
            if (current->right == NULL)
                current->right = new HuffmanNode;
            current = current->right;
        }
    }
    //Creates leaf node
    current->id = a_code;
}

HuffmanTree::HuffmanTree(string in, string out)
{
    in_file_name = in;
    out_file_name = out;
    createNodeArray();
}
void HuffmanTree::createPriorityQueue()
{
    inputFile.open(in_file_name, ios::in);
    if(!inputFile.is_open()){
        throw NoFile();

    }
    inputFile.get(id);
    while (!inputFile.eof())
    {
        nodeArray[id]->freq++;
        inputFile.get(id);
    }
    inputFile.close();
    for (int i = 0; i < 128; i++)
    {
        if (nodeArray[i]->freq)
        {
            pq.push(nodeArray[i]);
        }
    }
}

void HuffmanTree::createHuffmanTree()
{
    priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
    while (temp.size() > 1)
    {
        //Create the HuffmanTree
        root = new HuffmanNode;
        root->freq = 0;
        root->left = temp.top();
        root->freq += temp.top()->freq;
        temp.pop();
        root->right = temp.top();
        root->freq += temp.top()->freq;
        temp.pop();
        temp.push(root);
    }
}

void HuffmanTree::calcHuffCodes()
{
    traverse(root, "");
}

void HuffmanTree::encodeSave()
{
    inputFile.open(in_file_name, ios::in);

    outputFile.open(out_file_name, ios::out | ios::binary);
    string in = "", str = "";

    //Saves the size of the priority queue
    in += (char)pq.size();
    priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
    while (!temp.empty())
    {
        //Get all characters and their HuffmanTree codes for output
        node_ptr current = temp.top();
        in += current->id;

        //Set the codes with a fixed 128-bit string form['0' + actual code]
        str.assign(127 - current->code.size(), '0');

        //Indicates the start of HuffmanTree code
        str += '1';
        str.append(current->code);
        in += (char)binary_to_decimal(str.substr(0, 8));
        for (int i = 0; i < 15; i++)
        {
            //Cuts string into 8-bit binary codes
            str = str.substr(8);
            in += (char)binary_to_decimal(str.substr(0, 8));
        }
        temp.pop();
    }
    str.clear();

    inputFile.get(id);
    while (!inputFile.eof())
    {
        //Get the HuffmanTree code
        str += nodeArray[id]->code;
        while (str.size() > 8)
        {
            //Cuts into 8-bit binary codes
            in += (char)binary_to_decimal(str.substr(0, 8));
            str = str.substr(8);
        }
        inputFile.get(id);
    }
    int count = 8 - str.size();
    if (str.size() < 8)
    {
        str.append(count, '0');
    }

    //Save number of 'count' at last
    in += (char)binary_to_decimal(str);
    in += (char)count;

    outputFile.write(in.c_str(), in.size());
    outputFile.write("6", 1);
    inputFile.close();
    outputFile.close();
}

void HuffmanTree::recreateTree()
{
    inputFile.open(in_file_name, ios::in | ios::binary);
    if(!inputFile.is_open()){
        throw NoFile();

    }
    //unsigned char to get number of node of tree
    unsigned char size;
    inputFile.read(reinterpret_cast<char*>(&size), 1);
    root = new HuffmanNode;
    for (int i = 0; i < size; i++)
    {
        char a_code;

        //16 unsigned char to obtain the binary code
        unsigned char h_code_c[16];
        inputFile.read(&a_code, 1);
        inputFile.read(reinterpret_cast<char*>(h_code_c), 16);
        string h_code_s = "";
        for (int i = 0; i < 16; i++)
        {
            //obtain the oringinal 128-bit binary string
            h_code_s += decimal_to_binary(h_code_c[i]);
        }
        int j = 0;
        while (h_code_s[j] == '0')
        {
            //deletes the added 0's to get the real HuffmanTree code
            j++;
        }

        h_code_s = h_code_s.substr(j + 1);
        build_tree(h_code_s, a_code);
    }
    inputFile.close();
}

void HuffmanTree::decodeSave()
{
    inputFile.open(in_file_name, ios::in | ios::binary);
    outputFile.open(out_file_name, ios::out);

    //get the size of HuffmanTree tree
    unsigned char size;
    inputFile.read(reinterpret_cast<char*>(&size), 1);

    inputFile.seekg(-1, ios::end);
    char magicNumber;

    //Reads magic number
    inputFile.read(&magicNumber, 1);
    if(magicNumber != '6'){
        throw IncorrectHuffman();
    }

    inputFile.seekg(-2, ios::end);

    char count0;
    inputFile.read(&count0, 1);

    //Jumps to where actual text starts
    inputFile.seekg(1 + 17 * size, ios::beg);
    vector<unsigned char> text;
    unsigned char textseg;
    inputFile.read(reinterpret_cast<char*>(&textseg), 1);
    while (!inputFile.eof())
    {
        //Gets the text byte by byte
        text.push_back(textseg);
        inputFile.read(reinterpret_cast<char*>(&textseg), 1);
    }

    text.pop_back();
    node_ptr current = root;
    string path;

    if(text.size() == 0){
        throw IncorrectHuffman();
    }

    for (int i = 0; i < text.size() - 1; i++)
    {
        //Translates the HuffmanTree code
        path = decimal_to_binary(text[i]);
        if (i == text.size() - 2) {
            path = path.substr(0, 8 - count0);
        }
        for (int j = 0; j < path.size(); j++){
            if (path[j] == '0')
                current = current->left;
            else
                current = current->right;
            if (current->left == NULL && current->right == NULL)
            {
                outputFile.put(current->id);
                current = root;
            }
        }
    }

    inputFile.close();
    outputFile.close();
}
