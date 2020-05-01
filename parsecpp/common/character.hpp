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

#include "../parser/parser.hpp"

namespace parsecpp::common
{
    template <typename I>
    class CharParser : public Parser<char, I>
    {
    public:
        CharParser(const char exemplar)
            : exemplar(exemplar), Parser<char, I>() { }

        virtual ~CharParser() = default;

        ParserResult<char, I> *execute(I input) const noexcept override
        {
            char c = *input;
            if (exemplar == c)
                return ParserResult<char, I>::Success(c, ++input);
            if (c == '\0')
                return ParserResult<char, I>::Failure("unexpected empty input", input);
            std::string error = "unexpected char \'c\'";
            error[17] = c;
            return ParserResult<char, I>::Failure(error, input);
        }

    private:
        const char exemplar;
    };
}
