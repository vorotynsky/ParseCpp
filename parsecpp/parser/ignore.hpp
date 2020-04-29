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

#include <parsecpp.hpp>

namespace parsecpp
{
    template <typename T1, typename T2, typename I>
    class LeftIgnoreComposition final : public CompositionParser<T1, T2, T1, I>
    {
    protected:
        T1 compose(const T1& first, const T2& second) const noexcept override
        {
            return first;
        }
    public:
        LeftIgnoreComposition(const Parser<T1, I> *parserA, const Parser<T2, I> *parserB)
            : CompositionParser<T1, T2, T1, I>(parserA, parserB) { }
        
        virtual ~LeftIgnoreComposition() = default;
    };

    template <typename T1, typename T2, typename I>
    class RightIgnoreComposition final : public CompositionParser<T1, T2, T2, I>
    {
    protected:
        T1 compose(const T1& first, const T2& second) const noexcept override
        {
            return second;
        }
    public:
        RightIgnoreComposition(const Parser<T1, I> *parserA, const Parser<T2, I> *parserB)
            : CompositionParser<T1, T2, T2, I>(parserA, parserB) { }
        
        virtual ~RightIgnoreComposition() = default;
    };

    template <typename I>
    struct Ignore
    {
        template <typename T1, typename T2>
        PARSECPP_STATIC_API auto left(const Parser<T1, I> *left, const Parser<T2, I> *right)
        {
            return new LeftIgnoreComposition<T1, T2, I>(left, right);
        }

        template <typename T1, typename T2>
        PARSECPP_STATIC_API auto right(const Parser<T1, I> *left, const Parser<T2, I> *right)
        {
            return new RightIgnoreComposition<T1, T2, I>(left, right);
        }
    };
}
