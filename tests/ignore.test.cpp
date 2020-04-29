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

parsecpp::Parser<char, Input> *left_parser = new parsecpp::common::CharParser<Input>('a');
parsecpp::Parser<char, Input> *right_parser = new parsecpp::common::CharParser<Input>('b');


TEST_CASE("[ignore, left] first choise => success")
{
    parsecpp::input::StringInput input("ab");
    auto parser = parsecpp::Ignore<Input>::left(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == true);
    CHECK(result->value() == 'a');
    delete result;
}

TEST_CASE("[ignore, right] second choise => success")
{
    parsecpp::input::StringInput input("ab");
    auto parser = parsecpp::Ignore<Input>::right(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == true);
    CHECK(result->value() == 'b');
    delete result;
}

TEST_CASE("[ignore, left] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Ignore<Input>::left(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[ignore, right] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Ignore<Input>::right(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[ignore, left] invalid first char in input => success")
{
    parsecpp::input::StringInput input("bb");
    auto parser = parsecpp::Ignore<Input>::left(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[ignore, right] invalid first char in input => success")
{
    parsecpp::input::StringInput input("bb");
    auto parser = parsecpp::Ignore<Input>::right(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[ignore, left] invalid second char in input => success")
{
    parsecpp::input::StringInput input("aa");
    auto parser = parsecpp::Ignore<Input>::left(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[ignore, right] invalid second char in input => success")
{
    parsecpp::input::StringInput input("aa");
    auto parser = parsecpp::Ignore<Input>::right(left_parser, right_parser);
    auto result = parser->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}
