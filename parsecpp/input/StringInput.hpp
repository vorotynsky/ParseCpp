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

#pragma once

#include <string>
#include "position.hpp"


namespace parsecpp::input
{
    class StringInput
    {
    public:
        class Iterator
        {
        private:
            int index;
            Position _position;
            const std::string *source;

            Iterator(const std::string *source);
            Iterator(const std::string *source, int index);

            inline void move();

        public:
            ~Iterator() = default;

            Iterator &operator++();
            Iterator operator++(int) const;

            Position position() const;
            char operator*() const;

            bool operator==(const Iterator &other) const;
            bool operator!=(const Iterator &other) const;

            friend StringInput;
        };

        StringInput(const std::string &source);
        ~StringInput() = default;

        Iterator begin() const;
        Iterator end() const;

    private:
        const std::string source;
    };
} // namespace parsecpp::input

