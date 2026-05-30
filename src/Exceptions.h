#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidIngredientException : public std::invalid_argument {
public:
    InvalidIngredientException(const std::string& msg) : std::invalid_argument(msg) {}
};

class RecipeNotFoundException : public std::runtime_error {
public:
    RecipeNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileLoadException : public std::runtime_error {
public:
    FileLoadException(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidInputException : public std::runtime_error {
public:
    InvalidInputException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
