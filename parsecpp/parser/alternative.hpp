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

#pragma once

#include <vector>

#include "parser.hpp"

namespace parsecpp
{
    template <typename T, typename I>
    class AlternativeParser final : public Parser <T, I>
    {
    public:
        AlternativeParser(const Parser<T, I> *parserA, const Parser<T, I> *parserB)
            : parserA(parserA), parserB(parserB) { }
        
        virtual ~AlternativeParser() = default;

        ParserResult<T, I> *execute(I input) const noexcept override
        {
            auto resultA = parserA->execute(input);
            if (*resultA)
                return resultA;
            else return parserB->execute(input);
        }

    private:
        const Parser<T, I> *parserA;
        const Parser<T, I> *parserB;
    };
}
