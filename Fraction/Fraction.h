#pragma once

#include <iostream>
#include <utility>

/*
 * Класс "Дробь" использует описанные ниже типы
 *
 * Вы можете переопределить типы на более длинные, чтобы повысить точность
 * расчетов, однако это увеличит размер дроби, или на более короткие, это
 * понизит точность расчетов, но уменьшит занимаемый классом объем памяти.
 * */
typedef double floatT;
typedef unsigned unsignedT;
typedef int integerT;
typedef char signT;

class ZeroDenominatorException;

class InvalidFractionInputException;

class Fraction { // Класс "Дробь"
public:

    /* КОНСТРУКТОРЫ */


    // Возможно сокращение дроби надо проводить в конструкторах а лучше создать в одном приватном
    Fraction() = default;

    explicit Fraction(integerT numerator, integerT denominator);

    Fraction(integerT val);

    Fraction(const Fraction &copy);


    /* ГЕТТЕРЫ */

    inline signT getSign() const;

    inline unsignedT getNum() const;

    inline unsignedT getDen() const;


    /* ОПЕРАТОРЫ ПРИВЕДЕНИЯ К ТИПУ */

    explicit operator floatT() const;

    explicit operator std::string() const;


    /* МАТЕМАТИЧЕСКИЕ ОПЕРАТОРЫ */

    Fraction operator-() const;

    friend Fraction operator+(const Fraction &left, const Fraction &right);

    friend Fraction operator-(const Fraction &left, const Fraction &right);

    friend Fraction operator*(const Fraction &left, const Fraction &right);

    friend Fraction operator/(const Fraction &left, const Fraction &right);

    /* ОПЕРАТОРЫ СРАВНЕНИЯ */

    friend bool operator==(const Fraction &left, const Fraction &right);

    friend bool operator!=(const Fraction &left, const Fraction &right);

    friend bool operator>(const Fraction &left, const Fraction &right);

    friend bool operator<(const Fraction &left, const Fraction &right);

    friend bool operator>=(const Fraction &left, const Fraction &right);

    friend bool operator<=(const Fraction &left, const Fraction &right);


    /* ОПЕРАТОРЫ ПРИСВАИВАНИЯ */

    Fraction &operator=(const Fraction &other);

    Fraction &operator+=(const Fraction &other);

    Fraction &operator-=(const Fraction &other);

    Fraction &operator*=(const Fraction &other);

    Fraction &operator/=(const Fraction &other);


    /* ОПЕРАТОРЫ ВВОДА/ВЫВОДА В ПОТОК */

    friend std::ostream &operator<<(std::ostream &out, const Fraction &f);

    friend std::istream &operator>>(std::istream &in, Fraction &f);


    /* ДРУЖЕСТВЕННЫЕ МАТЕМАТИЧЕСКИЕ ФУНКЦИИ */

    friend Fraction abs(const Fraction &f) {
        return Fraction(f.num, f.den, 1);
    }

    friend Fraction pow(const Fraction &f, int n);

private:
    unsignedT num{0};
    unsignedT den{1};
    signT sign{1};

    Fraction(unsignedT num, unsignedT den, signT sign);

    // Сокращает дробь
    void reduction();

    /* Сравнивает дроби, возвращает 0, если они равны, 1,
     * если left > right, -1, если left < right */
    static char compareFractions(const Fraction &left, const Fraction &right);

    // Возвращает наибольший общий делитель a и b
    static unsignedT gcd(unsignedT a, unsignedT b);

    // Возвращает наименьшее общее кратное a и b
    static integerT scm(unsignedT a, unsignedT b);

    /* Возращает беззнаковое число считанное из строки s начиная с позиции i
     * или 0, если считать число невозможно. i присваивается номер позиции
     * после считанного числа */
    static unsignedT readNumber(const std::string &s, size_t &i);

}; // Конец класса "Дробь"


class FractionException : public std::exception {
public:
    explicit FractionException(std::string str);

    const char *what() const noexcept override;

private:
    std::string errorMessage;
};

class ZeroDenominatorException : public FractionException {
public:
    explicit ZeroDenominatorException(const std::string &str);
};

class InvalidFractionInputException : public FractionException {
public:
    explicit InvalidFractionInputException(const std::string &str);
};