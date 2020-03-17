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

namespace parsecpp 
{
    template <typename T, typename I, typename R>
    class Container final : public Parser<T, I>
    {
    public:
        Container(const Parser<T, I> *parser, R *resource)
            : parser(parser), Parser<T, I>() 
            {
                this->resource = resource;
            }

        ParserResult<T, I> *execute(I input) const noexcept override
        {
            return parser->execute(input);
        }

        virtual ~Container()
        {
            delete resource;
        }

    private:
        const Parser<T, I> *parser;
        const R *resource;
    };
}

