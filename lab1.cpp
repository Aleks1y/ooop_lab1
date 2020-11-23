#include <iostream>
#include <vector>
#include <utility>
#include <string>
using namespace std;

unsigned int unknownMask = 0;

enum trit
{
    False = 0, //-1
    Unknown = 1, //0
    True = 3 //1

};

ostream& operator << (ostream& os, trit a)
{
    if (a == False)
        return os << "False";

    else if (a == True)
        return os << "True";

    else
        return os << "Unknown";
}

class TritSet
{
private:
    unsigned long long count;
    vector<unsigned int> arr;

public:

    TritSet(const unsigned long long count)
    {
        if (unknownMask == 0)
        {
            for (size_t i = 0; i < 8 * sizeof(unsigned int); i += 2)
                unknownMask |= 1 << i;
        }
        vector<unsigned int> a(ceil((2.0 * count) / (8.0 * sizeof(unsigned int))), unknownMask);
        arr = a;
        this->count = count;
    }

    void shrink()
    {
        unsigned int i = capacity() - 1;
        while(i >= 0 && (arr[i] ^ unknownMask) != 0)
        {
            i--;
        }
        arr.resize(i);
    }

    unsigned int capacity()
    {
        return (unsigned int)ceil((2.0 * count) / (8.0 * sizeof(unsigned int)));
    }

    class TritReference // nested friend class required for direct accessing trit
    {
    private:
        TritSet& set;
        const unsigned long long index;
    public:
        TritReference(TritSet& set, const unsigned long long index) : set(set), index(index) {};

        TritSet& getSet()
        {
            return set;
        }

        unsigned long long getIndex()
        {
            return index;
        }

        TritReference& operator=(const trit t)
        {
            if (t != Unknown && index >= set.count)
            {
                set.arr.resize(ceil((2.0 * (1.0 + index)) / (8.0 * sizeof(unsigned int))), unknownMask);
                set.count = index + 1;
            }
            
            unsigned long long index_arr = index / ((8 * sizeof(unsigned int)) / 2);
            unsigned int index_bit = (index % (8 * sizeof(unsigned int) / 2)) * 2;
            
            if (t == False)
            {
                set.arr[index_arr] &= ~(3u << index_bit);
            }
            else if (t == True)
            {
                set.arr[index_arr] |= (3u << index_bit);
            }
            else if (index < set.count)
            {
                set.arr[index_arr] |= (1u << index_bit);
                set.arr[index_arr] &= ~(1u << (index_bit + 1));
            }
            return *this;
        }

        bool operator ==(const trit t)
        {
            if (this->set.getTrit(this->getIndex()) == t)
                return 1;
            return 0;
        }
    };

    TritSet operator|(TritSet& a)
    {
        TritSet& big = a.capacity() > this->capacity() ? a : *this,
            small = a.capacity() > this->capacity() ? *this : a;
        TritSet tmp(big.count);
        size_t i;
        for (i = 0; i < small.capacity(); i++)
            tmp.arr[i] = small.arr[i] | big.arr[i];

        while (i < big.capacity())
        {
            tmp.arr[i] |= big.arr[i];
            i++;
        }
        return tmp;
    }

    TritSet operator&(TritSet& a)
    {
        TritSet& big = a.capacity() > this->capacity() ? a : *this,
            small = a.capacity() > this->capacity() ? *this : a;
        TritSet tmp(big.count);
        size_t i;
        for (i = 0; i < small.capacity(); i++)
            tmp.arr[i] = small.arr[i] & big.arr[i];

        while (i < big.capacity())
        {
            tmp.arr[i] &= big.arr[i];
            i++;
        }
        return tmp;
    }

    TritSet operator!()
    {
        TritSet tmp(this->count);
        for (size_t i = 0; i < tmp.capacity(); i++)
        {
            for (size_t j = 0; j < tmp.count * 2; j += 2)
            {
                if (((tmp.arr[i] >> j) & 3u) == True)
                {
                    tmp.arr[i] &= ~(3u << j);
                }
                else if (((tmp.arr[i] >> j) & 3u) == False)
                {
                    tmp.arr[i] |= 3u << j;
                }
            }
        }
        return tmp;
    }

    TritReference operator[](const unsigned long long index)
    {
        return TritReference(*this, index);
    }

    trit getTrit(unsigned long long index) const
    {
        if (index >= count)
            return Unknown;

        unsigned long long index_arr = index / ((8 * sizeof(unsigned int)) / 2);
        unsigned int index_trit = (index % (8 * sizeof(unsigned int) /2));

        unsigned short value = 0;

        value |= (arr[index_arr] >> (index_trit * 2)) & 3;

        switch (value)
        {
        case 0:
            return False;
        case 1:
            return Unknown;
        case 3:
            return True;
        }
    }
};

ostream& operator << (ostream& os, TritSet::TritReference t)
{
    if ((unsigned long long)t.getSet().capacity() * 4 < t.getIndex())
        return cout << Unknown;

    return cout << t.getSet().getTrit(t.getIndex());
}

int main(void)
{
    

    return 0;
}
