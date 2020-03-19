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
#include <string>

using Input = parsecpp::input::StringInput::Iterator;

typename parsecpp::Parser<char, Input> *charParser(const char c)
{
    return parsecpp::PredicateMapper<Input>::map([c](const char x) { return c == x;});
}

TEST_CASE("[traverse] string parser => success")
{
    std::string value = "hello, buddy!";
    auto parser = parsecpp::Traversable<Input>::traverse
    (
        charParser,
        value.begin(),
        value.end()
    );
    
    parsecpp::input::StringInput input(value);
    auto result = parser->execute(input.begin());
    CHECK(*result == true);
    CHECK(result->getInput() == input.end());
    
    auto result_string = std::string(result->value().begin(), result->value().end());
    CHECK(value == result_string);
}

TEST_CASE("[traverse] invalid input, string parser => failure")
{
    std::string value = "hello, buddy!";
    auto parser = parsecpp::Traversable<Input>::traverse
    (
        charParser,
        value.begin(),
        value.end()
    );
    
    parsecpp::input::StringInput input("hello, value!");
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[traverse] empty input, string parser => failure")
{
    std::string value = "hello, buddy!";
    auto parser = parsecpp::Traversable<Input>::traverse
    (
        charParser,
        value.begin(),
        value.end()
    );
    
    parsecpp::input::StringInput input("");
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}
