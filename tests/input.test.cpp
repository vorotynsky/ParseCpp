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
#include "../parsecpp/input/StringInput.hpp"

TEST_CASE("[string-input] empty input => 0-char")
{
    parsecpp::input::StringInput source("");
    auto it = source.begin();
    CHECK(it == source.end());
    CHECK(*it == '\0');
}

TEST_CASE("[string-input] string input => by-char")
{
    std::string test_value = "string: \"Hello world!\" ";
    parsecpp::input::StringInput source("");

    CHECK(std::equal(source.begin(), source.end(), test_value.begin()));
}

TEST_CASE("[string-input] after end => 0-char")
{
    std::string test_value = "string";
    parsecpp::input::StringInput source(test_value);

    REQUIRE(*source.end() == '\0');
}
