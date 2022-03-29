#pragma once

#include <iostream>

class ConstructionException : public std::invalid_argument {
public:
    explicit ConstructionException(const std::string &errorMessage);
};

class InvalidInputException : public std::invalid_argument {
public:
    explicit InvalidInputException(const std::string &errorMessage);
};

class ZeroDivisionException : public std::runtime_error {
public:
    explicit ZeroDivisionException(const std::string &errorMessage);
};

class OverflowException : public std::overflow_error {
public:
    explicit OverflowException(const std::string &errorMessage);
};
