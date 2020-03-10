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

TEST_CASE("[string-input] empty input => 0-char")
{
    parsecpp::input::StringInput source("");
    auto it = source.begin();
    CHECK(it == source.end());
    CHECK(*it == '\0');
    CHECK(it.position().column == 0);
    CHECK(it.position().line == 0);
}

TEST_CASE("[string-input] string input => by-char")
{
    std::string test_value = "string: \"Hello world!\" ";
    parsecpp::input::StringInput source(test_value);

    auto tv_it = test_value.begin();
    for(auto it = source.begin(); it != source.end(); tv_it++, ++it)
    {
        CHECK(*it == *tv_it);
        CHECK(it.position().column == std::distance(test_value.begin(), tv_it));
    }
}

TEST_CASE("[string-input] after end => 0-char")
{
    std::string test_value = "string";
    parsecpp::input::StringInput source(test_value);

    REQUIRE(*source.end() == '\0');
}

TEST_CASE("[string-input] new line => new position")
{
    parsecpp::input::StringInput source("hello\nworld");
    auto it = source.begin();
    while (it++ != source.end())
        ++it;

    CHECK(it.position().line == 1);
}
