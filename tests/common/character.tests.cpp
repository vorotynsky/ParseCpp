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


TEST_CASE("[character] valid input => success result")
{
    parsecpp::input::StringInput input("cool");
    auto parser = new parsecpp::common::CharParser<Input>('c');
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == 'c');
    CHECK(result->getInput() == ++input.begin());
    CHECK_THROWS(result->what());
}


TEST_CASE("[character] invalid input => failure result")
{
    parsecpp::input::StringInput input("wool");
    auto parser = new parsecpp::common::CharParser<Input>('c');
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected char 'w'");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}


TEST_CASE("[character] empty input => failure result")
{
    parsecpp::input::StringInput input("");
    auto parser = new parsecpp::common::CharParser<Input>('c');
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->what() == "unexpected empty input");
    CHECK(result->getInput() == input.begin());
    CHECK_THROWS(result->value());
}
