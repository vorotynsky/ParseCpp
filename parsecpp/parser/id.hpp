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
    template <typename T, typename I>
    class Id final : public Parser<T, I>
    {
    public:
        Id(const T &value)
            : value(value), Parser<T, I>() { }

        ParserResult<T, I> *execute(I input) const noexcept override
        {
            return ParserResult<T, I>::Success(value, input);
        }

        virtual ~Id() = default;
    private:
        const T value;
    };
}
