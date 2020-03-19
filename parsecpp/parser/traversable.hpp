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
#include <algorithm>
#include <iterator>

namespace parsecpp
{
    template <typename F, typename T, typename Iterator, typename I>
    class TraversedParser final : public Parser<std::vector<T>, I>
    {
    public:
        TraversedParser(const F generator, Iterator begin, Iterator end)
            : begin(begin), end(end), generator(generator), Parser<std::vector<T>, I>() { }

        ParserResult<std::vector<T>, I> *execute(I input) const noexcept override
        {
            std::vector<T> vres;
            vres.resize(std::distance(begin, end));
            I parsed_input = input;

            for (auto it = begin, itr = vres.begin(); it != end; ++it, ++itr)
            {
                auto parser = generator(*it);
                auto result = parser->execute(parsed_input);
                delete parser;

                if (*result) 
                {
                    parsed_input = result->getInput();
                    *itr = result->value();
                    delete result;
                }
                else 
                {
                    auto newResult = ParserResult<std::vector<T>, I>::Failture(result->what(), input);
                    delete result;
                    return newResult;
                }
            }

            return ParserResult<std::vector<T>, I>::Success(vres, parsed_input);                     
        }

        virtual ~TraversedParser() = default;

    private:
        Iterator begin;
        Iterator end;
        const F generator;
    };

    template <typename I>
    struct Traversable
    {
        template <typename Iter>
        using IterType = typename std::iterator_traits<Iter>::value_type;

        template <typename Iter, typename F>
        static inline auto traverse(const F generator, Iter begin, Iter end)
        {
            return new TraversedParser<F, IterType<Iter>, Iter, I>(generator, begin, end);
        };
    };
}
