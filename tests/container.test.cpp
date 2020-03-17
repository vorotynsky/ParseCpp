// Copyright 2020 vorotynsky
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

class rat final
{
    int *ptr;
public:
    rat(int *p) : ptr(p) { }
    ~rat() { ++*ptr; }
};

TEST_CASE("[container] delete parser => delete resource. expected result")
{
    int delete_counter = 0;
    rat *obj = new rat(&delete_counter);
    const std::string error = "Rats on the board!";
    auto emptyP = new parsecpp::Empty<char, Input>(error);
    auto parser = new parsecpp::Container<char, Input, rat> (emptyP, obj);

    parsecpp::input::StringInput input("The Age of Rats");
    auto result = parser->execute(input.begin());
    
    // CHECK the output of Empty parser
    CHECK(*result == false);
    CHECK(result->what() == error);

    delete parser;
    delete emptyP;

    CHECK(delete_counter == 1);
}