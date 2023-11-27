//
//  main.cpp
//  picture
//
//  Created by Raymond Kao on 10/25/23.
//
#include <cctype>
#include <string>
#include "grid.h"
#include <cassert>
#include <iostream>
using namespace std;

void plotHorizontalLine(int r, int c, int distance, char ch);
void plotVerticalLine(int r, int c, int distance, char ch);

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

bool workingString(string commandString, int& badPos);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int main()
{
    for (;;)
        {
            cout << "Enter the number of grid rows and columns (max 30 each): ";
            int nRows;
            int nCols;
            cin >> nRows >> nCols;
            cin.ignore(10000, '\n');
            if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
            {
                setSize(nRows, nCols);
                break;
            }
            cout << "The numbers must be between 1 and 30." << endl;
        }
        char currentChar = '*';
        int currentMode = FG;
        for (;;)
        {
            cout << "Enter a command string (empty line to quit): ";
            string cmd;
            getline(cin, cmd);
            if (cmd == "")
                break;
            int position;
            int status = performCommands(cmd, currentChar, currentMode, position);
            switch (status)
            {
              case 0:
                draw();
                break;
              case 1:
                cout << "Syntax error at position " << position << endl;
                break;
              case 2:
                if (!isprint(currentChar))
                    cout << "Current character is not printable" << endl;
                if (currentMode != FG  &&  currentMode != BG)
                    cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
                break;
              case 3:
                cout << "Cannot perform command at position " << position << endl;
                break;
              default:
                  // It should be impossible to get here.
                cout << "performCommands returned " << status << "!" << endl;
            }
        }
}
void plotHorizontalLine(int r, int c, int distance, char ch){
    for (int i = 0; i < distance; i++) {
        if(distance >= 0) {
            setChar(r, c+i , ch);
        }
    }
}
void plotVerticalLine(int r, int c, int distance, char ch) {
    for(int i = 0; i < distance; i++) {
        if(distance >= 0) {
            setChar(r+i, c, ch);
        }
    }
}
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
    if (dir != HORIZ && dir != VERT) { //starting are all needed safety checks before the action
        return false;
    }
    if (fgbg != FG && fgbg != BG) {
        return false;
    }
    if (!isprint(plotChar)){
        return false;
    }
    if(dir == HORIZ) {
        if (r > getRows() || r <= 0) {
            return false;
        }
        if (c > getCols() || c <= 0) {
            return false;
        }
        if (c + distance > getCols() || c + distance <= 0) { //returns false if out of bounds
            return false;
        }
        int x = 0;
        if (distance >= 0) {
            while (x <= distance){
                if (fgbg == FG) {
                    setChar(r, c+x, plotChar);
                }
                else if (fgbg == BG && getChar(r, c+x) == ' ') {
                    setChar(r, c+x, plotChar);
                }
                x += 1;
            }
        }
        else {
            while (x <= abs(distance)) { //because negative I absolute value because it is easier for me to comprehend that way
                if (fgbg == FG) {
                    setChar(r, c-x, plotChar);
                }
                else if (fgbg == BG) {
                    if (getChar(r, c-x) == ' '){
                        setChar(r, c-x , plotChar);
                    }
                }
                x += 1;
            }
        }
    }
    
    else if (dir == VERT) {
        if (c > getCols() || c <= 0) { //same checks
            return false;
        }
        if (r > getRows() || r <= 0) {
            return false;
        }
        if (r + distance > getRows() || r + distance <= 0) {
            return false;
        }
        
        int x = 0;
        if (distance > 0) {
            while (x <= distance) {
                if (fgbg == FG) {
                    setChar(r+x, c, plotChar);
                }
                else if (fgbg == BG) {
                    if (getChar(r+x, c) == ' ' ) { //vert is the same but rows instead because going up and down changes rows
                        setChar(r+x, c, plotChar);
                    }
                }
                x += 1;
            }
        }
        else {
            while (x <= abs(distance)) {
                if (fgbg == FG) {
                    setChar(r-x, c, plotChar);
                }
                else if (fgbg == BG) {
                    if (getChar(r-x, c) == ' ' ) {
                        setChar(r-x, c, plotChar);
                    }
                }
                x += 1;
            }
        }
    }
    return true;
}

bool workingString(string commandString, int& badPos) {
    int i = 0;
    while ( i < commandString.size() ){
        if ((tolower(commandString[i]) != 'c') && (tolower(commandString[i]) != 'h') && (tolower(commandString[i]) != 'v') && (tolower(commandString[i]) != 'f') && (tolower(commandString[i]) != 'b')) { //first character has to be a command
            badPos = i;
            return false;
        }
        if (tolower(commandString[i]) == 'b' || tolower(commandString[i]) == 'f') {
            if (i >= commandString.size() -1) { //b or f shouldnt be the end of the string
                badPos = i + 1;
                return false;
            }
            if (!(isprint(commandString[i+1]))) { //check for valid character after b or f
                badPos = i + 1;
                return false;
            }
            i += 2;
            //
        }
        else if (tolower(commandString[i]) == 'h' || tolower(commandString[i]) == 'v') { //increments through h and v if somehting is wrong set as bad pos and return false
            i++;
            if (i <= commandString.size() -1 && isdigit(commandString[i]) ) { //have these if stateents becaues it could be single digit or double digit h or v
                i++;
                if (i <= commandString.size() -1 && isdigit(commandString[i]))
                    i++;
            }
            else if (i <= commandString.size() -1 && commandString[i] == '-') { // same thing as before but checking for negative single and double digit
                i++;
                if (i <= commandString.size() -1 && isdigit(commandString[i]) ) {
                    i++;
                    if (i <= commandString.size() -1 && isdigit(commandString[i])) {
                        i++;
                    }
                } else {
                    badPos = i;
                    return false;
                }
            }
            else {
                badPos = i;
                return false;
            }
        }
        else if (tolower(commandString[i]) == 'c') { //c is one character so skip over after
            i++;
        }
    }
    return true;
}
int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    if (!workingString(commandString, badPos)) { //by checking for errors at the start I can assume every string is valid after
        return 1;
    }
    if (!isprint(plotChar) || (mode != FG && mode != BG)) { //check the basics of the string: has to be printable and fg or bg.
        return 2;
    }
    int k = 0;
    int distance = 0;
    int currentCols = 1;
    int currentRows = 1;
    bool returnStatus = true; //this helps for out of bounds check
    while (k != commandString.size()) {
        if (commandString == "") {
            return 0;
        }
        if (tolower(commandString[k]) == 'c') {
            clearGrid();
            plotChar = '*'; //default
            mode = FG;
            
        }
        if (tolower(commandString[k]) == 'f') {
            plotChar = commandString[k+1]; //next index has to be the plotcharacter for f and b if valid
            mode = FG;
            k++;
        }
        if (tolower(commandString[k]) == 'b') {
            plotChar = commandString[k+1];
            mode = BG;
            k++;
        }
        if (tolower(commandString[k]) == 'h') {
            // negative double digit
            if (commandString[k+1] == '-' && isdigit(commandString[k+2]) && isdigit(commandString[k+3])) {
                distance =  -(10*(int(commandString[k+2] - '0')) + (int(commandString[k+3]-'0'))); //minus '0' to subtract the ascii value and times 10 after that for tens place
                returnStatus = plotLine(currentRows, currentCols, distance, HORIZ, plotChar, mode);
                if (returnStatus == true) {
                    currentCols += distance; //updates current distance if valid command
                }
                else {
                    badPos = k; //this is for out of bounds commands
                    return 3;
                }
            }
            // 2 digit
            else if (isdigit(commandString[k+1]) && isdigit(commandString[k+2])) {
                distance = 10*(int(commandString[k+1]- '0')) + (int(commandString[k+2]-'0'));
                returnStatus = plotLine(currentRows, currentCols, distance , HORIZ, plotChar, mode);
                if (returnStatus == true) {
                    currentCols += distance;
                }
                else {
                    badPos = k;
                    return 3;
                }
            }
            // single digit
            else if (isdigit(commandString[k+1])) {
                distance = int(commandString[k+1]- '0');
                returnStatus = plotLine(currentRows, currentCols, distance, HORIZ, plotChar, mode);
                if (returnStatus == true) {
                    currentCols += distance;
                }
                else {
                    badPos = k;
                    return 3;
                }
            }
            // negative single digit
            else if (commandString[k+1] == '-') {
                if (!isdigit(commandString[k+3])) {
                    distance =  -1*int(commandString[k+2]-'0');
                    returnStatus = plotLine(currentRows, currentCols, distance, HORIZ, plotChar, mode);
                    if (returnStatus == true) {
                        currentCols += distance;
                    }
                    else {
                        badPos = k;
                        return 3;
                    }
                }
            }
            
        }
        if (tolower(commandString[k]) == 'v') {
            // negative double digit
            if (commandString[k+1] == '-' && isdigit(commandString[k+2]) && isdigit(commandString[k+3])) { //perks of checking for invalid strings is that I can assume its valid
                distance = -(10*(int(commandString[k+2] - '0')) + (int(commandString[k+3]-'0')));
                returnStatus = plotLine(currentRows, currentCols, distance, VERT, plotChar, mode);
                if (returnStatus == true) {
                    currentRows += distance;
                }
                else {
                    badPos = k;
                    return 3;
                }
            }           
            // 2 digit
            else if (isdigit(commandString[k+1]) && isdigit(commandString[k+2])) {
                distance = 10*(int(commandString[k+1]- '0')) + (int(commandString[k+2]-'0'));
                returnStatus = plotLine(currentRows, currentCols, distance, VERT, plotChar, mode);
                if (returnStatus == true) {
                    currentRows += distance;
                }
                else {
                    badPos = k;
                    return 3;
                }
            }
            // single digit
            else if (isdigit(commandString[k+1])) {
                distance = int(commandString[k+1]- '0');
                returnStatus = plotLine(currentRows, currentCols, distance, VERT, plotChar, mode);
                if (returnStatus == true) {
                    currentRows += distance;
                }
                else {
                    badPos = k;
                    return 3;
                }
            }
            
            // negative single digit
            else if (commandString[k+1] == '-') {
                if (!isdigit(commandString[k+3])) {
                    distance =  -1*int(commandString[k+2]-'0');
                    returnStatus = plotLine(currentRows, currentCols, distance, VERT, plotChar, mode);
                    if (returnStatus == true) {
                        currentRows += distance;
                    }
                    else {
                        badPos = k;
                        return 3;
                    }
                }
                
            }
            
        }
        k++;
    }
    return 0;
}

