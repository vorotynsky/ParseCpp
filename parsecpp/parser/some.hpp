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

#include "parser.hpp"
#include <vector>

namespace parsecpp 
{
    template <typename T, typename I>
    class SomeParser final : public Parser<std::vector<T>, I>
    {
    public:
        SomeParser(const Parser<T, I> *parser)
            : parser(parser), Parser<std::vector<T>, I>() { }

        ParserResult<std::vector<T>, I> *execute(I init_input) const noexcept override
        {
            std::vector<T> vect;
            auto result = parser->execute(init_input);
            auto input = result->getInput();
            while (*result == true)
            {
                vect.push_back(result->value());
                input = result->getInput();
                delete result;
                result = parser->execute(input);
            }

            ParserResult<std::vector<T>, I> *newResult;
            if (vect.size() == 0)
                newResult = ParserResult<std::vector<T>, I>::Failture(result->what(), result->getInput());
            else newResult = ParserResult<std::vector<T>, I>::Success(vect, init_input);
            
            delete result;
            return newResult;
        }

        virtual ~SomeParser() = default;
    private:
        const Parser<T, I> *parser;
    };
}
