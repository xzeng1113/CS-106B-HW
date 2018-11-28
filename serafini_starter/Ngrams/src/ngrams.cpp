/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: ngrams.cpp
 * -------------------------
 * This program takes in a text file and then randomly
 * generates new text in the same style.
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
#include "queue.h"
#include "vector.h"
#include "map.h"
#include "random.h"

using namespace std;

/*Function prototypes*/
void printOutIntro();
string getFileName(ifstream &input);
int getNValue();
int getWordNum(int nValue);
Map<Queue<string>, Vector<string>> buildMap(ifstream &input, int nValue);
void randomScript(int nValue, Vector<Queue<string>> &windows, Map<Queue<string>, Vector<string>> &wordMap);
void printScript(Queue<string> &toReturn);

int main() {
    ifstream input;
    string fileName;
    int nValue;
    Map<Queue<string>, Vector<string>> wordMap;

    printOutIntro();
    fileName = getFileName(input);
    nValue = getNValue();
    wordMap = buildMap(input, nValue);
    Vector<Queue<string>> windows = wordMap.keys();
    randomScript(nValue, windows, wordMap);

    cout << "Exiting." << endl;
    return 0;
}

/* Function: printOutIntro
 * purpose: print out the intro welcome message
*/
void printOutIntro(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << " " << endl;
}

/* Function: getFileName
 * purpose: prompt the user for a valid file name.
 * When the file names given by the user is invalid,
 * re-prompt the user until a existing file is given.
*/
string getFileName(ifstream &input){
    string fileName;

    while(true){
        fileName = promptUserForFile(input, "Input file name? ");
        if(fileExists(fileName)) break;
    }
    return fileName;
}

/* Function: getNValue
 * purpose: get the N-value given by user, when N is less
 * than two, re-prompt the user until get a valid one
*/
int getNValue(){
    int nValue;

    while(true){
        nValue = getInteger("Value of N? ");
        if(nValue < 2){
            cout << "N must be 2 or greater." << endl;
        }else{
            cout << " " << endl;
            break;
        }
    }
    return nValue;
}

/* Function: buildMap
 * purpose: build the map from N-1 prefix to one suffix
*/
Map<Queue<string>, Vector<string>> buildMap(ifstream &input, int nValue){
    Map<Queue<string>, Vector<string>> wordMap;
    Queue<string> window, firstWindow;
    int winElem = nValue - 1;
    string word;

    while(input >> word){
        //Make sure there are always (N-1) elements in the window
        if(window.size() < winElem){
            window.enqueue(word);
            firstWindow.enqueue(word);
        }else{
            if(window.size() == winElem){
                if(wordMap.containsKey(window)){
                    Vector<string> suffix = wordMap.get(window);
                    suffix.add(word);
                    wordMap.put(window, suffix);
                }else{
                    Vector<string> suffix;
                    suffix.add(word);
                    wordMap.put(window, suffix);
                }
            }
            //Update window
            window.dequeue();
            window.enqueue(word);
        }
    }   
    //Wrap the map and do (N-1) more times iteration
    for(int i = 0; i < winElem; i ++){
        string wrapAround = firstWindow.dequeue();
        if(wordMap.containsKey(window)){
            Vector<string> suffix = wordMap.get(window);
            suffix.add(wrapAround);
            wordMap.put(window, suffix);
        }else{
            Vector<string> suffix;
            suffix.add(wrapAround);
            wordMap.put(window, suffix);
        }
        //Update the window
        window.dequeue();
        window.enqueue(wrapAround);
    }
    return wordMap;
}

int getWordNum(int nValue){
    int randomWords;
    string N = integerToString(nValue);

    while(true){
        randomWords = getInteger("# of random words to generate (0 to quit)? ");
        if(randomWords < nValue && randomWords != 0){
            cout << "Must be at least " + N + " words." << endl;
            cout << " " << endl;
        }else{
            break;
        }
    }
    return randomWords;
}

/* Function: randomScript
 * purpose: generate a random script using the map built before
*/
void randomScript(int nValue, Vector<Queue<string>> &windows, Map<Queue<string>, Vector<string>> &wordMap){
    int randomWords;

    while(true){
        randomWords = getWordNum(nValue);
        if(randomWords == 0) break;
        //Find the starting point
        int index = randomInteger(0, windows.size() - 1);
        Queue<string> window = windows[index];
        Queue<string> toReturn = window;

        for(int n = (nValue - 1); n < randomWords; n++){
            //Obtain a random surffix
            Vector<string> suffixes = wordMap.get(window);
            int surIndex = randomInteger(0, suffixes.size() - 1);
            string suffix = suffixes[surIndex];
            toReturn.enqueue(suffix);

            //Update window
            window.dequeue();
            window.enqueue(suffix);
        }
        printScript(toReturn);
    }
}

/* Function: printScript
 * purpose: print out the random script
*/
void printScript(Queue<string> &toReturn){
    cout << "... ";
    while(!toReturn.isEmpty()){
        cout << toReturn.dequeue() + " ";
    }
    cout << " ..." << endl;
    cout << " " << endl;
}
