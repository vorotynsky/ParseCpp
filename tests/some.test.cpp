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

const parsecpp::Parser<char, Input> *m_parser = new parsecpp::common::CharParser<Input>('m');

TEST_CASE("[some] valid 1 char in input => success")
{
    parsecpp::input::StringInput input("m123");
    auto some = parsecpp::Alternative<Input>::some(m_parser);
    auto result = some->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'm'));

    delete result;
    delete some;
}

TEST_CASE("[some] valid 5 char in input => success")
{
    parsecpp::input::StringInput input("mmmmm123");
    auto some = parsecpp::Alternative<Input>::some(m_parser);
    auto result = some->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(5, 'm'));

    delete result;
    delete some;
}

TEST_CASE("[some] invalid input => failure")
{
    parsecpp::input::StringInput input("zmmmmmmm123");
    auto some = parsecpp::Alternative<Input>::some(m_parser);
    auto result = some->execute(input.begin());
    
    CHECK(*result == false);

    delete result;
    delete some;
}

TEST_CASE("[many] valid 1 char in input => success")
{
    parsecpp::input::StringInput input("m23");
    auto many = parsecpp::Alternative<Input>::many(m_parser);
    auto result = many->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'm'));

    delete result;
    delete many;
}

TEST_CASE("[many] valid 5 char in input => success")
{
    parsecpp::input::StringInput input("mmmmm123");
    auto many = parsecpp::Alternative<Input>::many(m_parser);
    auto result = many->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(5, 'm'));

    delete result;
    delete many;
}

TEST_CASE("[many] invalid input => success")
{
    parsecpp::input::StringInput input("123");
    auto many = parsecpp::Alternative<Input>::many(m_parser);
    auto result = many->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(0, 'm'));

    delete result;
    delete many;
}
