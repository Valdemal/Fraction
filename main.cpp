#include <iostream>
#include <aclapi.h>
#include "Fraction/Fraction.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8); // Подключение русского языка
    try {
        Fraction f1, f2;
        cout << "Введите первую дробь:";
        cin >> f1;
        cout << "Введите вторую дробь:";
        cin >> f2;
        Fraction res = f1 / f2;
        cout << "Результат:" << res << endl;
//
//        throw std::runtime_error("Опа, ошибка!");
    } catch (ConstructionException &e) {
        cout << "Ошибка при создании дроби!" << endl;
        cout << e.what();
    } catch (InvalidInputException &e) {
        cout << "Ошибка при вводе дроби!" << endl;
        cout << e.what();
    } catch (ZeroDivisionException &e) {
        cout << "Ошибка деления на 0!" << endl;
        cout << e.what();
    } catch (OverflowException &e) {
        cout << "Ошибка переполнения дроби!";
        cout << e.what();
    } catch (std::exception &e) {
        cout << "Какая-то ошибка!";
        cout << e.what();
    }
}