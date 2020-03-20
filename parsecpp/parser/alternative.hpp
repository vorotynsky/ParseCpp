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
#include "container.hpp"

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
        inline static auto many(const Parser<T, I> *parser)
        {
            VectParser<T> *idParser = new Id<std::vector<T>, I> (std::vector<T>());
            VectParser<T> *someParser = new SomeParser<T, I>(parser);
            VectParser<T> *many = new AlternativeParser<std::vector<T>, I>(someParser, idParser);   

            auto node1 = new VectParserPair<T>(idParser, someParser);
            auto node2 = new VectParserTriple<T>(many, node1);

            return new Container<std::vector<T>, I, VectParserTriple<T>>(many, node2);
        }

        template <typename T>
        inline static auto some(const Parser<T, I> *parser)
        {
            return new SomeParser<T, I>(parser);
        }

    private:
        template <typename T>
        using VectParser = Parser<std::vector<T>, I>;

        template <typename T>
        using VectParserPair = DestructPair<VectParser<T>, VectParser<T>>;

        template <typename T>
        using VectParserTriple = DestructPair<VectParser<T>, VectParserPair<T>>;
    };
}
