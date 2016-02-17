#include "boggleplayer.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, std::string **diceArray)
{
    ROWS = rows;
    COLS = cols;

    // Clear and resize boardVector
    boardVector.clear();
    boardVector.resize(ROWS, vector<string>(COLS));

    // Clear and resize wasVisited
    wasVisited.clear();
    wasVisited.resize(ROWS, vector<bool>(COLS));
    // cout << "==== The Board ====" << endl;

    // Traverse diceArray and convert all elements to lowercase. The store the
    // elements in boardVector
    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            // Convert diceArray[r][c] to all lowercase and insert
            // the result into the board
            boardVector[r][c] = diceArray[r][c];
            transform(diceArray[r][c].begin(),
                      diceArray[r][c].end(),
                      boardVector[r][c].begin(), ::tolower);
            // cout << boardVector[r][c];
        }
        // cout << "\n";
    }
    // cout << "===================" << endl;
    boardExists = true;
}

vector<int> BogglePlayer::isOnBoard(const string &word_to_check)
{
    // convert word_to_check to lowercase
    string lowerCaseWord = word_to_check;
    transform(word_to_check.begin(), word_to_check.end(),
              lowerCaseWord.begin(), ::tolower);

    // Location of the word in the board
    vector<int> wordLocation;
    // Check if setBoard has been called yet
    if (!boardExists)
        return wordLocation;

    // Traverse every element in the board
    for (int r = 0 ; r < ROWS; r++)
    {
        for (int c = 0 ; c < COLS; c++)
        {
            // Recursively search for the word at the current index
            if (searchAtIndex(lowerCaseWord, wordLocation, r, c))
                return wordLocation;
        }
    }
    return wordLocation;
}

bool BogglePlayer::searchAtIndex(const string &word_to_check,
                                 vector<int> &wordLocation, int r, int c)
{
    // Make sure that the board exists
    if (!boardExists)
        return false;

    // Make sure that the current position is in range
    if (r > (ROWS-1) || c > (COLS-1) || r < 0 || c < 0)
        return false;

    // The word to check is smaller than the contents of
    // this element in the board
    if (word_to_check.length() < boardVector[r][c].length())
        return false;
    // Check if this element has already been visited
    if (wasVisited[r][c])
        return false;

    // Check if current element in the board is a substring of word_to_check
    if (word_to_check.substr(0, boardVector[r][c].length()) != boardVector[r][c])
        return false;
    // All checks passed. This element of
    // boardVector contains the string we need
    wordLocation.push_back(COLS * r + c);
    if (word_to_check == boardVector[r][c])
        return true;
    wasVisited[r][c] = true;
    // Recursively iterate through the adjust[][] array defined in boggleplayer.h which allows
    // us to check all of the current element's neighbors for strings that match
    // what we are looking for
    for (short zod = 0; zod < 8; zod++)
    {
        // Jump to the next neighbor and call the recursive search function
        if (searchAtIndex(word_to_check.substr(boardVector[r][c].length()),
                          wordLocation, adjust[0][zod] + r, adjust[1][zod] + c))
        {
            // Once a word is found, set wasVisited to false as we move back up
            // the stack frame
            wasVisited[r][c] = false;
            return true;
        }
    }

    // The next part of the word_to_check was not found
    // in any of the surrounding elements
    wasVisited[r][c] = false;
    // Clear wordLocation vector
    wordLocation.pop_back();
    return false;
}


void BogglePlayer::buildLexicon(const set<string> &word_list)
{
    if (lexiconExists)
        delete Lexicon;

    Lexicon = new TST();
    // cout << "Building Lexicon..." << endl;

    // Store the set in a vector so that it can be shuffled
    vector<string> lexVector(word_list.begin(), word_list.end());
    // Shuffle the vector so that the structure of the TST is not a linked list
    random_shuffle(lexVector.begin(), lexVector.end());
    // Insert all elements into the TST
    for (auto &zod : lexVector)
    {
        // cout << "Inserting " << zod << " into Lexicon" << endl;
        Lexicon->insert(zod, Lexicon->root);
    }
    lexVector.clear();
    // cout << "Core dump is here " << endl;
    // cout << "Root of Lexicon is: " << Lexicon->root->data << endl;
    // cout << "Mid of Lexicon is: " << Lexicon->root->mid->data << endl;
    lexiconExists = true;
}

bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string> *words)
{
    // Store the minimum_word_length
    minWordLength = minimum_word_length;
    // Check if Lexicon and Board exist
    if (!lexiconExists || !boardExists)
        return false;

    // Iterate through the entire board
    // Call the recursive function at each element on the board that will get
    // all valid words that start with that element
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            // cout << "Get all valid words at " << boardVector[r][c] << endl;
            getAllValidWordsAtIndex("", words, r, c);
        }
    }
    return true;
}

bool BogglePlayer::getAllValidWordsAtIndex(string currentWord, set<string> *words, int r, int c)
{
    // Make sure that the current position is in range
    if (r > (ROWS-1) || c > (COLS-1) || r < 0 || c < 0)
        return false;

    // Check if this element has already been visited
    if (wasVisited[r][c])
        return false;

    // Concatenate the contents of the current element of the board with currentWord
    currentWord += boardVector[r][c];

    // Check if the new concatenated word is a prefix of a word in the Lexicon
    char prefix = Lexicon->find(currentWord, Lexicon->root);
    // New concatenated string is not a prefix.
    if (!prefix)
        return false;
    wasVisited[r][c] = true;
    // currentWord is a valid word in the Lexicon. Insert it into the set
    if (currentWord.size() >= minWordLength && prefix == 2)
        words->insert(currentWord);

    // Check all of the current element's neighbors for strings that are in the
    // lexicon by iterating through the adjust[][] vector to visit the neighbors
    for (short zod = 0; zod < 8; zod++)
        getAllValidWordsAtIndex(currentWord, words, adjust[0][zod] + r, adjust[1][zod] + c);

    wasVisited[r][c] = false;
    return true;
}



bool BogglePlayer::isInLexicon(const string &word_to_check)
{
    // Check if Lexicon exists
    if (!lexiconExists)
        return false;
    // Find the word in the Lexicon
    char result = Lexicon->find(word_to_check, Lexicon->root);
    // Word was found if result is 2
    if (result == 2)
        return true;
    // Word was not found if result was 0 or 1
    else return false;
}

// This is producing a segfault when invoked. Not mandatory for assignment.
void BogglePlayer::getCustomBoard(string ** &new_board, unsigned int *rows, unsigned int *cols)
{
    COLS = *cols;
    ROWS = *rows;
    std::ifstream customBoard("brd.txt");

    // Clear and resize boardVector
    boardVector.clear();
    boardVector.resize(ROWS, vector<string>(COLS));

    // Clear and resize wasVisited
    wasVisited.clear();
    wasVisited.resize(ROWS, vector<bool>(COLS));
    // cout << "==== The Board ====" << endl;
    //
    // Allocate space for the new board
    new_board = new std::string*[ROWS];
    for (unsigned int r=0; r < *rows; ++r)
    {
        string *row = new std::string[COLS];
        for (unsigned int c = 0; c < *cols; ++c)
            customBoard >> row[c];
        new_board[r] = row;
    }

    // store new board
    for (unsigned int r = 0; r < *rows; r++)
    {
        for (unsigned int c = 0; c < *cols; c++)
        {
            // Convert diceArray[r][c] to all lowercase and insert
            // the result into the board
            boardVector[r][c] = new_board[r][c];
            transform(new_board[r][c].begin(),
                      new_board[r][c].end(),
                      boardVector[r][c].begin(), ::tolower);
            // cout << boardVector[r][c];
        }
        // cout << "\n";
    }
    // cout << "===================" << endl;
    boardExists = true;

}

