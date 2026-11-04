/**
 * @file main.cpp
 * @brief Консольный интерфейс для проверки класса Matrix.
 */

#include <iostream>
#include <limits>
#include <windows.h>
#include "Matrix.h"

void printMenu() {
    std::cout << "\n========================================\n"
              << "          МЕНЮ РАБОТЫ С МАТРИЦЕЙ        \n"
              << "========================================\n"
              << "1. Создать новую матрицу (изменить размер)\n"
              << "2. Ввести элементы матрицы вручную\n"
              << "3. Вывести текущую матрицу на экран\n"
              << "4. Загрузить матрицу из файла\n"
              << "5. Транспонировать матрицу\n"
              << "6. Проверить свойства матрицы (тип)\n"
              << "7. Увеличить все элементы на 1 (преинкремент)\n"
              << "8. Выход\n"
              << "========================================\n"
              << "Выберите действие: ";
}

int main() {
    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Matrix m; // Создаем пустую матрицу по умолчанию

    int choice = 0;

    while (choice != 8) {
        printMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Пожалуйста, введите число.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    size_t r, c;
                    std::cout << "Введите количество строк и столбцов через пробел: ";
                    std::cin >> r >> c;
                    m.resize(r, c);
                    std::cout << "Размер матрицы изменен на " << r << "x" << c << ".\n";
                    break;
                }
                case 2: {
                    if (m.getRows() == 0 || m.getCols() == 0) {
                        std::cout << "Сначала задайте размер матрицы (пункт 1).\n";
                    } else {
                        std::cout << "Введите элементы матрицы (" << m.getRows() << "x" << m.getCols() << "):\n";
                        std::cin >> m; // Используем перегруженный оператор >>
                        std::cout << "Данные успешно введены.\n";
                    }
                    break;
                }
                case 3:
                    if (m.getRows() == 0) {
                        std::cout << "Матрица пуста.\n";
                    } else {
                        std::cout << "Текущая матрица:\n" << m; // Используем перегруженный оператор <<
                    }
                    break;
                case 4: {
                    std::string filename;
                    std::cout << "Введите имя файла (формат файла: строки столбцы элементы...): ";
                    std::cin >> filename;
                    m.loadFromFile(filename);
                    std::cout << "Матрица успешно загружена из файла.\n";
                    break;
                }
                case 5: {
                    if (m.getRows() == 0) {
                        std::cout << "Матрица пуста.\n";
                    } else {
                        Matrix t = m.transpose();
                        std::cout << "Результат транспонирования (новая матрица):\n" << t;
                    }
                    break;
                }
                case 6:
                    if (m.getRows() == 0) {
                        std::cout << "Матрица пуста.\n";
                    } else {
                        std::cout << "--- Свойства матрицы ---\n";
                        std::cout << "Квадратная:          " << (m.isSquare() ? "Да" : "Нет") << "\n";
                        std::cout << "Диагональная:        " << (m.isDiagonal() ? "Да" : "Нет") << "\n";
                        std::cout << "Единичная:           " << (m.isIdentity() ? "Да" : "Нет") << "\n";
                        std::cout << "Нулевая:             " << (m.isNull() ? "Да" : "Нет") << "\n";
                        std::cout << "Симметричная:        " << (m.isSymmetric() ? "Да" : "Нет") << "\n";
                        std::cout << "Верхняя треугольная: " << (m.isUpperTriangular() ? "Да" : "Нет") << "\n";
                        std::cout << "Нижняя треугольная:  " << (m.isLowerTriangular() ? "Да" : "Нет") << "\n";
                    }
                    break;
                case 7:
                    if (m.getRows() == 0) {
                        std::cout << "Матрица пуста.\n";
                    } else {
                        ++m; // Используем перегруженный преинкремент
                        std::cout << "Матрица после увеличения всех элементов на 1:\n" << m;
                    }
                    break;
                case 8:
                    std::cout << "Завершение работы программы...\n";
                    break;
                default:
                    std::cout << "Неверный выбор. Введите число от 1 до 8.\n";
            }
        } catch (const std::exception& e) {
            // Перехват исключений из Matrix.cpp (например, std::out_of_range или std::runtime_error)
            std::cout << "ОШИБКА: " << e.what() << "\n";
        }
    }
    return 0;
}