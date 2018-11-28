/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: wordladder.cpp
 * -------------------------
 * This program generally create a word ladder
 * between the first word and second word given by
 * user. The program also makes sure that it always
 * gives the shortest ladder.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "lexicon.h"
#include "queue.h"
#include "stack.h"

using namespace std;

/*Function prototypes*/
void printOutIntro();
string getFileName(ifstream &infile);
bool bothExist(string &startWord, string &endWord, Lexicon &dict);
bool isSameLength(string &startWord, string &endWord);
bool sameWord(string &startWord, string &endWord);
Stack<string> wordLadder(string &startWord, string &endWord, Lexicon &dict);
Set<string> findNeighbor(string &word, Lexicon &dict);
void startToPlay(string &startWord, string &endWord, Lexicon &dict);


int main() {
    ifstream infile;
    string fileName, startWord, endWord;

    printOutIntro();
    fileName = getFileName(infile);

    //Read the dictionary file into a lexicon
    Lexicon dict(fileName);

    while(true){
        startWord = getLine("Word #1 (or Enter to quit): ");
        if(startWord == "") break;
        startWord = toLowerCase(startWord);

        endWord = getLine("Word #2 (or Enter to quit): ");
        if(endWord == "") break;
        endWord = toLowerCase(endWord);

        startToPlay(startWord, endWord, dict);
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/* Function: printOutIntro
 * purpose: print out the intro welcome message
*/
void printOutIntro(){
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << " " << endl;
}

/* Function: getFileName
 * purpose: prompt the user for a valid file name.
 * When the file names given by the user is invalid,
 * re-prompt the user until a existing file is given.
*/
string getFileName(ifstream &infile){
    string fileName;

    while(true){
        fileName = promptUserForFile(infile, "Dictionary file name? ");
        if(fileExists(fileName)) break;
    }
    cout << " " << endl;
    return fileName;
}

/* Function: wordLadder
 * purpose: create a word ladder from the start word
 * to the end word using breadth-first search
*/
Stack<string> wordLadder(string &startWord, string &endWord, Lexicon &dict){
    Set<string> usedWords;
    Queue<Stack<string>> ladders;
    Stack<string> firstStep, toReturn;

    //Initialize the used words set and the create the first
    //stack in the ladders queue. Also initialize the ladder.
    usedWords.add(startWord);
    firstStep.push(startWord);
    ladders.enqueue(firstStep);

    while(!ladders.isEmpty()){
        Stack<string>currLadder = ladders.dequeue();

        //Look at the word on the top of the stack
        string topWord = currLadder.peek();

        //If the word is end word, a word ladder is found
        if(topWord == endWord){
            toReturn = currLadder;
            return toReturn;
        }else{
            //If the word is not end word, then find all valid neighbors of the word
            Set<string> validNeighbors = findNeighbor(topWord, dict);

            //Loop over all valid neighbor words and if it did not be used before
            //then copy the current stack and put the neighbor on the top. Then
            //add the copy stack to ladders queue.
            for(string word: validNeighbors){
                if(!usedWords.contains(word)){
                    Stack<string>nextLadder = currLadder;
                    nextLadder.push(word);
                    ladders.enqueue(nextLadder);
                    usedWords.add(word);
                }
            }

        }
    }
    return toReturn;
}

/* Function: findNeighbor
 * purpose: find all valid neighbors of a word where
 * the neighbors are words that have only one different
 * word from a given word
*/
Set<string> findNeighbor(string &word, Lexicon &dict){
    Set<string> neighbors;

    for(size_t i = 0; i < word.length(); i++){
        for(char ch = 'a'; ch <= 'z'; ch ++){
            string poNeighbor = word;
            poNeighbor[i] = ch;
            if(dict.contains(poNeighbor)){
                neighbors.add(poNeighbor);
            }
        }
    }
    return neighbors;
}

/* Function: startToPlay
 * purpose: start to play the word ladder
 */
void startToPlay(string &startWord, string &endWord, Lexicon &dict){
    Stack<string> ladder;

    //Check user input errors
    if(!isSameLength(startWord, endWord)){
        cout << "The two words must be the same length." << endl;
        cout << " " << endl;
    }else if(!bothExist(startWord, endWord, dict)){
        cout << "The two words must be found in the dictionary." << endl;
        cout << " " << endl;
    }else if(sameWord(startWord, endWord)){
        cout << "The two words must be different." << endl;
        cout << " " << endl;
    }else{
        ladder = wordLadder(startWord, endWord, dict);
        if(ladder.isEmpty()){
            cout << "No word ladder found from " + endWord + " back to "
                    + startWord + "." << endl;
        }else{
            cout << "A ladder from " + endWord + " back to " + startWord + ":" << endl;
            while(!ladder.isEmpty()){
                cout << ladder.pop() + " ";
            }
            cout << " " << endl;
        }
        cout << " " << endl;
    }
}

/* Function: bothExist
 * purpose: make sure that both words given by user are
 * in the dictionary
*/
bool bothExist(string &startWord, string &endWord, Lexicon &dict){
    return (dict.contains(startWord) && dict.contains(endWord));
}

/* Function: isSameLength
 * purpose: make sure that two words are the same length
*/
bool isSameLength(string &startWord, string &endWord){
    return startWord.length() == endWord.length();
}

/* Function: sameWord
 * purpose: make sure that two words not the same
*/
bool sameWord(string &startWord, string &endWord){
    return startWord == endWord;
}











