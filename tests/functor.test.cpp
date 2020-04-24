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

const parsecpp::Parser<char, Input> *a_parser = new parsecpp::common::CharParser<Input>('a');

std::string mulChar(char c)
{
    return std::string(3, c);
}

TEST_CASE("[functor] valid input => success")
{
    parsecpp::input::StringInput input("aloha");
    auto mapped = parsecpp::Functor<Input>::map(mulChar, a_parser);
    auto result = mapped->execute(input.begin());
    REQUIRE(*result == true);
    CHECK(result->value() == "aaa");
}

TEST_CASE("[functor] failure input => failure")
{
    parsecpp::input::StringInput input("ohh...");
    auto mapped = parsecpp::Functor<Input>::map(mulChar, a_parser);
    auto result = mapped->execute(input.begin());
    REQUIRE(*result == false);
}

TEST_CASE("[functor] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto mapped = parsecpp::Functor<Input>::map(mulChar, a_parser);
    auto result = mapped->execute(input.begin());
    REQUIRE(*result == false);
}

const parsecpp::Parser<int, Input> *parser_3
    = parsecpp::Functor<Input>::map(
        [](char c) { return (int) c - '0'; },
        new parsecpp::common::CharParser<Input>('3')
    );

std::string mulCharT(char c, int times)
{
    return std::string(times, c);
}

TEST_CASE("[functor fmap] valid input => success")
{
    parsecpp::input::StringInput input("a3asas");
    REQUIRE(parser_3->execute(++input.begin())->value() == 3);

    auto mapped = parsecpp::Functor<Input>::fmap(mulCharT, a_parser);
    auto applied = parsecpp::Applicative<Input>::apply(mapped, parser_3);
    auto result = applied->execute(input.begin());
    CHECK(*result == true);
    CHECK(result->value() == "aaa");
}

TEST_CASE("[functor fmap] empty input => failure")
{
    parsecpp::input::StringInput input("");

    auto mapped = parsecpp::Functor<Input>::fmap(mulCharT, a_parser);
    auto applied = parsecpp::Applicative<Input>::apply(mapped, parser_3);
    auto result = applied->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[functor fmap] invalid input for parser 1 => failure")
{
    parsecpp::input::StringInput input("b3asas");
    REQUIRE(parser_3->execute(++input.begin())->value() == 3);

    auto mapped = parsecpp::Functor<Input>::fmap(mulCharT, a_parser);
    auto applied = parsecpp::Applicative<Input>::apply(mapped, parser_3);
    auto result = applied->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    CHECK(result->what() == "unexpected char \'b\'");
}

TEST_CASE("[functor fmap] invalid input for parser 2 => failure")
{
    parsecpp::input::StringInput input("a4asas");

    auto mapped = parsecpp::Functor<Input>::fmap(mulCharT, a_parser);
    auto applied = parsecpp::Applicative<Input>::apply(mapped, parser_3);
    auto result = applied->execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    CHECK(result->what() == "unexpected char \'4\'");
}
