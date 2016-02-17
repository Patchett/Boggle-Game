#include "boggleutil.h"
#include <iostream>

using namespace std;

void TST::insert(string data, TSTNode *&current)
{
    if (!current) // Create a new node if current is null
        current = new TSTNode(data[0]);

    if (data[0] > current->data) // Go left
        insert(data, current->right);
    else if (data[0] < current->data) // Go right
        insert(data, current->left);
    // First letter of data is equal to current->data, truncate that first letter and go mid.
    else if (data.length() > 1)
        insert(data.substr(1), current->mid);
    else
        current->end = true;
}

char TST::find(string data, TSTNode *current)
{
    if (!current) // Base case
        return 0;
    // cout << "Find on current letter: " << current->data << endl;

    if (data.length() == 1 && data[0] == current->data)
    {
        if (!current->end) // The word was a substring but not a full word
            return 1;
        else // The word was a full word found in the lexicon
            return 2;
    }

    if (data[0] > current->data) // Go right
        return find(data, current->right);
    else if (data[0] < current->data) // Go left
        return find(data, current->left);
    else // Letters match, truncate the first letter of data and go mid.
        return find(data.substr(1), current->mid);
}

void TST::clear(TSTNode *current)
{
    if (!current) // Base case
        return;

    // Recursively traverse the trie
    clear(current->left);
    clear(current->mid);
    clear(current->right);

    delete current;
}
