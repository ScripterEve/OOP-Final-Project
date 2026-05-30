#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class InvalidIngredientException : public std::exception {
private:
    std::string message;
public:
    InvalidIngredientException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class RecipeNotFoundException : public std::exception {
private:
    std::string message;
public:
    RecipeNotFoundException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileLoadException : public std::exception {
private:
    std::string message;
public:
    FileLoadException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
