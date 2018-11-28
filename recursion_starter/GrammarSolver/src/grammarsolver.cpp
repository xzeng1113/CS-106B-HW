/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Xiaoxi Zeng
 * Section leader: Semir Shafi
 * This prgram generates grammatically correct sentences for a given symbol
 * a certain number of times given a BNF input file.
 */

#include "grammarsolver.h"
#include "map.h"
#include "random.h"


using namespace std;

const string delimiter = "::=";

/*Function prototypes*/
string grammarHelper(string symbol, Map<string, Vector<string>> &grammar);
string grammarHelper(string symbol, string toReturn, Map<string, Vector<string>> &grammar);


/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    string line;
    Map<string, Vector<string>> grammar;
    int delimiterLength = delimiter.length();
    Vector<string> sentences;

    //Read in the input file and store it into a Map
    while(getline(input, line)){
        int index = stringIndexOf(line, "::=");
        string nonTerminal = line.substr(0, index);
        string rule = line.substr(index + delimiterLength);
        Vector<string> rules = stringSplit(rule, "|");

        //Throw an exception for duplicate non-terminal
        if(grammar.containsKey(nonTerminal)){
            throw("Illegal input. Dulpicate nonterminal.");
        }else{
            grammar.put(nonTerminal, rules);
        }
    }

    //Throw an exception for an empty symbol
    if(symbol == " "){
        throw("Empty symbol.");
    }

    //Generate a certain times of grammar
    if(times <= 0){
        return {};
    }else{
        for(int i = 0; i < times; i++){
        string sentence = grammarHelper(symbol, grammar);
        sentence = trim(sentence);
        sentences.add(sentence);
        }
        return sentences;
    }
}

/* Function: grammarHelper
 * purpose: generate grammar
*/
string grammarHelper(string symbol, Map<string, Vector<string>> &grammar){
    return grammarHelper(symbol, " ", grammar);
}

/* Function: grammarHelper
 * purpose: using recursion to generate grammar, using toReturn
 * to keep track of the return string
*/
string grammarHelper(string symbol, string toReturn, Map<string, Vector<string>> &grammar){
    //Base Case
    if(!grammar.containsKey(symbol)){
         toReturn = toReturn + symbol + " ";
         return toReturn;
    }

    Vector<string> rules = grammar.get(symbol);
    //Randomly choose a rule
    int index = randomInteger(0, rules.size() - 1);
    string rule = rules[index];
    Vector<string> R = stringSplit(rule, " ");

    //Recursive call
    for(string str: R){
        toReturn = grammarHelper(str, toReturn, grammar);
    }
    return toReturn;
}



