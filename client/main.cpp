#include <iostream>
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

const Parser<std::tuple<int, char>, Input> *composed = Monoid<Input>::compose(number, dash);
const Parser<int, Input> *numberDash = Functor<Input>::map([](const auto &t) {
    return std::get<0>(t);
}, composed);

const Parser<std::tuple<std::vector<int>, int>, Input> *a = Monoid<Input>::compose(
    Alternative<Input>::many(numberDash), number
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
    // Automaticaly delete nested parsers
    auto parser = new Container<std::vector<int>, Input, types::DestructingContainer>(
        new types::DestructingContainer({
            types::dwrap(digit), types::dwrap(dash), types::dwrap(digits), types::dwrap(number),
            types::dwrap(composed), types::dwrap(a), types::dwrap(dashSeparated)
        }),
        dashSeparated
    );


    parsecpp::input::StringInput input("10-12-24-23-324124-123-2");
    auto result = parser->execute(input.begin());
    if (*result)
        std::cout << result->value() << std::endl;

    delete parser;
}