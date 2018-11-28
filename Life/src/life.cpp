/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: life.cpp
 * -------------------------
 * This program generally simulates the life cycle
 * of bacteria. The initial pattern is given by
 * users and then simulates the birth and death of
 * future generations of bacteria cells.
 * The "rotateGrid" project (Chris mentioned in class)
 * was used as a reference when writing this code.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"

using namespace std;

/*Function prototypes*/
void printOutIntro();
string getFileName(ifstream &infile);
string wrapResponse();
string getAction();
void readInGrid(ifstream &infile, Grid<char> &firstGe);
void printGrid(Grid<char> &grid);
Grid<char> animate(Grid<char> &state, string &wrapCheck);
Grid<char> update(Grid<char> &preState, string &wrapCheck);
int nonWrapNum(Grid<char> &state, int r, int c);
int wrapNum(Grid<char> &state, int r, int c);
void createNext(Grid<char> &state, Grid<char> &newState, int counter, int r, int c);

const int PAUSE_TIME = 50;

int main() {
    ifstream infile;
    string fileName, wrapCheck, action;
    Grid<char> state, newState;

    printOutIntro();
    fileName = getFileName(infile);
    wrapCheck = wrapResponse();
    readInGrid(infile, state);
    printGrid(state);

    while(true){
        action = getAction();

        //Implement the main menu of the program
        if(action == "a"){
            newState = animate(state, wrapCheck);
            state = newState;
        }else if(action == "t"){
            newState = update(state, wrapCheck);
            printGrid(newState);
            state = newState;
        }else if(action == "q"){
            break;
        }

    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

/* Function: printOutIntro
 * purpose: print out the intro welcome message
*/
void printOutIntro(){
   cout << "Welcome to the CS 106B Game of Life," << endl;
   cout << "a simulation of the lifecycle of a bacteria colony." << endl;
   cout << "Cells (X) live and die by the following rules:" << endl;
   cout << "- A cell with 1 or fewer neighbors dies." << endl;
   cout << "- Locations with 2 neighbors remain stable." << endl;
   cout << "- Locations with 3 neighbors will create life." << endl;
   cout << "- A cell with 4 or more neighbors dies." << endl;
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
        fileName = promptUserForFile(infile, "Grid input file name? ");
        if(fileExists(fileName)) break;
    }
    return fileName;
}

/* Function: wrapResponse
 * purpose: prompt the user for his/her choice of wrap.
 * The valid response is constraint to y/n. If the user
 * type in invalid reponse, re-prompt until the user
 * gives a valid one.
*/
string wrapResponse(){
    string wrapResponse;
    bool illegalResponse = true;

    while(illegalResponse){
        wrapResponse = getLine("Should the simulation wrap around the grid (y/n)? ");
        wrapResponse = toLowerCase(wrapResponse);

        if(wrapResponse == "y" || wrapResponse == "n"){
            illegalResponse = !illegalResponse;
        }else{
            cout << "Illegal response. Please try again." << endl;
        }
    }

    return wrapResponse;
}

/* Function: readInGrid
 * purpose: read the initial pattern of the
 * grid given by the file
*/
void readInGrid(ifstream &infile, Grid<char> &firstGe){
    string line;
    int rows, cols;
    getline(infile, line);
    rows = stringToInteger(line);
    getline(infile, line);
    cols = stringToInteger(line);
    firstGe.resize(rows, cols);
    for(int r = 0; r < rows; r++){
        getline(infile, line);
        for(int c = 0; c < cols; c++){
            firstGe[r][c] = line[c];
        }
    }
    infile.close();
}

/* Function: printGrid
 * purpose: print the grid to the console
*/
void printGrid(Grid<char> &grid){
    for(int r = 0; r < grid.numRows(); r++){
        for(int c = 0; c < grid.numCols(); c++){
            cout << grid[r][c];
        }
        cout << endl;
    }
}

/* Function: getAction
 * purpose: ask the user for an action to
 * be taken to start the simulation
*/
string getAction(){
    string action;
    while(true){
        action = getLine("a)nimate, t)ick, q)uit? ");
        action = toLowerCase(action);
        if(action == "a" || action == "t" || action == "q"){
            break;
        }else{
            cout << "Invalid choice; please try again." << endl;
        }
    }
    return action;
}

/* Function: update
 * purpose: update the state of the grid
*/
Grid<char> update(Grid<char> &state, string &wrapCheck){
    Grid<char> newState;
    int gridHeight = state.numRows();
    int gridWidth = state.numCols();
    int neighborNum;
    newState.resize(gridHeight, gridWidth);
    for(int r = 0; r < gridHeight; r++){
        for(int c = 0; c < gridWidth; c++){
            if(wrapCheck == "y"){
                neighborNum = wrapNum(state, r, c);
            }else{
                neighborNum = nonWrapNum(state, r, c);
            }
            createNext(state, newState, neighborNum, r, c);
        }
    }
    return newState;
}

/* Function: nonWrapNum
 * purpose: get the number living cells in the surrounding
 * eight neighbors when the user choosing non-wrap mode
*/
int nonWrapNum(Grid<char> &state, int r, int c){
    int counter = 0;
    char self = state[r][c];

    for(int surRow = r - 1; surRow <= r + 1; surRow ++){
        for(int surCol = c - 1; surCol <= c + 1; surCol ++){
            if(state.inBounds(surRow, surCol)){
                char ch = state[surRow][surCol];
                if(ch == 'X'){
                    counter ++;
                }
            }
        }
    }

    //cross off the given cell itself

    if(self == 'X'){
        counter --;
    }

    return counter;
}

/* Function: wrapNum
 * purpose: get the number living cells in the surrounding
 * eight neighbors when the user choosing wrap mode
*/
int wrapNum(Grid<char> &state, int r, int c){
    int counter = 0;
    int neighRow, neighCol;
    char self = state[r][c];
    for(int surRow = r - 1; surRow <= r + 1; surRow ++){
        for(int surCol = c - 1; surCol <= c + 1; surCol ++){
            if(state.inBounds(surRow, surCol)){
                neighRow = surRow;
                neighCol = surCol;
            }else{
                neighRow = (surRow + state.numRows()) % state.numRows();
                neighCol = (surCol + state.numCols()) % state.numCols();
            }
            char ch = state[neighRow][neighCol];
            if(ch == 'X'){
                counter ++;
            }
        }
    }
    if(self == 'X'){
        counter --;
    }
    return counter;
}

/* Function: createNext
 * purpose: create the next generation of bacteria
 * cells based certain rules
*/
void createNext(Grid<char> &state, Grid<char> &newState, int counter, int r, int c){
    if(counter == 0 || counter == 1 || counter >= 4){
        newState[r][c] = '-';
    }else if(counter == 2){
        newState[r][c] = state[r][c];
    }else if(counter == 3){
        if(state[r][c] == '-'){
            newState[r][c] = 'X';
        }else if(state[r][c] == 'X'){
            newState[r][c] = state[r][c];
        }
    }
}

/* Function: animate
 * purpose: implement the animation loop
*/
Grid<char> animate(Grid<char> &state, string &wrapCheck){
    int frames;
    string framesStr;
    bool illegal = true;
    Grid<char> newState;

    while(illegal){
        framesStr = getLine("How many frames? ");
        if(stringIsInteger(framesStr)){
            frames = stringToInteger(framesStr);
            illegal = !illegal;
        }else{
            cout << "Illegal integer format. Try again." << endl;
        }
    }

    for(int i = 0; i < frames; i++){
        clearConsole();
        newState = update(state, wrapCheck);
        printGrid(newState);
        state = newState;
        pause(PAUSE_TIME);
    }

    return state;
}
