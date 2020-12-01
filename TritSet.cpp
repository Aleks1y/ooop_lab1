#include "TritSet.h"


ostream& operator << (ostream& os, trit a)
{
    if (a == trit::False)
        return os << "False";

    else if (a == trit::True)
        return os << "True";

    else
        return os << "Unknown";
}

TritSet::TritSet(const unsigned int count)
{
    for (size_t i = 0; i < 8 * sizeof(unsigned int); i += 2)
        unknownMask |= 1 << i;

    tritsInUINT = (8 * sizeof(unsigned int)) / 2;
    vector<unsigned int> a(ceil((double)count/ tritsInUINT), unknownMask);
    arr = a;
    this->count = count;
}

unsigned int TritSet::capacity()
{
    return (unsigned int)ceil((double)count / tritsInUINT);
}

TritSet& TritSet::TritReference:: getSet()
{
    return set;
}

unsigned int TritSet::TritReference::getIndex()
{
    return index;
}

TritSet::TritReference& TritSet::TritReference::operator=(const trit t)
{
    if (t != trit::Unknown && index >= set.count)
    {
        set.arr.resize((unsigned int)(ceil((1.0 + index) / set.tritsInUINT)), set.unknownMask);
        set.count = index + 1;
    }

    unsigned int index_arr = index / set.tritsInUINT;
    unsigned int index_bit = (index % set.tritsInUINT) * 2;

    if (t == trit::False)
    {
        set.arr[index_arr] &= ~(3u << index_bit);
    }
    else if (t == trit::True)
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

bool TritSet::TritReference::operator ==(const trit trit)
{
    if (index >= set.count)
        return trit == trit::Unknown;

    unsigned int index_arr = index / set.tritsInUINT;
    unsigned int index_trit_in_uint = index % set.tritsInUINT;

    unsigned short value = 0;

    value |= (set.arr[index_arr] >> (index_trit_in_uint * 2)) & 3;

    switch (value)
    {
    case 0:
        return trit == trit::False;
    case 1:
        return trit == trit::Unknown;
    case 3:
        return trit == trit::True;
    }
}

bool TritSet::TritReference::operator !=(const trit trit)
{
    return !(this->operator ==(trit));
}

TritSet TritSet::operator|(TritSet& a)
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

TritSet TritSet::operator&(TritSet& a)
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

TritSet TritSet::operator!()
{
    TritSet tmp(this->count);
    for (size_t i = 0; i < tmp.capacity(); i++)
    {
        for (size_t j = 0; j < tmp.count * 2; j += 2)
        {
            if (((this->arr[i] >> j) & 3u) == 3) // True = 3
            {
                tmp.arr[i] &= ~(3u << j);
            }
            else if (((this->arr[i] >> j) & 3u) == 0) // False = 0
            {
                tmp.arr[i] |= 3u << j;
            }
        }
    }
    return tmp;
}

TritSet::TritReference TritSet::operator[](const unsigned int index)
{
    return TritReference(*this, index);
}

size_t TritSet::cardinality(trit value)
{
    bool enableCount = (value != trit::Unknown);

    size_t counter = 0;
    for (size_t i = this->count - 1;; i--)
    {
        if (enableCount && (*this)[i] == value)
            counter++;
        else if (!enableCount && (*this)[i] != trit::Unknown)
            enableCount = true;
        if (!i)
            break;
    }
    return counter;
}

unordered_map<trit, int> TritSet::cardinality()
{
    unordered_map<trit, int> tritInfo;
    tritInfo.insert(make_pair<trit, int>(trit::True, cardinality(trit::True)));
    tritInfo.insert(make_pair<trit, int>(trit::False, cardinality(trit::False)));
    tritInfo.insert(make_pair<trit, int>(trit::Unknown, cardinality(trit::Unknown)));

    return tritInfo;
}

void TritSet::trim(size_t lastIndex)
{
    if (lastIndex < count)
    {
        count = lastIndex;
        while (lastIndex % 16 != 0)
        {
            (*this)[lastIndex] = trit::Unknown;
            lastIndex++;
        }
        unsigned int index_arr = lastIndex / tritsInUINT;
        arr.resize(index_arr);
    }
}

size_t TritSet::length()
{
    size_t length = count;
    while (length > 0 && (*this)[length - 1] == trit::Unknown)
    {
        length--;
    }
    return length;
}

void TritSet::shrink()
{
    long int i = capacity() - 1;
    while (i >= 0 && arr[i] == unknownMask)
    {
        i--;
    }
    if (i != capacity() - 1)
    {
        arr.resize(i + 1);
        unsigned int indexLastTrit = (arr.size()) * tritsInUINT - 1;
        while (indexLastTrit > 0 && (*this)[indexLastTrit] == trit::Unknown)
        {
            indexLastTrit--;
        }
        count = indexLastTrit + 1;
    }
}

ostream& operator << (ostream& os, TritSet::TritReference t)
{
    if (t.getSet()[t.getIndex()] == trit::False)
    {
        return cout << "False";
    }
    else if (t.getSet()[t.getIndex()] == trit::True)
    {
        return cout << "True";
    }
    else
    {
        return cout << "Unknown";
    }
}