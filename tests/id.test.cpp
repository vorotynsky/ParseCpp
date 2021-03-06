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

struct test_value final
{
    test_value(const std::string &a, int b)
        : a(a), b(b) { }

    std::string a;
    int b;

    bool operator==(const test_value &other) const
    {
        return a == other.a && b == other.b;
    }
};

TEST_CASE("[id] empty input => success")
{
    test_value value("hello", 10);
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Id<test_value, Input>(value);
    auto result = parser.execute(input.begin());
    CHECK(*result == true);
    CHECK(result->getInput() == input.begin());
    CHECK(result->value() == value);
    delete result;
}

TEST_CASE("[id] an input => success")
{
    test_value value("hello", 10);
    parsecpp::input::StringInput input("text");
    auto parser = parsecpp::Id<test_value, Input>(value);
    auto result = parser.execute(input.begin());
    CHECK(*result == true);
    CHECK(result->getInput() == input.begin());
    CHECK(result->value() == value);
    delete result;
}

TEST_CASE("[empty] empty input => failure")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Empty<test_value, Input>("error");
    auto result = parser.execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[empty] an input => failure")
{
    parsecpp::input::StringInput input("text");
    auto parser = parsecpp::Empty<test_value, Input>("error");
    auto result = parser.execute(input.begin());
    CHECK(*result == false);
    CHECK(result->getInput() == input.begin());
    delete result;
}

TEST_CASE("[label] success => success")
{
    test_value value("hello", 10);
    parsecpp::input::StringInput input("text");
    auto parser = parsecpp::Id<test_value, Input>(value);
    auto labeled = parsecpp::Label<test_value, Input>(&parser, "label error message");
    auto result = labeled.execute(input.begin());
    
    CHECK(*result == true);
    CHECK(result->getInput() == input.begin());
    CHECK(result->value() == value);
    delete result;
}

TEST_CASE("[label] failure => failure with replaced message ")
{
    parsecpp::input::StringInput input("");
    auto parser = parsecpp::Empty<test_value, Input>("error");
    auto labeled = parsecpp::Label<test_value, Input>(&parser, "label error message");
    auto result = labeled.execute(input.begin());

    CHECK(*result == false);
    CHECK(result->what() == "label error message");
    CHECK(result->getInput() == input.begin());
    delete result;
}
