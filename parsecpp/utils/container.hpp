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
#include <memory>
#include <initializer_list>

namespace parsecpp::types
{
    class Destructable
    {
    protected:
        Destructable() = default;
    public:
        virtual ~Destructable() = default;
    };

    template <typename T>
    struct DestructableElement final : Destructable
    {
        DestructableElement(const T *value)
            : value(value), Destructable() { }
        
        virtual ~DestructableElement()
        {
            delete value;
        }

    private:
        const T *value;
    };

    template <typename T>
    Destructable *dwrap(const T *value)
    {
        return new DestructableElement<T>(value);
    }

    class DestructingContainer final
    {
    public:
        DestructingContainer(const std::initializer_list<const Destructable *> &list)
            : data(list) { }

        ~DestructingContainer() 
        {
            for (auto &v : data)
                delete v;
        }

    private:
        std::vector<const Destructable *> data;
    };
}
