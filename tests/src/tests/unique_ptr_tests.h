#pragma once
#include <gtest/gtest.h>
#include <stulu/stl/unique_ptr.h>

template<class T>
using unique_ptr_type = Stulu::unique_ptr<T>;

class UniquePtrTest : public ::testing::Test {
protected:
    using ValueType = int;
    unique_ptr_type<ValueType> ptr;
};

// Test case to check if unique_ptr is initially nullptr
TEST_F(UniquePtrTest, InitiallyNullptr) {
    EXPECT_EQ(nullptr, ptr.get());
}

// Test case to check if unique_ptr properly manages memory
TEST_F(UniquePtrTest, MemoryManagement) {
    ptr.reset(new ValueType(42));
    EXPECT_NE(nullptr, ptr.get()); // Memory should be allocated
    EXPECT_EQ(42, *ptr); // Value should be set correctly

    ValueType* rawPtr = ptr.release();
    EXPECT_EQ(nullptr, ptr.get()); // Ownership transferred, so ptr should be nullptr
    EXPECT_EQ(42, *rawPtr); // Value should still be accessible via raw pointer

    delete rawPtr; // Clean up raw pointer memory
}

// Test case to check if unique_ptr can be reset with new memory
TEST_F(UniquePtrTest, Reset) {
    ptr.reset(new ValueType(42));
    EXPECT_EQ(42, *ptr); // Value should be set correctly

    ptr.reset(new ValueType(99));
    EXPECT_EQ(99, *ptr); // Value should be updated
}

// Test case to check if unique_ptr can be moved
TEST_F(UniquePtrTest, Move) {
    unique_ptr_type<ValueType> ptr2(new ValueType(42));
    unique_ptr_type<ValueType> ptr3(std::move(ptr2));

    EXPECT_EQ(nullptr, ptr2.get()); // Ownership transferred, so ptr2 should be nullptr
    EXPECT_NE(nullptr, ptr3.get()); // ptr3 should now own the memory
    EXPECT_EQ(42, *ptr3); // Value should be preserved
}