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

#include "StringInput.hpp"
#include <stdexcept>

namespace parsecpp::input
{
    StringInput::Iterator::Iterator(const std::string *source) 
        : Iterator(source, 0) { }

    StringInput::Iterator::Iterator(const std::string *source, int index) 
        : source(source), index(index) { }

    inline void StringInput::Iterator::move()
    {
        if (source->size() <= index)
            return;

        if (source->at(index) == '\n')
        {
            _position.column = 0;
            ++_position.line;
        }
        else
        {
            ++_position.column;
        }
        ++index;        
    }

    typename StringInput::Iterator &StringInput::Iterator::operator++()
    {
        this->move();
        return *this;
    }

    typename StringInput::Iterator StringInput::Iterator::operator++(int) const
    {
        auto newIt = *this;
        newIt.move();
        return newIt;
    }

    Position StringInput::Iterator::position() const
    {
        if (_position.line + _position.column == 0 && index != 0)
            throw std::logic_error("Can't get position on end iterator. ");
        return _position;
    }

    char StringInput::Iterator::operator*() const
    {
        return (source->size() <= index) ? '\0' : (*source)[index];
    }

    bool StringInput::Iterator::operator==(const Iterator &other) const
    {
        return source == other.source
            && index  == other.index;
    }

    bool StringInput::Iterator::operator!=(const Iterator &other) const
    {
        return !(*this == other);
    }

    StringInput::StringInput(const std::string &source)
        : source(source) { }

    typename StringInput::Iterator StringInput::begin() const
    {
        return Iterator(&source);
    }

    typename StringInput::Iterator StringInput::end() const
    {
        return Iterator(&source, source.size());
    }
} // namespace parsecpp::input
