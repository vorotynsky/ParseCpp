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

namespace parsecpp 
{
    /// Parses a char using Predicate (char -> bool)
    template<typename I, typename Predicate>
    struct PredicateParser final : Parser<char, I>
    {
        PredicateParser(Predicate p)
            : predicate(p) { }

        ParserResult<char, I> *execute(I input) const noexcept override
        {
            char c = *input;
            if (c == '\0')
                return ParserResult<char, I>::Failure("unexpected empty input", input);
            if (predicate (*input))
                return ParserResult<char, I>::Success(c, ++input);
            std::string error = "unexpected char \'c\'";
            error[17] = c;
            return ParserResult<char, I>::Failure(error, input);
        }
        
        virtual ~PredicateParser() = default;

    private:
        Predicate predicate;
    };

    template<typename I>
    struct PredicateMapper final
    {
        template<typename P>
        static inline PredicateParser<I, P> *map(P predicate)
        {
            return new PredicateParser<I, P>(predicate);
        }
    };
}
