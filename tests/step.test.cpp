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
#include <cctype>
#include <parsecpp.hpp>

using Input = parsecpp::input::StringInput::Iterator;


parsecpp::Parser<char, Input> *h_parser = new parsecpp::common::CharParser<Input>('h');
parsecpp::Parser<char, Input> *space_parser = new parsecpp::common::CharParser<Input>(' ');

TEST_CASE("[steby] valid 1 char in input => success")
{
    parsecpp::input::StringInput input("h");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'h'));
    delete result;
}

TEST_CASE("[steby1] valid 1 char in input => success")
{
    parsecpp::input::StringInput input("h");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'h'));
    delete result;
}

TEST_CASE("[steby] empty input => success")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>({ }));
    delete result;
}

TEST_CASE("[steby1] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[steby] invalid input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>({ }));
    delete result;
}

TEST_CASE("[steby1] invalid input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[steby] many input => success")
{
    parsecpp::input::StringInput input("h h h h h h h");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(7, 'h'));
    delete result;
}

TEST_CASE("[steby1] many input => success")
{
    parsecpp::input::StringInput input("h h h h h h h");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(7, 'h'));
    delete result;
}

TEST_CASE("[steby] rested separator => success")
{
    parsecpp::input::StringInput input("h ");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'h'));
    CHECK(*(result->getInput()) == ' ');
    delete result;
}

TEST_CASE("[steby1] rested separator => success")
{
    parsecpp::input::StringInput input("h ");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(1, 'h'));
    CHECK(*(result->getInput()) == ' ');
    delete result;
}

TEST_CASE("[steby] rested separator after `many` input => success")
{
    parsecpp::input::StringInput input("h h h h h h h ");
    auto parser = parsecpp::Alternative<Input>::stepBy(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(7, 'h'));
    CHECK(*(result->getInput()) == ' ');
    delete result;
}

TEST_CASE("[steby1] rested separator after `many` input => success")
{
    parsecpp::input::StringInput input("h h h h h h h ");
    auto parser = parsecpp::Alternative<Input>::stepBy1(h_parser, space_parser);
    auto result = parser->execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->value() == std::vector<char>(7, 'h'));
    CHECK(*(result->getInput()) == ' ');
    delete result;
}
