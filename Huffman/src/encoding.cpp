/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: encoding.cpp
 * -------------------------
 * This program implements standard Huffman algorithm to compress
 * and decompress files.
*/

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"
#include "string.h"


/*Function Prototype*/
void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string> &encodingMap, string soFar);
void decodeDataHelper(HuffmanNode *encodingTree, string &decode, string &toReturn);

/* Function: buildFrequencyTable
 * Purpose: Count the occurance of each characters in the file and
 * store them in a map
*/
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while(true){
        int character = input.get();
        if(character != -1){
            if(freqTable.isEmpty()){
                freqTable[character] = 1;
            }else{
                if(freqTable.containsKey(character)){
                    freqTable[character]++;
                }else{
                    freqTable[character] = 1;
                }
            }
        }else{
            break;
        }
    }
    //Add single occurance of PSEUDO_EOF to represent the end of the file
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}

/* Function: buildEncodingTree
 * Purpose: create a Huffman coding tree base on the frequency of each character
*/
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> encodingQueue;
    HuffmanNode *front = nullptr;
    //Note that spec says the map contains at least one key/value pair, so this program does not consider the empty map scenario
    for(int key: freqTable){
        HuffmanNode *newNode = new HuffmanNode;
        newNode->character = key;
        newNode->count = freqTable[key];
        encodingQueue.enqueue(newNode, freqTable[key]);
    }
    while(encodingQueue.size() > 1){
        HuffmanNode *leftChild = encodingQueue.dequeue();
        HuffmanNode *rightChild = encodingQueue.dequeue();
        HuffmanNode *newNode = new HuffmanNode;
        newNode->character = NOT_A_CHAR;
        newNode->count = leftChild->count + rightChild->count;
        newNode->zero = leftChild;
        newNode->one = rightChild;
        encodingQueue.enqueue(newNode, newNode->count);
    }
    //Dequeue one last node, and make the pointer point to this node
    if(encodingQueue.size() == 1){
        HuffmanNode *lastNode = encodingQueue.dequeue();
        front = lastNode;
    }
    return front;
}

/* Function: buildEncodingMap
 * Purpose: Build a Huffman coding map from each character to its binary encoding
*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree){
    Map<int, string> encodingMap;
    string soFar = "";
    //If the encoding tree is nullptr, then return an empty map
    if(encodingTree == nullptr){
        return encodingMap;
    }
    //If the root is PSEUDO_EOF, then the map would only have one element
    if(encodingTree->character == PSEUDO_EOF){
        encodingMap[PSEUDO_EOF] = "";
    }else{
        buildEncodingMapHelper(encodingTree, encodingMap, soFar); //Call recursive helper function
    }
    return encodingMap;
}

/* Function: encodeData
 * Purpose:Encode the input file using the Huffman coding map to form a
 * shorter binary representation
*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    rewindStream(input);
    string encode = "";
    //Encode each character to binary using the encoding map
    while(true){
        int character = input.get();
        if(character == -1){
            break;
        }else{
            encode += encodingMap[character];
        }
    }
    encode += encodingMap[PSEUDO_EOF];
    while(encode.length() != 0){
        string oneBit = encode.substr(0, 1);
        output.writeBit(stringToInteger(oneBit));
        encode = encode.substr(1);
    }
}

/* Function: decodeData
 * Purpose: Decode the file data into the original uncompressed contents.
*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    string decode = "";
    //Read in the data to be decoded
    while(true){
        int oneBit = input.readBit();
        if(oneBit == -1){
            break;
        }else{
            decode += integerToString(oneBit);
        }
    }
    while(encodingTree != nullptr && decode.length() != 0){
        string toReturn = "";
        decodeDataHelper(encodingTree, decode, toReturn); //Call recursive helper function to find each charater
        if(toReturn != ""){
            int putInt = stringToInteger(toReturn);
            if(putInt == PSEUDO_EOF){
                break;
            }
            output.put(putInt);
        }
    }
}

/* Function: compress
 * Purpose: Overall function to implement the functio of compression a file using
 * the functions defined before
*/
void compress(istream& input, obitstream& output) {
    Map<int, int> frequencyTable = buildFrequencyTable(input);
    output << frequencyTable;
    HuffmanNode *encodingTree = buildEncodingTree(frequencyTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    freeTree(encodingTree);
    encodeData(input, encodingMap, output);
}

/* Function: decompress
 * Purpose: Overall function to implement the functio of decompression a file using
 * the functions defined before
*/
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> frequencyTable;
    input >> frequencyTable;
    HuffmanNode *decoding = buildEncodingTree(frequencyTable);
    decodeData(input, decoding, output);
    free(decoding);
}

/* Function: freeTree
 * Purpose: Free the memory associated with the root of the tree and the all the nodes
 * in the subtrees
*/
void freeTree(HuffmanNode* node) {
    if(node != nullptr){
        freeTree(node->zero); //Recursively free the memory of the nodes in left subtree
        freeTree(node->one); //Recursively free the memory of the nodes in left subtree
        delete node;
    }
}

/* Function: buildingEncodingMapHelper
 * Purpose: Recursive helper function, help to build the encoding the tree
 * by recursively walking through the binary tree to find the shorter binary
 * representation of each character
*/
void buildEncodingMapHelper(HuffmanNode* encodingTree, Map<int, string> &encodingMap, string soFar){
    //Base Case
    if(encodingTree->zero == nullptr && encodingTree->one == nullptr){
        encodingMap[encodingTree->character] = soFar;
        return;
    }else{
        //Recursive call
        buildEncodingMapHelper(encodingTree->zero, encodingMap, soFar + "0");
        buildEncodingMapHelper(encodingTree->one, encodingMap, soFar + "1");
    }
}

/* Function: decodeDataHelper
 * Purpose:  Recursive helper function, recursively walking through the tree
 * to find each character
*/
void decodeDataHelper(HuffmanNode *encodingTree, string &decode, string &soFar){
    //Base Case
    if(decode.length() == 0) return;
    if(encodingTree->zero == nullptr && encodingTree->one == nullptr){
        soFar += integerToString((encodingTree->character));
        return;
    }
    //Recursive call
    if(decode.substr(0, 1) == "0"){
        decode = decode.substr(1);
        decodeDataHelper(encodingTree->zero, decode, soFar);
    }else if(decode.substr(0, 1) == "1"){
        decode = decode.substr(1);
        decodeDataHelper(encodingTree->one, decode, soFar);
    }
}

