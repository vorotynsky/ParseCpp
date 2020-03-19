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
    template <typename F, typename S, typename T, typename I>
    class ApplicativeParser final : public Parser<T, I>
    {
    public:
        ApplicativeParser(const Parser<F, I> *function, const Parser<S, I> *arg)
            : function(function), arg(arg) { }

        virtual ~ApplicativeParser() = default;

        ParserResult<T, I> *execute(I input) const noexcept override
        {
            ParserResult<F, I> *funcRes = function->execute(input);
            ParserResult<T, I> *result = nullptr;
            if (*funcRes)
            {
                ParserResult<S, I> *argRes = arg->execute(funcRes->getInput());
                if (*argRes)
                {
                    auto value = funcRes->value() (argRes->value());
                    result = ParserResult<T, I>::Success(value, argRes->getInput());
                }
                else
                {
                    result = ParserResult<T, I>::Failture(argRes->what(), input);
                }
                
                delete argRes;
            }
            else
            {
                result = ParserResult<T, I>::Failture(funcRes->what(), input);
            }

            delete funcRes;
            return result;
        }

    private:
        const Parser<F, I> *function;
        const Parser<S, I> *arg;
    };

    template <typename I>
    struct Applicative
    {
        /// The type of the applying S to F
        template<typename F, typename S>
        using Result = decltype(std::declval<F>() (std::declval<S>()));

        template <typename F, typename Arg>
        inline static auto apply(const Parser<F, I> *pf, const Parser<Arg, I> *parg)
        {
            return new ApplicativeParser<F, Arg, Result<F, Arg>, I> (pf, parg);
        }
    };
}
