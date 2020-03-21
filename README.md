# ParseC++

Header only, easy to use **Parser combinator** library on C++.

RELEASED version 0.1.0

## Example

This code parses a sequence of dash-separated digits

```cpp

using namespace parsecpp;
using Input = parsecpp::input::StringInput::Iterator;

const Parser<char, Input> *digit = PredicateMapper<Input>::map([](char c) { return std::isdigit(c); }); 
const Parser<char, Input> *dash = PredicateMapper<Input>::map([](char c) { return c == '-'; });

const Parser<int, Input> *number = Functor<Input>::map([](const std::vector<char> &v) { 
    return std::atoi(std::string(v.begin(), v.end()).c_str());
}, Alternative<Input>::some(digit));

const Parser<int, Input> *numberDash = Functor<Input>::map([](const auto &t) {
    return std::get<0>(t);
}, Monoid<Input>::compose(number, dash));

const Parser<std::tuple<std::vector<int>, int>, Input> *a = Monoid<Input>::compose(
    Alternative<Input>::many(numberDash),
    number
);

const Parser<std::vector<int>, Input> *dashSeparated = Functor<Input>::map([](const auto &t) {
    auto v = std::get<0>(t);
    v.push_back(std::get<1>(t));
    return v;
}, a);


parsecpp::input::StringInput input("10-12-24-23-324124-123-2");
auto result = dashSeparated->execute(input.begin());
if (*result)
    std::cout << result->value();
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
