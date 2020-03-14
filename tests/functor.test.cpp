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

const parsecpp::Parser<char, Input> *a_parser
    = parsecpp::PredicateMapper<Input>::map([](char c) { return c == 'a'; });

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

TEST_CASE("[functor] failture input => failture")
{
    parsecpp::input::StringInput input("ohh...");
    auto mapped = parsecpp::Functor<Input>::map(mulChar, a_parser);
    auto result = mapped->execute(input.begin());
    REQUIRE(*result == false);
}

TEST_CASE("[functor] empty input => failture")
{
    parsecpp::input::StringInput input("");
    auto mapped = parsecpp::Functor<Input>::map(mulChar, a_parser);
    auto result = mapped->execute(input.begin());
    REQUIRE(*result == false);
}
