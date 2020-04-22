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
#include "../utils/flatten.hpp"

namespace parsecpp 
{
    template <typename T1, typename T2, typename TResult, typename I>
    class CompositionParser : public Parser<TResult, I>
    {
    public:
        ParserResult<TResult, I> *execute(I input) const noexcept override
        {
            ParserResult<TResult, I> *result;
            auto resultA = parserA->execute(input);
            if (*resultA)
            {
                auto resultB = parserB->execute(resultA->getInput());
                if (*resultB)
                {
                    auto tuple = this->compose(resultA->value(), resultB->value());
                    result = ParserResult<TResult, I>::Success(tuple, resultB->getInput());
                }
                else
                {
                    result = ParserResult<TResult, I>::Failure(resultB->what(), input);
                }
                delete resultB;
            }
            else
            {
                result = ParserResult<TResult, I>::Failure(resultA->what(), input);
            }
            
            delete resultA;
            return result;
        }

        virtual ~CompositionParser() = default;

    protected:
        CompositionParser(const Parser<T1, I> *parserA, const Parser<T2, I> *parserB)
            : parserA(parserA), parserB(parserB), Parser<TResult, I>() { }

        virtual TResult compose(const T1&, const T2&) const noexcept = 0;

    private:
        const Parser<T1, I> *parserA;
        const Parser<T2, I> *parserB;
    };


    template <typename T1, typename T2, typename I>
    class FoldComposition final : public CompositionParser<T1, T2, std::tuple<T1, T2>, I>
    {
    protected:
        std::tuple<T1, T2> compose(const T1& first, const T2& second) const noexcept override
        { 
            return std::make_tuple(first, second);
        }
    public:
        FoldComposition(const Parser<T1, I> *parserA, const Parser<T2, I> *parserB)
            : CompositionParser<T1, T2, std::tuple<T1, T2>, I>(parserA, parserB) { }
        
        virtual ~FoldComposition() = default;
    };


    template <typename T1, typename T2, typename I>
    class LeftFlattenComposition final : public CompositionParser<T1, T2, functional::LFlattenTuple<T1, T2>, I>
    {
    protected:
        typename functional::LFlattenTuple<T1, T2> compose(const T1& first, const T2& second) const noexcept override
        { 
            return functional::lflatten(std::make_tuple(first, second));
        }
    public:
        LeftFlattenComposition(const Parser<T1, I> *pa, const Parser<T2, I> *pb)
            : CompositionParser<T1, T2, functional::LFlattenTuple<T1, T2>, I>(pa, pb) { }
        
        virtual ~LeftFlattenComposition() = default;
    };


    template <typename I>
    struct Monoid final
    {
        template <typename T1, typename T2>
        static inline auto compose2(const Parser<T1, I> *p1, const Parser<T2, I> *p2)
        {
            return new FoldComposition<T1, T2, I>(p1, p2);
        }

        template <typename PA, typename PB, typename... POther>
        static inline auto leftFoldCompose(const PA *pa, const PB *pb)
        {
            return compose2(pa, pb);
        }

        template <typename PA, typename PB, typename... POther>
        static inline auto leftFoldCompose(const PA *pa, const PB *pb, const POther*... pother)
        {
            return leftFoldCompose(containerize(leftFoldCompose(pa, pb)), pother...);
        }

        template <typename PA, typename PB, typename... POther>
        static inline auto compose(const PA *pa, const PB *pb)
        {
            return leftFlattenCompose2(pa, pb);
        }

        template <typename PA, typename PB, typename... POther>
        static inline auto compose(const PA *pa, const PB *pb, const POther*... pother)
        {
            return compose(containerize(compose(pa, pb)), pother...);
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

        template <typename T1, typename T2>
        static inline auto leftFlattenCompose2(const Parser<T1, I> *p1, const Parser<T2, I> *p2)
        {
            return new LeftFlattenComposition<T1, T2, I>(p1, p2);
        }
    };
}
