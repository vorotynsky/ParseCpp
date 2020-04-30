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

#include <vector>

#include "parser.hpp"
#include "some.hpp"
#include "step.hpp"
#include "container.hpp"
#include "../utils/container.hpp"

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

            delete resultA;
            return parserB->execute(input); 
        }

    private:
        const Parser<T, I> *parserA;
        const Parser<T, I> *parserB;
    };

    template <typename I>
    struct Alternative {

        template <typename T>
        PARSECPP_STATIC_API auto many(const Parser<T, I> *parser)
        {
            VectParser<T> *idParser = new Id<std::vector<T>, I> (std::vector<T>());
            VectParser<T> *someParser = new SomeParser<T, I>(parser);
            VectParser<T> *many = new AlternativeParser<std::vector<T>, I>(someParser, idParser);
            
            auto container = new types::DestructingContainer ({
                types::dwrap(idParser),
                types::dwrap(someParser),
                types::dwrap(many)
            });

            return new Container<std::vector<T>, I, types::DestructingContainer>(many, container);
        }

        template <typename T>
        PARSECPP_STATIC_API auto some(const Parser<T, I> *parser)
        {
            return new SomeParser<T, I>(parser);
        }

        template <typename T, typename S>
        PARSECPP_STATIC_API auto stepBy(const Parser<T, I> *parser, const Parser<S, I> *stepper)
        {
            auto id = new Id<std::vector<T>, I>(std::vector<T>());
            auto steps = new StepParser<T, S, I>(parser, stepper);
            auto stepby = new AlternativeParser<std::vector<T>, I>(steps, id);

            auto container = new types::DestructingContainer ({
                types::dwrap(id), types::dwrap(steps), types::dwrap(stepby)
            });

            return new Container<std::vector<T>, I, types::DestructingContainer>(stepby, container);
        }

        template <typename T, typename S>
        PARSECPP_STATIC_API auto stepBy1(const Parser<T, I> *parser, const Parser<S, I> *stepper)
        {
            return new StepParser<T, S, I>(parser, stepper);
        }

    private:
        template <typename T>
        using VectParser = Parser<std::vector<T>, I>;
    };
}
