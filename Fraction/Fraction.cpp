#include "Fraction.h"

/* КОНСТРУКТОРЫ */

Fraction::Fraction(unsignedT num, unsignedT den, signT sign)
        : num(num), den(den), sign(sign) {

    this->reduction();
}

Fraction::Fraction(integerT numerator, integerT denominator) {
    if (denominator == 0)
        throw ConstructionException("Denominator can't be zero!");

    *this = Fraction(abs(numerator), abs(denominator), numerator * denominator > 0 ? 1 : -1);
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

inline signT Fraction::getSign() const {
    return sign;
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

Fraction operator*(const Fraction &left, const Fraction &right) {
    // Проверка на переполнение
    if (Fraction::checkMultiplicationOverflow(left.getNum(), right.getNum()) ||
        Fraction::checkMultiplicationOverflow(left.getDen(), right.getDen()))
        throw OverflowException("Multiplication Overflow");

    return Fraction(
            left.getNum() * right.getNum(),
            left.getDen() * right.getDen(),
            left.getSign() * right.getSign()
    );

}

Fraction operator/(const Fraction &left, const Fraction &right) {
    // Проверка деления на нулевую дробь
    if (right.getNum() == 0)
        throw ZeroDivisionException("Division on zero fraction!");

    // Проверка на переполнение
    if (Fraction::checkMultiplicationOverflow(left.getNum(), right.getDen()) ||
        Fraction::checkMultiplicationOverflow(left.getDen(), right.getNum()))
        throw OverflowException("Division Overflow!");

    return Fraction(
            left.getNum() * right.getDen(),
            left.getDen() * right.getNum(),
            left.getSign() * right.getSign()
    );
}

Fraction operator-(const Fraction &left, const Fraction &right) {
    return left + (-right);
}

Fraction operator+(const Fraction &left, const Fraction &right) {
    Fraction::commonDenStruct cds(left, right);

    if (left.getSign() == right.getSign()) {
        // Проверка на переполнение при сложении
        if (Fraction::checkAdditionOverflow(cds.leftNum, cds.rightNum))
            throw OverflowException("Addition Overflow!");

        return Fraction(cds.leftNum + cds.rightNum, cds.commonDen, left.getSign());
    } else {
        unsignedT maxNum, minNum;
        signT maxSign;

        if (cds.leftNum > cds.rightNum) {
            maxNum = cds.leftNum;
            minNum = cds.rightNum;
            maxSign = left.getSign();
        } else if (cds.leftNum < cds.rightNum) {
            maxNum = cds.rightNum;
            minNum = cds.leftNum;
            maxSign = right.getSign();
        } else
            return Fraction();

        return Fraction(maxNum - minNum, cds.commonDen, maxSign);
    }
}


/* ОПЕРАТОРЫ СРАВНЕНИЯ */

bool operator==(const Fraction &left, const Fraction &right) {
    return Fraction::compareFractions(left, right) == 0;
}

bool operator!=(const Fraction &left, const Fraction &right) {
    return Fraction::compareFractions(left, right) != 0;
}

bool operator>(const Fraction &left, const Fraction &right) {
    return Fraction::compareFractions(left, right) == 1;
}

bool operator<(const Fraction &left, const Fraction &right) {
    return Fraction::compareFractions(left, right) == -1;
}

bool operator>=(const Fraction &left, const Fraction &right) {
    return Fraction::compareFractions(left, right) > -1;
}

bool operator<=(const Fraction &left, const Fraction &right) {
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
    unsignedT den;

    if (i == s.length())
        den = 1;
    else {
        if (s[i] == '/')
            i++;
        else
            throw InvalidInputException("Read Fraction Error");

        den = Fraction::readNumber(s, i);

        if (den == 0)
            throw ConstructionException("Denominator can't takes zero value!");
    }

    f = Fraction(num, den, sign);

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
    unsignedT k = gcd(this->num, this->den);

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

unsignedT Fraction::scm(unsignedT a, unsignedT b) {
    unsignedT k = gcd(a, b);
    return (a * b) / k;
}

char Fraction::compareFractions(const Fraction &left, const Fraction &right) {
    if (left.getSign() > right.getSign())
        return 1;
    else if (left.getSign() < right.getSign())
        return -1;
    else {
        commonDenStruct cds(left, right);

        bool isPositive = left.getSign() == 1;

        if (cds.leftNum > cds.rightNum)
            return isPositive ? 1 : -1;
        else if (cds.leftNum < cds.rightNum)
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

bool Fraction::checkMultiplicationOverflow(unsignedT left, unsignedT right) {
    if (left == 0 || right == 0)
        return false;

    unsignedT maxValueOfType = ~0;
    return (maxValueOfType / left) < right;
}

bool Fraction::checkAdditionOverflow(unsignedT left, unsignedT right) {
    unsignedT res = left + right;
    return res < left || res < right;
}

Fraction::commonDenStruct::commonDenStruct(const Fraction &left, const Fraction &right) {
    // Наверное можно сделать оптимальнее
    if (left.getDen() != right.getDen()) {
        if (checkMultiplicationOverflow(left.getDen(), right.getDen()) ||
            checkAdditionOverflow(left.getDen(), right.getDen()))

            throw OverflowException("Common Denominator Error!");

        this->commonDen = scm(left.getDen(), right.getDen());

        if (checkMultiplicationOverflow(left.getNum(), commonDen) ||
            checkMultiplicationOverflow(right.getNum(), commonDen))

            throw OverflowException("Common Denominator Error!");

        this->leftNum = left.getNum() * commonDen / left.getDen();
        this->rightNum = right.getNum() * commonDen / right.getDen();
    } else {
        this->commonDen = left.getDen();
        this->rightNum = right.getNum();
        this->leftNum = left.getNum();
    }
}
