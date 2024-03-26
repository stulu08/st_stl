#pragma once
#include <gtest/gtest.h>
#include <stulu/stl/array.h>

#include <array>

template<class T, size_t Size>
using array_type = stulu::array<T, Size>;

class ArrayTest : public ::testing::Test {
protected:
    array_type<int, 5> arr;
};

// Test case to check constructors
TEST_F(ArrayTest, Constructors) {
    array_type<int, 5> arr2{ 1, 2, 3, 4, 5 }; // Initializer list constructor
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i + 1, arr2[i]);
    }
}

// Test case to check at function
TEST_F(ArrayTest, At) {
    for (int i = 0; i < 5; ++i) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i + 1, arr.at(i));
    }

    // Check out of bounds access
    //EXPECT_THROW(arr.at(5), std::out_of_range);
}

// Test case to check front and back functions
TEST_F(ArrayTest, FrontAndBack) {
    arr = { 1, 2, 3, 4, 5 };

    EXPECT_EQ(1, arr.front());
    EXPECT_EQ(5, arr.back());
}

// Test case to check fill function
TEST_F(ArrayTest, Fill) {
    arr.fill(10);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(10, arr[i]);
    }
}

// Test case to check swap function
TEST_F(ArrayTest, Swap) {
    array_type<int, 5> other = { 6, 7, 8, 9, 10 };
    arr = { 1, 2, 3, 4, 5 };

    arr.swap(other);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(i + 6, arr[i]);
        EXPECT_EQ(i + 1, other[i]);
    }
}

// Test case to check size function
TEST_F(ArrayTest, Size) {
    EXPECT_EQ(5, arr.size());
}