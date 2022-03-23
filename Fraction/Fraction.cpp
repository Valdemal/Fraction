#include "Fraction.h"

/* КОНСТРУКТОРЫ */

Fraction::Fraction(unsignedT num, unsignedT den, signT sign) :
        num(num), den(den), sign(sign) {}

Fraction::Fraction(integerT numerator, integerT denominator) {
    if (denominator == 0)
        throw ZeroDenominatorException("Fraction construction error! Denominator can't be zero!");

    this->num = abs(numerator);
    this->den = abs(denominator);
    this->sign = numerator * denominator > 0 ? 1 : -1;

    this->reduction();
}

Fraction::Fraction(integerT val) {
    if (val < 0) {
        this->num = -val;
        this->sign = -1;
    } else
        this->num = val;
}


Fraction::Fraction(const Fraction &copy) :
        num(copy.num), den(copy.den), sign(copy.sign) {}


/* ГЕТТЕРЫ */

signT Fraction::getSign() const {
    return sign;
}

unsignedT Fraction::getNum() const {
    return num;
}

unsignedT Fraction::getDen() const {
    return den;
}


/* ОПЕРАЦИИ ПРИВЕДЕНИЯ К ТИПУ */

Fraction::operator floatT() const {
    return sign * (floatT(num) / den);
}

Fraction::operator std::string() const {
    std::string res;

    if (this->getSign() == -1)
        res += "-";

    res += std::to_string(this->getNum());

    if (this->getDen() != 1)
        res += "/" + std::to_string(this->getDen());

    return res;
}


/* МАТЕМАТИЧЕСКИЕ ОПЕРАТОРЫ */

Fraction Fraction::operator-() const {
    Fraction res(*this);
    res.sign = -res.sign;
    return res;
}

Fraction operator*(const Fraction &left, const Fraction &right){
    Fraction res(
            left.getNum() * right.getNum(),
            left.getDen() * right.getDen(),
            left.getSign() * right.getSign()
    );
    res.reduction();
    return res;
}

Fraction operator/(const Fraction &left, const Fraction &right){
    if (right.getNum() == 0)
        throw ZeroDenominatorException("Division on zero fraction!");

    Fraction res(
            left.getNum() * right.getDen(),
            left.getDen() * right.getNum(),
            left.getSign() * right.getSign()
    );

    res.reduction();
    return res;
}

Fraction operator-(const Fraction &left, const Fraction &right){
    return left + (-right);
}

Fraction operator+(const Fraction &left, const Fraction &right) {
    unsignedT commonDen = Fraction::scm(left.getDen(), right.getDen());

    unsignedT leftNum = left.getNum() * commonDen / left.getDen();
    unsignedT rightNum = right.getNum() * commonDen / right.getDen();

    Fraction res;

    if (left.getSign() == right.getSign()) {
        res = Fraction(leftNum + rightNum, commonDen, left.getSign());
    } else {
        unsignedT maxNum, minNum;
        signT maxSign;

        if (leftNum > rightNum) {
            maxNum = leftNum;
            minNum = rightNum;
            maxSign = left.sign;
        } else if (leftNum < rightNum) {
            maxNum = rightNum;
            minNum = leftNum;
            maxSign = right.sign;
        } else
            return Fraction();

        res = Fraction(maxNum - minNum, commonDen, maxSign);
    }

    res.reduction();
    return res;
}


/* ОПЕРАТОРЫ СРАВНЕНИЯ */

bool operator==(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) == 0;
}

bool operator!=(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) != 0;
}

bool operator>(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) == 1;
}

bool operator<(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) == -1;
}

bool operator>=(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) > -1;
}

bool operator<=(const Fraction &left, const Fraction &right){
    return Fraction::compareFractions(left, right) < 1;
}


/* ОПЕРАТОРЫ ПРИСВАИВАНИЯ */

Fraction &Fraction::operator=(const Fraction &other) {
    this->sign = other.getSign();
    this->num = other.getNum();
    this->den = other.getDen();

    return *this;
}

Fraction &Fraction::operator+=(const Fraction &other) {
    return *this = *this + other;
}

Fraction &Fraction::operator-=(const Fraction &other) {
    return *this = *this - other;
}

Fraction &Fraction::operator*=(const Fraction &other) {
    return *this = *this * other;
}

Fraction &Fraction::operator/=(const Fraction &other) {
    return *this = *this / other;
}


/* ОПЕРАТОРЫ ВВОДА/ВЫВОДА В ПОТОК */

std::istream &operator>>(std::istream &in, Fraction &f) {
    std::string s;
    in >> s;

    size_t i = 0;

    signT sign = 1;
    if (s[i] == '-') {
        i++;
        sign = -1;
    }

    unsignedT num = Fraction::readNumber(s, i);

    if (i == s.length())
        f.den = 1;
    else {
        if (s[i] == '/')
            i++;
        else
            throw InvalidFractionInputException("Read Fraction Error");

        unsignedT den = Fraction::readNumber(s, i);

        if (den == 0)
            throw ZeroDenominatorException("Denominator can't takes zero value!");

        f.den = den;
    }

    f.num = num;
    f.sign = sign;
    f.reduction();

    return in;
}

std::ostream &operator<<(std::ostream &out, const Fraction &f) {
    return out << std::string(f);
}


/* ДРУЖЕСТВЕННЫЕ МАТЕМАТИЧЕСКИЕ ФУНКЦИИ */

Fraction pow(const Fraction &f, int n) {

    if (n == 0) {
        return Fraction(1);
    } else {
        unsignedT num = f.getNum(), den = f.getDen();

        int absN = abs(n);
        for (size_t i = 1; i < absN; ++i) {
            num *= f.getNum();
            den *= f.getDen();
        }

        signT sign = 1;
        if (f.getSign() == -1 && absN % 2 == 1)
            sign = -1;

        if (n > 0)
            return Fraction(num, den, sign);
        else
            return Fraction(den, num, sign);
    }

}


/* ВСПОМОГАТЕЛЬНЫЕ ПРИВАТНЫЕ СТАТИЧЕСКИЕ ФУНКЦИИ*/

void Fraction::reduction() {
    integerT k = gcd(this->num, this->den);

    this->num /= k;
    this->den /= k;
}

unsignedT Fraction::gcd(unsignedT a, unsignedT b) {
    while (a * b != 0) {
        if (a > b)
            a %= b;
        else
            b %= a;
    }

    return a + b;
}

integerT Fraction::scm(unsignedT a, unsignedT b) {
    unsignedT k = gcd(a, b);
    return (a * b) / k;
}

char Fraction::compareFractions(const Fraction &left, const Fraction &right) {
    if (left.getSign() > right.getSign())
        return 1;
    else if (left.getSign() < right.getSign())
        return -1;
    else {
        unsignedT commonDen = scm(left.getDen(), right.getDen());

        unsignedT leftNum = left.getNum() * commonDen / left.getDen();
        unsignedT rightNum = right.getNum() * commonDen / right.getDen();

        bool isPositive = left.getSign() == 1;

        if (leftNum > rightNum)
            return isPositive ? 1 : -1;
        else if (leftNum < rightNum)
            return isPositive ? -1 : 1;
        else
            return 0;
    }
}

unsignedT Fraction::readNumber(const std::string &s, size_t &i) {
    unsignedT num = 0;

    while (s[i] <= '9' && s[i] >= '0' && i < s.length())
        num = num * 10 + s[i++] - '0';

    return num;
}


/* ИСКЛЮЧЕНИЯ */

FractionException::FractionException(std::string str) :
        errorMessage(std::move(str)) {}

const char *FractionException::what() const noexcept {
    return errorMessage.c_str();
}

ZeroDenominatorException::ZeroDenominatorException(const std::string &str) :
        FractionException(str) {};

InvalidFractionInputException::InvalidFractionInputException(const std::string &str) :
        FractionException(str) {}
