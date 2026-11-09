#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif
#include "TuringMachine.h"


// Загружает демонстрационную программу: прибавление 1 к двоичному числу.
// Например, "1011" (11) превратится в "1100" (12).

void loadBinaryIncrementDemo(TuringMachine& tm) {
    tm.loadTape("1011");
    tm.setStates("q_right", "q_halt");

    // 1. Идем до самого правого края числа
    tm.addRule(Rule("q_right", '0', "q_right", '0', Direction::RIGHT));
    tm.addRule(Rule("q_right", '1', "q_right", '1', Direction::RIGHT));
    tm.addRule(Rule("q_right", '_', "q_add",   '_', Direction::LEFT));

    // 2. Складываем с переносом разряда справа налево
    tm.addRule(Rule("q_add", '1', "q_add",  '0', Direction::LEFT));
    tm.addRule(Rule("q_add", '0', "q_halt", '1', Direction::NONE));
    tm.addRule(Rule("q_add", '_', "q_halt", '1', Direction::NONE));
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    bool logMode = false;
    std::string filename = "";

    // Разбор аргументов командной строки (ищем ключ -log и имя файла)
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-log") == 0) {
            logMode = true;
        } else {
            filename = argv[i];
        }
    }

    TuringMachine tm;

    // Если передан файл через командную строку, сразу исполняем его
    if (!filename.empty()) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
            return 1;
        }
        try {
            tm.loadFromStream(file);
            tm.run(logMode);
            std::cout << "Результат на ленте: " << tm.getTapeContent() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Ошибка выполнения: " << e.what() << "\n";
            return 1;
        }
        return 0;
    }

    // Надежный интерактивный цикл (не закроется сам при ошибке ввода)
    while (true) {
        std::cout << "\n============================================\n";
        std::cout << "   МАШИНА ТЬЮРИНГА (Логирование -log: " << (logMode ? "ВКЛ" : "ВЫКЛ") << ")\n";
        std::cout << "============================================\n";
        std::cout << "1. Загрузить демо-программу (бинарный инкремент 1011 + 1)\n";
        std::cout << "2. Загрузить программу и ленту из файла\n";
        std::cout << "3. Задать ленту и состояния вручную\n";
        std::cout << "4. Добавить правило перехода\n";
        std::cout << "5. Удалить правило перехода\n";
        std::cout << "6. Выполнить 1 шаг\n";
        std::cout << "7. Запустить до конца (Run)\n";
        std::cout << "8. Переключить режим логирования (-log)\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите действие: ";

        std::string choiceStr;
        if (!std::getline(std::cin >> std::ws, choiceStr)) {
            break;
        }

        int choice = -1;
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            std::cout << "[Ошибка] Введите номер пункта меню (число от 0 до 8).\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "Выход из программы.\n";
            break;
        }

        try {
            if (choice == 1) {
                TuringMachine freshTm;
                loadBinaryIncrementDemo(freshTm);
                tm = freshTm;
                std::cout << "Демо загружено! Начальная лента: " << tm.getTapeContent() << "\n";
            } else if (choice == 2) {
                std::string path;
                std::cout << "Введите имя файла: ";
                std::getline(std::cin >> std::ws, path);
                std::ifstream f(path);
                if (!f.is_open()) {
                    std::cout << "Не удалось открыть файл.\n";
                } else {
                    tm.loadFromStream(f);
                    std::cout << "Программа успешно загружена.\n";
                }
            } else if (choice == 3) {
                std::string data, sState, hState;
                std::cout << "Введите начальную ленту (без пробелов, например 1011): ";
                std::cin >> data;
                std::cout << "Начальное состояние (например q0): ";
                std::cin >> sState;
                std::cout << "Финальное состояние (например qf): ";
                std::cin >> hState;
                std::cin.ignore(10000, '\n');

                tm.loadTape(data);
                tm.setStates(sState, hState);
                std::cout << "Лента и состояния установлены.\n";
            } else if (choice == 4) {
                std::cout << "Введите 5 параметров ЧЕРЕЗ ПРОБЕЛ:\n";
                std::cout << "Формат: тек_сост чит_симв нов_сост пис_симв L/R/N\n";
                std::cout << "Пример: q0 1 q0 0 R\nВвод: ";

                std::string line;
                std::getline(std::cin >> std::ws, line);
                std::istringstream iss(line);

                std::string cs, ns;
                char rs, ws, d;

                if (!(iss >> cs >> rs >> ns >> ws >> d)) {
                    std::cout << "[Ошибка ввода!] Нужно ввести 5 значений ЧЕРЕЗ ПРОБЕЛ. Пример: q0 1 q0 0 R\n";
                } else {
                    Direction dir = Direction::NONE;
                    if (d == 'L' || d == 'l') dir = Direction::LEFT;
                    else if (d == 'R' || d == 'r') dir = Direction::RIGHT;
                    else if (d == 'N' || d == 'n') dir = Direction::NONE;
                    else {
                        std::cout << "[Ошибка!] Направление должно быть L, R или N.\n";
                        continue;
                    }
                    tm.addRule(Rule(cs, rs, ns, ws, dir));
                    std::cout << "Правило успешно добавлено: (" << cs << ", " << rs << ") -> ("
                              << ns << ", " << ws << ", " << d << ")\n";
                }
            } else if (choice == 5) {
                std::cout << "Введите ЧЕРЕЗ ПРОБЕЛ тек_сост и чит_симв для удаления (например: q0 1): ";
                std::string line;
                std::getline(std::cin >> std::ws, line);
                std::istringstream iss(line);

                std::string cs;
                char rs;
                if (!(iss >> cs >> rs)) {
                    std::cout << "[Ошибка ввода!] Введите состояние и символ через пробел.\n";
                } else {
                    if (tm.removeRule(cs, rs)) std::cout << "Правило удалено.\n";
                    else std::cout << "Правило не найдено.\n";
                }
            } else if (choice == 6) {
                bool moved = tm.step(true);
                if (!moved) std::cout << "Машина остановлена (состояние: " << tm.getCurrentState() << ").\n";
            } else if (choice == 7) {
                size_t steps = tm.run(logMode);
                std::cout << "Выполнено шагов: " << steps << "\n";
                std::cout << "Итоговое содержимое ленты: " << tm.getTapeContent() << "\n";
            } else if (choice == 8) {
                logMode = !logMode;
            } else {
                std::cout << "Неизвестный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Исключение: " << e.what() << "\n";
        }
    }

    return 0;
}