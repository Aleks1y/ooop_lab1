#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

using namespace std;

enum class trit
{
    False = 0, //-1
    Unknown = 1, //0
    True = 3 //1

};

ostream& operator << (ostream& os, trit a);

class TritSet
{
private:

    unsigned int count;
    vector<unsigned int> arr;
    unsigned int unknownMask = 0,
        tritsInUINT;

public:

    TritSet(const unsigned int count);
    unsigned int capacity();
    
    class TritReference
    {
    private:

        TritSet& set;
        unsigned int index;

    public:

        TritReference(TritSet& set, const unsigned int index) : set(set), index(index) {};
        TritSet& getSet();
        unsigned int getIndex();
        TritReference& operator=(const trit t);
        bool operator ==(const trit t);
        bool operator !=(const trit t);
    };

    TritSet operator|(TritSet& a);
    TritSet operator&(TritSet& a);
    TritSet operator!();
    TritReference operator[](const unsigned int index);

    size_t cardinality(trit value);
    unordered_map<trit, int> cardinality();
    void trim(size_t lastIndex);
    size_t length();
    void shrink();
};

ostream& operator << (ostream& os, TritSet::TritReference t);