#include "FractionExceptions.h"

/* ИСКЛЮЧЕНИЯ */

ConstructionException::ConstructionException(const std::string &errorMessage) :
        std::invalid_argument(errorMessage) {}

InvalidInputException::InvalidInputException(const std::string &errorMessage) :
        std::invalid_argument(errorMessage) {}

ZeroDivisionException::ZeroDivisionException(const std::string &errorMessage) :
        std::runtime_error(errorMessage) {}

OverflowException::OverflowException(const std::string &errorMessage) :
        std::overflow_error(errorMessage) {}
