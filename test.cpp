#include "pch.h"
#include "TritSet.h"

TEST(TritSet, AllocationTest)
{
    for (int i = 0; i <= 1000; ++i)
    {
        TritSet set(i);
        size_t allocLength = set.capacity();
        EXPECT_GE(allocLength, i * 2 / 8 / sizeof(unsigned int));
    }
}

TEST(TritSet, TritMemoryUnknown)
{
    TritSet set(6);
    size_t allocLength = set.capacity();
    set[5] = trit::True;

    for (size_t i = 6; i < 17; i++)
    {
        EXPECT_TRUE(set[i] == trit::Unknown);
    }
}

TEST(TritSet, TritValue)
{
    TritSet set(6);
    set[0] = trit::True;
    set[1] = trit::False;
    set[2] = trit::True;
    set[3] = trit::True;
    set[5] = trit::False;

    EXPECT_TRUE(set[0] == trit::True);
    EXPECT_TRUE(set[1] == trit::False);
    EXPECT_TRUE(set[2] == trit::True);
    EXPECT_TRUE(set[3] == trit::True);
    EXPECT_TRUE(set[4] == trit::Unknown);
    EXPECT_TRUE(set[5] == trit::False);
}

TEST(TritSet, UnknownOutOfBoundary)
{
    TritSet set(1000);
    size_t allocLength = set.capacity();

    set[1000000000] = trit::Unknown;
    EXPECT_EQ(allocLength, set.capacity());
}

TEST(TritSet, ComparingOutOfBoundary)
{
    TritSet set(1000);
    size_t allocLength = set.capacity();

    EXPECT_FALSE(set[2000000000] == trit::True);
    EXPECT_EQ(allocLength, set.capacity());
}

TEST(TritSet, TrueOutOfBoundary)
{
    TritSet set(1000);
    size_t allocLength = set.capacity();

    set[10000] = trit::True;
    EXPECT_LT(allocLength, set.capacity());
    EXPECT_TRUE(set[10000] == trit::True);
    for (size_t i = 10001; i < 10020; i++)
    {
        EXPECT_TRUE(set[i] == trit::Unknown);
    }
}


TEST(TritSet, Shrink)
{
    TritSet set(10);
    for (int i = 10; i <= 1000; ++i)
    {
        set[i] = trit::True;
        set[i * 2] = trit::True;
        size_t allocLength = set.capacity();
        set[i * 2] = trit::Unknown;
        set.shrink();
        EXPECT_TRUE(allocLength > set.capacity());
    }
}

TEST(TritSet, OrAllocation)
{
    TritSet setA(1000);
    TritSet setB(2000);

    TritSet setC = setA | setB;

    EXPECT_EQ(setB.capacity(), setC.capacity());
}

TEST(TritSet, AndAllocation)
{
    TritSet setA(1000);
    TritSet setB(2000);

    TritSet setC = setA & setB;

    EXPECT_EQ(setB.capacity(), setC.capacity());
}

TEST(TritSet, OrTest)
{
    TritSet setA(9);
    TritSet setB(9);

    setA[3] = trit::True;
    setA[4] = trit::True;
    setA[5] = trit::True;
    setA[6] = trit::False;
    setA[7] = trit::False;
    setA[8] = trit::False;

    setB[0] = trit::True;
    setB[1] = trit::False;
    setB[3] = trit::True;
    setB[4] = trit::False;
    setB[6] = trit::True;
    setB[7] = trit::False;

    TritSet setC = setA | setB;

    EXPECT_TRUE(setC[0] == trit::True);
    EXPECT_TRUE(setC[1] == trit::Unknown);
    EXPECT_TRUE(setC[2] == trit::Unknown);
    EXPECT_TRUE(setC[3] == trit::True);
    EXPECT_TRUE(setC[4] == trit::True);
    EXPECT_TRUE(setC[5] == trit::True);
    EXPECT_TRUE(setC[6] == trit::True);
    EXPECT_TRUE(setC[7] == trit::False);
    EXPECT_TRUE(setC[8] == trit::Unknown);
}

TEST(TritSet, AndTest)
{
    TritSet setA(9);
    TritSet setB(9);

    setA[3] = trit::True;
    setA[4] = trit::True;
    setA[5] = trit::True;
    setA[6] = trit::False;
    setA[7] = trit::False;
    setA[8] = trit::False;

    setB[0] = trit::True;
    setB[1] = trit::False;
    setB[3] = trit::True;
    setB[4] = trit::False;
    setB[6] = trit::True;
    setB[7] = trit::False;

    TritSet setC = setA & setB;

    EXPECT_TRUE(setC[0] == trit::Unknown);
    EXPECT_TRUE(setC[1] == trit::False);
    EXPECT_TRUE(setC[2] == trit::Unknown);
    EXPECT_TRUE(setC[3] == trit::True);
    EXPECT_TRUE(setC[4] == trit::False);
    EXPECT_TRUE(setC[5] == trit::Unknown);
    EXPECT_TRUE(setC[6] == trit::False);
    EXPECT_TRUE(setC[7] == trit::False);
    EXPECT_TRUE(setC[8] == trit::False);
}

TEST(TritSet, NotTest)
{
    TritSet setA(3);

    setA[0] = trit::True;
    setA[1] = trit::False;
    setA[2] = trit::Unknown;

    TritSet setC = !setA;

    EXPECT_TRUE(setC[0] == trit::False);
    EXPECT_TRUE(setC[1] == trit::True);
    EXPECT_TRUE(setC[2] == trit::Unknown);
}

TEST(TritSet, CardinalityFunctionTest)
{
    TritSet set(15);
    set[3] = trit::True;
    set[4] = trit::True;
    set[5] = trit::True;
    set[6] = trit::False;
    set[7] = trit::False;
    set[8] = trit::False;

    EXPECT_EQ(set.cardinality(trit::False), 3);
    EXPECT_EQ(set.cardinality(trit::True), 3);
    EXPECT_EQ(set.cardinality(trit::Unknown), 3);
}

TEST(TritSet, CardinalityMapTest)
{
    TritSet set(15);
    set[3] = trit::True;
    set[4] = trit::True;
    set[5] = trit::True;
    set[6] = trit::False;
    set[7] = trit::False;
    set[8] = trit::False;

    auto a = set.cardinality();

    EXPECT_EQ(a[trit::True], 3);
    EXPECT_EQ(a[trit::False], 3);
    EXPECT_EQ(a[trit::Unknown], 3);
}

TEST(TritSet, TrimTest)
{
    TritSet set(18);
    set[3] = trit::True;
    set[4] = trit::True;
    set[5] = trit::True;
    set[6] = trit::False;
    set[7] = trit::False;
    set[8] = trit::False;
    set[9] = trit::True;
    set[10] = trit::True;
    set[11] = trit::True;
    set[12] = trit::False;
    set[13] = trit::False;
    set[14] = trit::False;
    set[15] = trit::True;
    set[16] = trit::True;
    set[17] = trit::True;

    for (size_t j = 16; j > 0; j--)
    {
        set.trim(j);
        for (size_t i = j; i < 18; i++)
        {
            EXPECT_TRUE(set[i] == trit::Unknown);
        }
    }
}

TEST(TritSet, LengthTest)
{
    TritSet set(18);
    set[3] = trit::True;
    set[4] = trit::True;
    set[5] = trit::True;
    set[6] = trit::False;
    set[7] = trit::False;
    set[8] = trit::False;
    set[9] = trit::True;
    set[10] = trit::True;
    set[11] = trit::True;
    set[12] = trit::False;
    set[13] = trit::False;
    set[14] = trit::False;
    set[15] = trit::True;
    set[16] = trit::True;
    set[17] = trit::True;

    for (size_t j = 17; j > 3; j--)
    {
        set[j] = trit::Unknown;
        EXPECT_EQ(set.length(), j);
    }
}

TEST(TritSet, Test)
{
    TritSet set(1000);
    // length of internal array
    size_t allocLength = set.capacity();
    EXPECT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(unsigned int));

    set[100000000] = trit::Unknown;
    assert(allocLength == set.capacity());

    // false, but no exception or memory allocation
    if (set[2000000000] == trit::True) {}
    EXPECT_TRUE(allocLength == set.capacity());

    //выделение памяти
    set[100000000] = trit::True;
    EXPECT_TRUE(allocLength < set.capacity());

    //no memory operations
    allocLength = set.capacity();
    set[100000000] = trit::Unknown;
    set[1000000] = trit::False;
    EXPECT_TRUE(allocLength == set.capacity());

    //освобождение памяти до начального значения или 
    //до значения необходимого для хранения последнего установленного трита
    //в данном случае для трита 1000000
    set.shrink();
    EXPECT_TRUE(allocLength > set.capacity());

    TritSet setA(1000);
    TritSet setB(2000);
    TritSet setC = setA & setB;
    EXPECT_TRUE(setC.capacity() == setB.capacity());
}

TEST(TritSet, auto)
{
    TritSet set(6);
    size_t allocLength = set.capacity();

    for (auto trit : set)
    {
        trit = trit::True;
        cout << trit;
    }

    for (size_t i = 0; i < allocLength; i++)
    {
        EXPECT_TRUE(set[i] == trit::True);
    }
}
