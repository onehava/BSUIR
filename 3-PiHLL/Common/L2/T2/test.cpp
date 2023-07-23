#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <vector>
#include <iostream>
#include "my_string.h"

TEST_CASE("Empty constructor")
{
    String string;

    REQUIRE(string.length() == 0);
    REQUIRE(string.capacity() == 15);
}

TEST_CASE("Legacy-string constructor")
{
    const char* legacy_string = "Lorem ipsum dolor sit amet";
    String string(legacy_string);

    REQUIRE(string.length() == strlen(legacy_string));
    REQUIRE(string.capacity() >= string.length());
    REQUIRE(strcmp(string.c_str(), legacy_string) == 0);
}

TEST_CASE("Copy constructor")
{
    String base = "Lorem ipsum dolor sit amet";
    String copy = base;

    REQUIRE(copy.length() == base.length());
    REQUIRE(copy.capacity() >= base.capacity());
    REQUIRE(strcmp(base.c_str(), copy.c_str()) == 0);
}

TEST_CASE("Equals and not-equals operator")
{
    String a = "Lorem ipsum";
    String b = "Dolor sit amet";

    REQUIRE((a == a));
    REQUIRE((b == b));
    REQUIRE((a != b));
    REQUIRE((b != a));
}

TEST_CASE("Plus operator")
{
    String a = "Lorem ipsum";
    String b = "Dolor sit amet";
    String c = a + b;

    REQUIRE(strcmp(c.c_str(), "Lorem ipsumDolor sit amet") == 0);
}

TEST_CASE("Plus-equals operator")
{
    String a = "Lorem ipsum";
    String b = "Dolor sit amet";
    a += b;

    REQUIRE(strcmp(a.c_str(), "Lorem ipsumDolor sit amet") == 0);
}

TEST_CASE("Indexing operator")
{
    String string = "Lorem ipsum";
    const char* legacy = "Lorem ipsum";

    for(int i = 0; i < string.length(); i++){
        REQUIRE(string[i] == legacy[i]);
    }
}

TEST_CASE("Substring operator")
{
    String string = "Lorem ipsum dolor sit amet";
    String substring = string(6, 11);

    REQUIRE(strcmp(substring.c_str(), "ipsum dolor") == 0);
}

TEST_CASE("Output operator")
{
    std::ostringstream ostream;
    std::vector<String> words = {"Lorem", "ipsum", "dolor", "sit", "amet"};
    const char* expect = "Lorem ipsum dolor sit amet";

    auto word = words.begin();
    ostream << *word;
    word++;

    while(word != words.end()){
        ostream << ' ' << *word;
        word++;
    }

    std::string result = ostream.str();
    REQUIRE(strcmp(result.c_str(), expect) == 0);
}

TEST_CASE("Input operator")
{
    std::istringstream istream("    Lorem    ipsum dolor sit amet");
    std::vector<String> words = {"Lorem", "ipsum", "dolor", "sit", "amet"};

    auto word = words.begin();

    while(!istream.eof()){
        String a;
        istream >> a;

        REQUIRE(word != words.end());
        REQUIRE((a == *word));
        word++;
    }
}