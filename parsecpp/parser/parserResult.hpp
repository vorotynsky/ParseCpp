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

#include <stdexcept>

namespace parsecpp
{
    template <typename T, typename P>
    class SuccessParserResult;

    template <typename T, typename P>
    class FailureParserResult;

    template <typename T, typename P>
    class ParserResult
    {
    public:
        static inline ParserResult<T, P> *Success(const T &value, const P input) noexcept 
        {
            return new SuccessParserResult<T, P>(input, value);
        }

        static inline ParserResult<T, P> *Failure(const std::string &error, const P input) noexcept
        {
            return new FailureParserResult<T, P>(input, error);
        }
        
        virtual operator bool() const noexcept
        {
            return false;
        }
        
        virtual const std::string &what() const
        {
            throw std::logic_error("This parser result haven't an error message. ");
        }

        virtual const T& value() const
        {
            throw std::logic_error("This parser result haven't a result value. ");
        }

        P getInput() const
        {
            return this->input;
        }
    
        virtual ~ParserResult() = default;
    protected:
        ParserResult(const P &input)
            : input(input) { }
    private:
        const P input;
    };

    template <typename T, typename P>
    class SuccessParserResult final : private ParserResult<T, P>
    {
        operator bool() const noexcept override
        {
            return true;
        }
        
        const T& value() const override
        {
            return _value;
        }

        SuccessParserResult(const P &input, const T &value)
            : ParserResult<T, P> (input), _value(value) { }

        const T _value;
        
        friend ParserResult<T, P>;
    };

    template <typename T, typename P>
    class FailureParserResult final : private ParserResult<T, P>
    {        
        const std::string &what() const override
        {
            return message;
        }

        FailureParserResult(const P &input, const std::string &message)
            : ParserResult<T, P> (input), message(message) { }

        const std::string message;
        
        friend ParserResult<T, P>;
    };
}
