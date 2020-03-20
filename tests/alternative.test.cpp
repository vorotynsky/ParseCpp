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

const parsecpp::Parser<char, Input> *a_or_b_parser
    = new parsecpp::AlternativeParser<char, Input> 
    (
        parsecpp::PredicateMapper<Input>::map([](char c) { return c == 'a'; }),
        parsecpp::PredicateMapper<Input>::map([](char c) { return c == 'b'; })
    );

TEST_CASE("[alternative] first choice in input => success")
{
    parsecpp::input::StringInput input("animal");
    auto result = a_or_b_parser->execute(input.begin());
    CHECK(*result);
    CHECK(result->value() == 'a');
    CHECK(result->getInput() == ++input.begin());
}

TEST_CASE("[alternative] second choice in input => success")
{
    parsecpp::input::StringInput input("badger");
    auto result = a_or_b_parser->execute(input.begin());
    CHECK(*result);
    CHECK(result->value() == 'b');
    CHECK(result->getInput() == ++input.begin());
}

TEST_CASE("[alternative] invalid input => failure")
{
    parsecpp::input::StringInput input("castle");
    auto result = a_or_b_parser->execute(input.begin());
    CHECK(*result == false);
}

TEST_CASE("[alternative] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto result = a_or_b_parser->execute(input.begin());
    CHECK(*result == false);
}
