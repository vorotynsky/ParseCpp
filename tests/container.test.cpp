// Copyright 2020 Vorotynsky Maxim
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <catch2/catch.hpp>
#include <parsecpp.hpp>

using Input = parsecpp::input::StringInput::Iterator;

template <typename T>
class rat final
{
    T *ptr;
public:
    rat(T *p) : ptr(p) { }
    ~rat() { ++*ptr; }
};

TEST_CASE("[container] delete parser => delete resource. expected result")
{
    int delete_counter = 0;
    rat<int> *obj = new rat<int>(&delete_counter);
    const std::string error = "Rats on the board!";
    auto emptyP = new parsecpp::Empty<char, Input>(error);
    auto parser = new parsecpp::Container<char, Input, rat<int>> (emptyP, obj);

    parsecpp::input::StringInput input("The Age of Rats");
    auto result = parser->execute(input.begin());
    
    // CHECK the output of Empty parser
    CHECK(*result == false);
    CHECK(result->what() == error);

    delete parser;
    delete emptyP;

    CHECK(delete_counter == 1);
}

TEST_CASE("[container] delete many intems => delete resources")
{
    using namespace parsecpp::types;

    struct {
        int a = 0;
        long b = 0;
        short c = 0;
    } delete_counters = {0, 0, 0};


    auto container = new DestructingContainer ({
        dwrap(new rat<int>(&delete_counters.a)),
        dwrap(new rat<long>(&delete_counters.b)),
        dwrap(new rat<short>(&delete_counters.c))
    });

    const std::string error = "Rats on the board!";
    auto emptyP = new parsecpp::Empty<char, Input>(error);
    auto parser = new parsecpp::Container<char, Input, DestructingContainer>(emptyP, container);

    parsecpp::input::StringInput input("The Age of Rats");
    auto result = parser->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->what() == error);

    delete parser;
    delete emptyP;

    CHECK(delete_counters.a == 1);
    CHECK(delete_counters.b == 1);
    CHECK(delete_counters.c == 1);
}
