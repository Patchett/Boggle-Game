#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <string>

using std::string;

class TSTNode
{
public:
    TSTNode *left;
    TSTNode *right;
    TSTNode *mid;
    bool end; // Signifies whether this node holds the last char in a word in the Lexicon
    const char data;

    TSTNode(const char &val) : left(NULL), right(NULL), mid(NULL), end(false), data(val) {}
};

class TST
{
public:
    TSTNode *root;

    // Constructor
    TST() : root(NULL) {}

    /**
    * Tells us whether the passed in string exists in the Lexicon or is a prefix of
    * a word in the Lexicon
    * @method TST::find
    * @param  data        Check if data is a prefix of any word in the TST
    * @param  current     Pointer to the current TSTNode in the recursion
    * @return             2: The word was a prefix and a valid word in the Lexicon
    *                     1: The word was a prefix but not a valid word in the Lexicon
    *                     0: The word was not a prefix or a valid word in the Lexicon
    */
    char find(string data, TSTNode *current);

    /**
    * Recursively inserts a new string into the TST
    * @method TST::insert
    * @param  data        The string to be inserted
    * @param  current     The current node in the traversal
    */
    void insert(string data, TSTNode *&current);

    // Destructor delegates to clear() method
    ~TST()
    {
        clear(root);
        root = NULL;
    }

private:

    /**
    * Helper method for the destructor. Recursively deletes all nodes in the TST.
    * @method TST::clear
    * @param  current    current node in the traversal
    */
    void clear(TSTNode *current);
};

#endif // BOGGLEUTIL_H
