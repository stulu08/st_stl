#pragma once
#include <gtest/gtest.h>
#include <stulu/stl/vector.h>

template<class T>
using vector_type = Stulu::vector<T>;

class VectorTest : public ::testing::Test {
protected:
    vector_type<int> vec;
};

// Test case to check constructors
TEST_F(VectorTest, Constructors) {
    vector_type<int> vec1; // Default constructor
    EXPECT_TRUE(vec1.empty());
    EXPECT_EQ(0, vec1.size());

    vector_type<int> vec2(5); // Constructor with initial size
    EXPECT_EQ(5, vec2.size());
    EXPECT_EQ(0, vec2[0]); // Elements should be default-initialized
}

// Test case to check reserve function
TEST_F(VectorTest, Reserve) {
    vec.reserve(100);
    EXPECT_GE(vec.capacity(), 100); // Capacity should be at least 100 after reserving
}

// Test case to check resize function
TEST_F(VectorTest, Resize) {
    vec.resize(5);
    EXPECT_EQ(5, vec.size());
    EXPECT_EQ(0, vec[0]); // Elements should be default-initialized

    vec.resize(10, 100); // Resize with fill value
    EXPECT_EQ(10, vec.size());
    for (int i = 5; i < 10; ++i) {
        EXPECT_EQ(100, vec[i]); // Check new elements are filled with 100
    }

    vec.resize(3); // Shrink vector
    EXPECT_EQ(3, vec.size());
}

// Test case to check clear function
TEST_F(VectorTest, Clear) {
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    vec.clear();
    EXPECT_TRUE(vec.empty());
}

// Test case to check shrink_to_fit function
TEST_F(VectorTest, ShrinkToFit) {
    vec.reserve(100);
    size_t capacity = vec.capacity();

    vec.shrink_to_fit();
    EXPECT_EQ(vec.size(), vec.capacity()); // Size should be equal to capacity after shrinking
}

// Test case to check push_back function
TEST_F(VectorTest, PushBack) {
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    EXPECT_EQ(10, vec[0]);
    EXPECT_EQ(20, vec[1]);
    EXPECT_EQ(30, vec[2]);
}

// Test case to check pop_back function
TEST_F(VectorTest, PopBack) {
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    vec.pop_back();
    EXPECT_EQ(2, vec.size());
    EXPECT_EQ(20, vec.back());
}

// Test case to check empty function
TEST_F(VectorTest, Empty) {
    EXPECT_TRUE(vec.empty());

    vec.push_back(10);
    EXPECT_FALSE(vec.empty());
}

// Test case to check capacity function
TEST_F(VectorTest, Capacity) {
    EXPECT_GE(vec.capacity(), vec.size());

    vec.reserve(100);
    EXPECT_GE(vec.capacity(), 100);
}

// Test case to check size function
TEST_F(VectorTest, Size) {
    EXPECT_EQ(0, vec.size());

    vec.push_back(10);
    vec.push_back(20);
    EXPECT_EQ(2, vec.size());
}