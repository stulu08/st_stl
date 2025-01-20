#pragma once
#include <gtest/gtest.h>
#include <stulu/stl/string.h>

using string_type = Stulu::string;

class StringTest : public ::testing::Test {
protected:
    string_type str;
};

// Test case to check default constructor and empty function
TEST_F(StringTest, DefaultConstructorAndEmpty) {
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(0, str.size());
}

// Test case to check constructor from const char*
TEST_F(StringTest, ConstructorFromConstChar) {
    const char* text = "Hello";
    string_type str(text);

    EXPECT_EQ(5, str.size());
    EXPECT_EQ('H', str.front());
    EXPECT_EQ('o', str.back());
}

// Test case to check copy constructor
TEST_F(StringTest, CopyConstructor) {
    string_type src = "Hello";
    string_type copied(src);

    EXPECT_EQ(src, copied);
    EXPECT_NE(src.data(), copied.data()); // Ensure deep copy
}

// Test case to check copy assignment operator
TEST_F(StringTest, CopyAssignmentOperator) {
    string_type src = "Hello";
    string_type copied;
    copied = src;

    EXPECT_EQ(src, copied);
    EXPECT_NE(src.data(), copied.data()); // Ensure deep copy
}

// Test case to check move constructor
TEST_F(StringTest, MoveConstructor) {
    string_type src = "Hello";
    const char* srcData = src.data();
    string_type moved(std::move(src));

    EXPECT_EQ("", src); // Source should be empty after move
    EXPECT_EQ("Hello", moved);
    EXPECT_EQ(srcData, moved.data()); // Ensure shallow move
}

// Test case to check move assignment operator
TEST_F(StringTest, MoveAssignmentOperator) {
    string_type src = "Hello";
    const char* srcData = src.data();
    string_type moved;
    moved = std::move(src);

    EXPECT_EQ("", src); // Source should be empty after move
    EXPECT_EQ("Hello", moved);
    EXPECT_EQ(srcData, moved.data()); // Ensure shallow move
}

// Test case to check back and front functions
TEST_F(StringTest, BackAndFront) {
    str = "Hello";

    EXPECT_EQ('H', str.front());
    EXPECT_EQ('o', str.back());
}

// Test case to check empty and size functions
TEST_F(StringTest, EmptyAndSize) {
    str = "Hello";

    EXPECT_FALSE(str.empty());
    EXPECT_EQ(5, str.size());
}

// Test case to check length function
TEST_F(StringTest, Length) {
    str = "Hello";

    EXPECT_EQ(5, str.length());
}

// Test case to check data function
TEST_F(StringTest, Data) {
    str = "Hello";

    const char* data = str.data();
    EXPECT_EQ('H', data[0]);
    EXPECT_EQ('o', data[4]);
}

// Test case to check c_str function
TEST_F(StringTest, CStr) {
    str = "Hello";

    const char* cstr = str.c_str();
    EXPECT_EQ('H', cstr[0]);
    EXPECT_EQ('o', cstr[4]);
}

// Test case to check at function
TEST_F(StringTest, At) {
    str = "Hello";

    EXPECT_EQ('H', str.at(0));
    EXPECT_EQ('o', str.at(4));
}

// Test case to check assign function with const char*
TEST_F(StringTest, AssignConstChar) {
    str.assign("Hello");

    EXPECT_EQ('H', str.at(0));
    EXPECT_EQ('o', str.at(4));
}

// Test case to check append function on an empty string with const char*
TEST_F(StringTest, EmptyAppendConstChar) {
    str.append("Hello");
    str.append(" ");
    str.append("World");

    EXPECT_EQ(str, "Hello World");
    EXPECT_EQ(str.length(), 11);
}

// Test case to check append function with substring of a const char*
TEST_F(StringTest, SublenAppend) {
    str = "Hello";
    str.append("Hello World", 5, 6) ;

    EXPECT_EQ(str, "Hello World");
    EXPECT_EQ(str.length(), 11);
}

// Test case to check append function
TEST_F(StringTest, AppendConstChar) {
    str = "Hello";
    str.append(' ');
    str.append(Stulu::string("World"));
    str.append((" from C++"));

    EXPECT_EQ(str, "Hello World from C++");
    EXPECT_EQ(str.length(), 20);
}

// Test case to check num char append function
TEST_F(StringTest, AppendChars) {
    str = "Hello";
    str.append(5, ' ');

    EXPECT_EQ(str, "Hello     ");
    EXPECT_EQ(str.length(), 10);
}