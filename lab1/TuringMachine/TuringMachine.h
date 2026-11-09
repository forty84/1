/**
 * @file TuringMachine.h
 * @brief Объявление классов для моделирования Машины Тьюринга.
 */

#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @brief Направление сдвига каретки на ленте.
 */
enum class Direction {
    LEFT,   ///< Сдвиг влево ('L')
    RIGHT,  ///< Сдвиг вправо ('R')
    NONE    ///< Остаться на месте ('N')
};

/**
 * @class Rule
 * @brief Класс правила перехода автомата.
 */
class Rule {
public:
    std::string currentState; ///< Исходное состояние
    char readSymbol;          ///< Ожидаемый символ на ленте
    std::string nextState;    ///< Новое состояние
    char writeSymbol;         ///< Символ для записи на ленту
    Direction moveDir;        ///< Направление движения каретки

    Rule();
    Rule(const std::string& currSt, char readSym,
         const std::string& nextSt, char writeSym, Direction dir);

    bool matches(const std::string& state, char sym) const;

    friend std::ostream& operator<<(std::ostream& os, const Rule& rule);
};

/**
 * @class Tape
 * @brief Бесконечная лента Машины Тьюринга на базе двусвязного списка.
 */
class Tape {
private:
    struct Node {
        char symbol;
        Node* prev;
        Node* next;
        Node(char s) : symbol(s), prev(nullptr), next(nullptr) {}
    };

    Node* head;       ///< Самая левая созданная ячейка
    Node* tail;       ///< Самая правая созданная ячейка
    Node* carriage;   ///< Указатель каретки на текущую ячейку
    char blankSymbol; ///< Символ пустой ячейки (по умолчанию '_')

    void clear();
    void copyFrom(const Tape& other);

public:
    explicit Tape(char blank = '_');
    Tape(const Tape& other);
    Tape& operator=(const Tape& other);
    ~Tape();

    char read() const;
    void write(char sym);

    // Операторы сдвига каретки (префиксные и постфиксные)
    Tape& operator++();    ///< Сдвиг каретки вправо (++tape)
    Tape operator++(int);  ///< Сдвиг каретки вправо (tape++)
    Tape& operator--();    ///< Сдвиг каретки влево (--tape)
    Tape operator--(int);  ///< Сдвиг каретки влево (tape--)

    void load(const std::string& data);
    std::string toString() const;
    void printWithCarriage(std::ostream& os = std::cout) const;
};

/**
 * @class TuringMachine
 * @brief Интерпретатор Машины Тьюринга, объединяющий ленту и массив правил.
 */
class TuringMachine {
private:
    Tape tape;
    Rule* rules;          ///< Динамический массив правил
    size_t rulesCount;    ///< Текущее количество правил
    size_t rulesCapacity; ///< Вместимость выделенного массива

    std::string currentState;
    std::string haltState;

    void resizeRules(size_t newCapacity);

public:
    explicit TuringMachine(char blank = '_');
    TuringMachine(const TuringMachine& other);
    TuringMachine& operator=(const TuringMachine& other);
    ~TuringMachine();

    void setStates(const std::string& startState, const std::string& finalState);
    std::string getCurrentState() const;
    std::string getHaltState() const;

    void loadTape(const std::string& initialData);
    std::string getTapeContent() const;
    Tape& getTape();

    void addRule(const Rule& rule);
    bool removeRule(const std::string& currSt, char readSym);
    const Rule* findRule(const std::string& state, char sym) const;
    size_t getRulesCount() const;

    /**
     * @brief Загружает конфигурацию машины из потока (файла или строки).
     */
    void loadFromStream(std::istream& is);

    /**
     * @brief Выполняет ровно один такт работы машины.
     * @return true, если правило применено; false, если машина остановилась.
     */
    bool step(bool log = false, std::ostream& os = std::cout);

    /**
     * @brief Запускает цикл выполнения до остановки или превышения лимита шагов.
     * @return Общее число выполненных шагов.
     */
    size_t run(bool log = false, size_t maxSteps = 10000, std::ostream& os = std::cout);
};

#endif // TURING_MACHINE_H