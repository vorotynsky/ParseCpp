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
#include "../utils/container.hpp"

namespace parsecpp
{
    template <typename T, typename S, typename I>
    class StepParser final : public Parser<std::vector<T>, I>
    {
    public:
        ParserResult<std::vector<T>, I> *execute(I input) const noexcept
        {
            auto vresult = value->execute(input);
            auto _vrd = types::autodestruct(&vresult);

            if (*vresult == false)
                return ParserResult<std::vector<T>, I>::Failure(
                    vresult->what(), 
                    vresult->getInput()
                );

            std::vector<T> vres;
            vres.push_back(vresult->value());
            I it = vresult->getInput();

            while (true)
            {
                auto sresult = sep->execute(it);
                auto _srd = types::autodestruct(&sresult);

                if (*sresult == false)
                    return ParserResult<std::vector<T>, I>::Success(vres, it);

                auto vresult = value->execute(sresult->getInput());
                auto _vrd = types::autodestruct(&vresult);

                if (*vresult == false)
                    return ParserResult<std::vector<T>, I>::Success(vres, it);
                
                vres.push_back(vresult->value());
                it = vresult->getInput();                
            }
        }

        StepParser(const Parser<T, I> *value, const Parser<S, I> *sep)
            : value(value), sep(sep), Parser<std::vector<T>, I>() { }

    private:
        const Parser<T, I> *value;
        const Parser<S, I> *sep;
    };
}

