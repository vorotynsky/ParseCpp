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

#include "parser.hpp"

namespace parsecpp
{
    template <typename F, typename S, typename T, typename I>
    class MappedParser final : public Parser<T, I>
    {
    private:
        F function;
        const Parser<S, I> &parser;
    public:
        MappedParser(F function, const Parser<S, I> &parser)
            : function(function), parser(parser) { }

        virtual ~MappedParser() = default;

        ParserResult<T, I> *execute(I input) const noexcept override
        {
            ParserResult<S, I> *result = parser(input);
            ParserResult<T, I> *newResult;

            if (*result)
                newResult = ParserResult<T, I>::Success(function(result->value()), result->getInput());
            else 
                newResult = ParserResult<T, I>::Failture(result->what().c_str(), result->getInput());

            delete result;
            return newResult;
        }

        static T mapValue(F f, S s)
        {
            return f(s);
        }
    };

    template <typename I, typename T> // T here, coz compiler can't deduce it.
    struct Functor final
    {
        template <typename S, typename F>
        inline static Parser<T, I> *map(F function, const Parser<S, I> *parser)
        {
            return new MappedParser<F, S, T, I>(function, *parser);
        }
    };
}
