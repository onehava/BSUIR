#include <catch2/catch_test_macros.hpp>
#include "list.h"

TEST_CASE("Empty constructor")
{
    List<int> list;

    REQUIRE(list.empty());
    REQUIRE_THROWS(list.at(0));
}

TEST_CASE("Element constructor")
{
    std::size_t count = 20;
    int element = 10;
    List<int> list(count, element);

    REQUIRE(list.size() == count);

    for(auto i = 0; i < count; i++){
        int v;
        REQUIRE_NOTHROW((v = list.at(i)));
        REQUIRE(v == element);
    }
}

TEST_CASE("Front interaction")
{
    std::size_t count = 20;
    List<int> list;

    for(auto i = 1; i <= count; i++){
        list.push_front(i);
        REQUIRE(list.front() == i);
    }

    REQUIRE(list.size() == count);

    for(auto i = count; i >= 1; i--){
        int v = list.front();
        REQUIRE(v == i);
        list.pop_front();
    }
}

TEST_CASE("Back interaction")
{
    std::size_t count = 20;
    List<int> list;

    for(auto i = 1; i <= count; i++){
        list.push_back(i);
        REQUIRE(list.back() == i);
    }

    REQUIRE(list.size() == count);

    for(auto i = count; i >= 1; i--){
        int v = list.back();
        REQUIRE(v == i);
        list.pop_back();
    }
}

TEST_CASE("Insert interaction")
{
    SECTION("Front-like interaction")
    {
        std::size_t count = 20;
        List<int> list;

        for(auto i = 1; i <= count; i++){
            list.insert(0, i);
            REQUIRE(list.at(0) == i);
        }

        REQUIRE(list.size() == count);

        for(auto i = count; i >= 1; i--){
            int v = list.at(0);
            REQUIRE(v == i);
            list.erase(0);
        }
    }

    SECTION("Back-like interaction")
    {
        std::size_t count = 20;
        List<int> list;

        for(auto i = 1; i <= count; i++){
            list.insert(i - 1, i);
            REQUIRE(list.at(i - 1) == i);
        }

        REQUIRE(list.size() == count);

        for(auto i = count; i >= 1; i--){
            int v = list.at(i - 1);
            REQUIRE(v == i);
            list.erase(i - 1);
        }
    }

    SECTION("Common interaction")
    {
        std::size_t count = 10;
        int element = 0;
        List<int> list(count, element);

        for(auto i = 1; i <= count; i += 2){
            list.insert(i, i);
        }

        for(auto i = 1; i <= count; i += 2){
            int v;
            REQUIRE_NOTHROW((v = list.at(i)));
            REQUIRE(v == i);
        }
    }
}