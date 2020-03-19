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
#include <string>

using Input = parsecpp::input::StringInput::Iterator;

TEST_CASE("[applicative] function in id-parser => function application")
{
    char testValue = 49;
    auto func = [](char c) { return c; };
    auto functionParser = new parsecpp::Id<decltype(func), Input>(func);
    auto valueParser = new parsecpp::Id<char, Input>(testValue);
    auto appliedParser = parsecpp::Applicative<Input>::apply(functionParser, valueParser);

    auto it = parsecpp::input::StringInput("").begin();
    auto result = appliedParser->execute(it);
    
    CHECK(*result);
    CHECK(result->value() == func(testValue));
    CHECK(result->getInput() == it); // because Id parser shouldn't move input. 
}

TEST_CASE("[applicative] empty function parser => failed")
{
    auto functionParser = new parsecpp::Empty<char(char), Input>("error function");
    auto valueParser = new parsecpp::Id<char, Input>('c');
    auto appliedParser = parsecpp::Applicative<Input>::apply(functionParser, valueParser);

    auto it = parsecpp::input::StringInput("").begin();
    auto result = appliedParser->execute(it);
    
    CHECK(*result == false);
    CHECK(result->what() == "error function");
    CHECK(result->getInput() == it); // because Id parser shouldn't move input. 
}
