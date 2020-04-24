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

const parsecpp::Parser<char, Input> *x_parser = new parsecpp::common::CharParser<Input>('x');
const parsecpp::Parser<char, Input> *y_parser = new parsecpp::common::CharParser<Input>('y');
const parsecpp::Parser<char, Input> *z_parser = new parsecpp::common::CharParser<Input>('z');

TEST_CASE("[monoid, compose] valid input for 3 parsers => success")
{
    parsecpp::input::StringInput input("xyz");
    auto combined = parsecpp::Monoid<Input>::compose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == true);
    CHECK(result->value() == std::make_tuple('x', 'y', 'z'));
}

TEST_CASE("[monoid, compose] invalid input for first parsers => failure")
{
    parsecpp::input::StringInput input("yyz");
    auto combined = parsecpp::Monoid<Input>::compose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, compose] invalid input for second parsers => failure")
{
    parsecpp::input::StringInput input("xxz");
    auto combined = parsecpp::Monoid<Input>::compose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, compose] invalid input for third parsers => failure")
{
    parsecpp::input::StringInput input("xyy");
    auto combined = parsecpp::Monoid<Input>::compose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, compose] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto combined = parsecpp::Monoid<Input>::compose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}



TEST_CASE("[monoid, lfold compose] valid input for 3 parsers => success")
{
    parsecpp::input::StringInput input("xyz");
    auto combined = parsecpp::Monoid<Input>::leftFoldCompose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == true);
    CHECK(result->value() == std::make_tuple(std::make_tuple('x', 'y'), 'z'));
}

TEST_CASE("[monoid, lfold compose] invalid input for first parsers => failure")
{
    parsecpp::input::StringInput input("yyz");
    auto combined = parsecpp::Monoid<Input>::leftFoldCompose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, lfold compose] invalid input for second parsers => failure")
{
    parsecpp::input::StringInput input("xxz");
    auto combined = parsecpp::Monoid<Input>::leftFoldCompose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, lfold compose] invalid input for third parsers => failure")
{
    parsecpp::input::StringInput input("xyy");
    auto combined = parsecpp::Monoid<Input>::leftFoldCompose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}

TEST_CASE("[monoid, lfold compose] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto combined = parsecpp::Monoid<Input>::leftFoldCompose(x_parser, y_parser, z_parser);
    auto result = combined->execute(input.begin());

    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
}
