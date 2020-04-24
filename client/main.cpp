#include <iostream>
#include <parsecpp.hpp>

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

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vect)
{
    for (const T &v : vect)
        std::cout << v << " ";
    return os;
}

int main()
{
    parsecpp::input::StringInput input("10-12-24-23-324124-123-2");
    auto result = dashSeparated->execute(input.begin());
    if (*result)
        std::cout << result->value() << std::endl;
}