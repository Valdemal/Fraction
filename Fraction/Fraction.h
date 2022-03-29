#pragma once

#include <inttypes.h>
#include "FractionExceptions.h"


/*
 * Класс "Дробь" использует описанные ниже типы
 *
 * Вы можете переопределить типы на более длинные, чтобы повысить точность
 * расчетов, однако это увеличит размер дроби, или на более короткие, это
 * понизит точность расчетов, но уменьшит занимаемый классом объем памяти.
 * */

typedef double floatT;
typedef uint8_t unsignedT;
typedef int16_t integerT;
typedef char signT;

class Fraction { // Класс "Дробь"
public:

    /* КОНСТРУКТОРЫ */

    Fraction() = default;

    explicit Fraction(integerT numerator, integerT denominator);

    Fraction(integerT val);

    Fraction(const Fraction &copy);


    /* ГЕТТЕРЫ */

    inline signT getSign() const;

    inline unsignedT getNum() const { return num; };

    inline unsignedT getDen() const { return den; };


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
        return Fraction(f.getNum(), f.getDen(), 1);
    }

    friend Fraction pow(const Fraction &f, int n);

private:
    unsignedT num{0};
    unsignedT den{1};
    signT sign{1};

    Fraction(unsignedT num, unsignedT den, signT sign);

    // Сокращает дробь. Внимание! сокращение проводится только в приватном констукторе
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

    /* Возвращает true, если перемножение беззнаковых чисе*/
    static bool checkMultiplicationOverflow(unsignedT left, unsignedT right);
}; // Конец класса "Дробь"