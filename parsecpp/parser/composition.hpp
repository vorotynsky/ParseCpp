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

#include <tuple>
#include "parsecpp.hpp"
#include "container.hpp"
#include "id.hpp"

namespace parsecpp 
{
    template <typename T1, typename T2, typename I>
    class CompositionParser final : public Parser<std::tuple<T1, T2>, I>
    {
    public:
        CompositionParser(const Parser<T1, I> *parserA, const Parser<T2, I> *parserB)
            : parserA(parserA), parserB(parserB), Parser<std::tuple<T1, T2>, I>() { }

        ParserResult<std::tuple<T1, T2>, I> *execute(I input) const noexcept override
        {
            ParserResult<std::tuple<T1, T2>, I> *result;
            auto resultA = parserA->execute(input);
            if (*resultA)
            {
                auto resultB = parserB->execute(resultA->getInput());
                if (*resultB)
                {
                    auto tuple = std::make_tuple(resultA->value(), resultB->value());
                    result = ParserResult<std::tuple<T1, T2>, I>::Success(tuple, resultB->getInput());
                }
                else
                {
                    result = ParserResult<std::tuple<T1, T2>, I>::Failture(resultB->what(), input);
                }
                delete resultB;
            }
            else
            {
                result = ParserResult<std::tuple<T1, T2>, I>::Failture(resultA->what(), input);
            }
            
            delete resultA;
            return result;
        }

        virtual ~CompositionParser() = default;
    private:
        const Parser<T1, I> *parserA;
        const Parser<T2, I> *parserB;
    };

    template <typename I>
    struct Monoid final
    {
        template <typename T1, typename T2>
        static inline auto compose2(const Parser<T1, I> *p1, const Parser<T2, I> *p2)
        {
            return new CompositionParser<T1, T2, I>(p1, p2);
        }

        template <typename PA, typename PB>
        static inline auto compose(const PA *pa, const PB *pb)
        {
            return compose2(pa, pb);
        }

        template <typename PA, typename PB, typename... POther>
        static inline auto compose(const PA *pa, const PB *pb, const POther*... pother)
        {
            return compose(containerize(compose(pa, pb)), pother...); // contaierize parsers
        }

        template <typename P>
        static inline Parser<P, I> *id(const P& value)
        {
            return new Id<P, I>(value);
        }

    private:
        template <typename T>
        static inline auto containerize(const Parser<T, I> *parser)
        {
            return new Container<T, I, const Parser<T, I>>(parser, parser);
        }
    };
}
