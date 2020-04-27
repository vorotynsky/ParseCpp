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

#include <tuple>

namespace parsecpp::functional
{
    template <typename... Types>
    constexpr auto lflatten(const std::tuple<Types...> &tuple)
    {
        return tuple;
    }

    template <typename A, typename B, typename... C>
    constexpr auto lflatten(const std::tuple<std::tuple<A, B>, C...> &tuple)
    {
        return std::tuple_cat(lflatten(std::get<0>(tuple)), std::make_tuple(std::get<1>(tuple)));
    }

    template <typename A, typename B, typename C, typename... D>
    constexpr auto lflatten(const std::tuple<std::tuple<std::tuple<A, B>, C>, D...> &tuple)
    {
        return std::tuple_cat(lflatten(std::get<0>(tuple)), std::make_tuple(std::get<1>(tuple)));
    }

    template <typename T1, typename T2>
    using LFlattenTuple = decltype(functional::lflatten(std::declval<std::tuple<T1, T2>>()));
}
