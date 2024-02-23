//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALC_CALCEXCEPTION_H
#define SMARTCALC_CALCEXCEPTION_H

#include <stdexcept>

namespace s21 {

class CalcException : public std::runtime_error {
   public:
    explicit CalcException(const char* arg) : runtime_error(arg){};
    virtual ~CalcException() = default;
};

class InputZeroLengthException : public CalcException {
   public:
    explicit InputZeroLengthException(const char* arg) : CalcException(arg){};
    virtual ~InputZeroLengthException() = default;
};

class InputTooLongException : public CalcException {
   public:
    explicit InputTooLongException(const char* arg) : CalcException(arg){};
    virtual ~InputTooLongException() = default;
};

class InputIncorrectException : public CalcException {
   public:
    explicit InputIncorrectException(const char* arg)
        : CalcException(arg){};
    virtual ~InputIncorrectException() = default;
};

class WrongOperatorException : public CalcException {
   public:
    explicit WrongOperatorException(const char* arg) : CalcException(arg){};
    virtual ~WrongOperatorException() = default;
};

class WrongXGraphException : public CalcException {
   public:
    explicit WrongXGraphException(const char* arg) : CalcException(arg){};
    virtual ~WrongXGraphException() = default;
};

class WrongYGraphException : public CalcException {
   public:
    explicit WrongYGraphException(const char* arg) : CalcException(arg){};
    virtual ~WrongYGraphException() = default;
};

}

#endif  // SMARTCALC_CALCEXCEPTION_H
