// Copyright 2020 vorotynsky
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
    class ParserResult final
    {
    private:
        union
        {
            std::string error;
            T _value;
        };

        P input;

    private:
        bool isSuccess;

        ParserResult(const P &input, bool isSuccess)
            : input(input), isSuccess(isSuccess) { }

    public:
        static ParserResult<T, P> *Success(const T &value, const P input) noexcept {
            auto result = new ParserResult<T, P>(input, true);
            if (result == nullptr)
                return nullptr;
            result->_value = value;
            return result;
        }

        static ParserResult<T, P> *Failture(const std::string &error, const P input) noexcept
        {
            auto result = new ParserResult<T, P>(input, false);
            if (result == nullptr)
                return nullptr;
            result->error = error;
            return result;
        }
        
        operator bool() const noexcept
        {
            return this->isSuccess;
        }
        
        const std::string &what() const
        {
            if (isSuccess)
                throw std::logic_error("Success parser result haven't a result value. ");
            return this->error;
        }

        const T& value() const
        {
            if (!isSuccess)
                throw std::logic_error("Failture parser result haven't a result value. ");
            return this->_value;
        }

        P getInput() const
        {
            return this->input;
        }
    
        ~ParserResult() { }
    };
}
