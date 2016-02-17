#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include "boggleutil.h"
#include "baseboggleplayer.h"
#include <vector>
#include <iostream>

using namespace std;

class BogglePlayer : public BaseBogglePlayer
{
public:
    BogglePlayer() : Lexicon(nullptr), ROWS(0), COLS(0), minWordLength(0), boardExists(false), lexiconExists(false) {}

    /**
     * Inserts all words from word_list into the TST
     * @method buildLexicon
     * @param  word_list    The words to be inserted into the lexicon
     */
    void buildLexicon(const set<string> &word_list);

    /**
    * Sets up the board by storing the passed in 2D array inside of a 2D vector
    * @method BogglePlayer::setBoard
    * @param  rows                   number of rows
    * @param  cols                   number of columns
    * @param  diceArray              the array that holds the characters which will
    *                                be on the board
    */
    void setBoard(unsigned int rows, unsigned int cols, string **diceArray);

    /**
     * Find all words that are on the board that are connected by an acyclic path
     * and found in the Lexicon
     * @method getAllValidWords
     * @param  minimum_word_length minimum length of words to look for
     * @param  words               A set of all words that were found
     * @return                     The set of all words that meet the conditions outlined
     *                             in the description
     */
    bool getAllValidWords(unsigned int minimum_word_length, set<string> *words);

    /**
     * Check if a word is in the lexicon
     * @method isInLexicon
     * @param  word_to_check Run the check on this word
     * @return               True if in the Lexicon, false if not.
     */
    bool isInLexicon(const string &word_to_check);

    /**
     * Attempts to find the passed in word on the board. If found, returns
     * a vector holding the positions of each character in the word. Positions
     * are in format row*COLS + c. If word was not found, returns an empty vector
     */
    vector<int> isOnBoard(const string &word_to_check);

    // Optional. I tried to implement it but didn't want to spend the time to
    // get it working
    void getCustomBoard(string ** &new_board, unsigned int *rows, unsigned int *cols);

    /**
     * Destructor deletes the Lexicon
     */
    ~BogglePlayer()
    {
        delete Lexicon;
    }

    // Holds the Lexicon
    TST *Lexicon;

private:
    // Holds number of rows and columns in the board
    int ROWS;
    int COLS;
    unsigned int minWordLength;
    // Holds the board
    vector<vector<string>> boardVector;
    vector<vector<bool>> wasVisited;
    // Flags for whether the lexicon/board have been created
    bool boardExists;
    bool lexiconExists;
    // Array for searching adjacent elements in the board
    int adjust[2][8] =
    {
        {1, 1, 1, 0, 0, -1, -1, -1 },
        {-1, 0, 1, -1, 1, -1, 0, 1 }
    };

    /**
     * Helper for isOnBoard. Recursive function which searches for the passed
     * in word at the given index in on the board by checking neighbors of
     * the current index for subsequent letters.
     * @method searchAtIndex
     * @param  word_to_check The word we are looking for
     * @param  wordLocation  Vector of locations of letters that were found
     * @param  r             row on the board that we are looking at
     * @param  c             column on the board that we are looking at
     * @return               True if word was found, false if not.
     */
    bool searchAtIndex(const string &word_to_check, vector<int> &wordLocation,
                       int r, int c);

    /**
     * Helper for getAllValidWords. Recursive function which will find all valid
     * words on the board starting at the passed in index which are also in the lexicon.
     * @method getAllValidWordsAtIndex
     * @param  currentWord             The word that we have created so far based
     *                                 on our path through the board
     * @param  words                   The set of all words that are found in the Lexicon
     *                                 and on the board
     * @param  r                       row on the board that we are looking at
     * @param  c                       column on the board that we are looking at
     * @return                         Return true if the word that we have made
     *                                 with the path we are traveling is found in
     *                                 the Lexicon. False if not.
     */
    bool getAllValidWordsAtIndex(string currentWord, set<string> *words, int r, int c);
};

#endif // BOGGLEPLAYER_H


