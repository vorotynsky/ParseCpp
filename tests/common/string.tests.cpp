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

#define APPEND4(a) ++(++(++(++a)))

TEST_CASE("[string] valid input => success result")
{
    parsecpp::input::StringInput input("cool");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == "cool");
    CHECK(result->getInput() == APPEND4(input.begin()));
    CHECK_THROWS(result->what());
}

TEST_CASE("[string] part of input => success result")
{
    parsecpp::input::StringInput input("cool.");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == "cool");
    CHECK(result->getInput() == APPEND4(input.begin()));
    CHECK(*result->getInput() == '.');
    CHECK_THROWS(result->what());
}

TEST_CASE("[string] invalid first input => failure result")
{
    parsecpp::input::StringInput input("wool");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected char 'w'");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}


TEST_CASE("[string] invalid second input => failure result")
{
    parsecpp::input::StringInput input("cul");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected char 'u'");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}

TEST_CASE("[string] invalid last input => failure result")
{
    parsecpp::input::StringInput input("cooo");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected char 'o'");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}

TEST_CASE("[string] unexpected end input => failure result")
{
    parsecpp::input::StringInput input("coo");
    auto parser = parsecpp::Traversable<Input>::string("cool");
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected empty input");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}

TEST_CASE("[string] empty input => failure result")
{
    parsecpp::input::StringInput input("");
    auto parser = new parsecpp::common::CharParser<Input>('c');
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected empty input");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}
