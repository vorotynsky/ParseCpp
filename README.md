# ParseC++

Header only, easy to use **Parser combinator** library on C++.

Released version 0.2.1

![ParseC++ Testing CI](https://github.com/vorotynsky/ParseCpp/workflows/ParseC++%20Testing%20CI/badge.svg?branch=master)

## Example

This code parses a sequence of dash-separated digits

```cpp
#include <parsecpp.hpp>
using namespace parsecpp;
using namespace parsecpp::common;
using Input = parsecpp::input::StringInput::Iterator;

const Parser<char, Input> *digit = PredicateMapper<Input>::map([](char c) { return std::isdigit(c); }); 
const Parser<char, Input> *dash = new CharParser<Input>('-');
const Parser<std::vector<char>, Input> *digits = Alternative<Input>::some(digit);

const Parser<int, Input> *number = Functor<Input>::map([](const std::vector<char> &v) { 
    return std::atoi(std::string(v.begin(), v.end()).c_str());
}, digits);

const Parser<std::vector<int>, Input> *dashSeparated = Alternative<Input>::stepBy1(number, dash);


auto parser = new Container<std::vector<int>, Input, types::DestructingContainer>(dashSeparated,
    new types::DestructingContainer({
        types::dwrap(digit), types::dwrap(dash), types::dwrap(digits),
        types::dwrap(number), types::dwrap(dashSeparated)
    })
);

parsecpp::input::StringInput input("10-12-24-23-324124-123-2");
auto result = parser->execute(input.begin());
if (*result)
    std::cout << result->value() << std::endl;

delete parser; // Automatically deletes nested parser, because it placed in container.
```

## Build

For use include `<parsecpp.hpp>`.

Build and run tests. Requires catch2.

```bash
cd build/
cmake .. -DENABLE_TESTING=ON
cmake --build .
ctest .
```
