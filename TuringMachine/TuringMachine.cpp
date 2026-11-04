/**
 * @file TuringMachine.cpp
 * @brief Реализация методов классов Rule, Tape и TuringMachine.
 */

#include "TuringMachine.h"

// =========================================================================
// Реализация класса Rule
// =========================================================================

Rule::Rule()
    : currentState(""), readSymbol('_'), nextState(""), writeSymbol('_'), moveDir(Direction::NONE) {}

Rule::Rule(const std::string& currSt, char readSym,
           const std::string& nextSt, char writeSym, Direction dir)
    : currentState(currSt), readSymbol(readSym),
      nextState(nextSt), writeSymbol(writeSym), moveDir(dir) {}

bool Rule::matches(const std::string& state, char sym) const {
    return currentState == state && readSymbol == sym;
}

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
    char dirChar = 'N';
    if (rule.moveDir == Direction::LEFT) dirChar = 'L';
    else if (rule.moveDir == Direction::RIGHT) dirChar = 'R';

    os << "(" << rule.currentState << ", " << rule.readSymbol << ") -> ("
       << rule.nextState << ", " << rule.writeSymbol << ", " << dirChar << ")";
    return os;
}

// =========================================================================
// Реализация класса Tape
// =========================================================================

Tape::Tape(char blank) : blankSymbol(blank) {
    head = new Node(blankSymbol);
    tail = head;
    carriage = head;
}

Tape::Tape(const Tape& other) : head(nullptr), tail(nullptr), carriage(nullptr), blankSymbol(other.blankSymbol) {
    copyFrom(other);
}

Tape& Tape::operator=(const Tape& other) {
    if (this != &other) {
        clear();
        blankSymbol = other.blankSymbol;
        copyFrom(other);
    }
    return *this;
}

Tape::~Tape() {
    clear();
}

void Tape::clear() {
    Node* curr = head;
    while (curr != nullptr) {
        Node* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    carriage = nullptr;
}

void Tape::copyFrom(const Tape& other) {
    if (other.head == nullptr) {
        head = new Node(blankSymbol);
        tail = head;
        carriage = head;
        return;
    }

    Node* otherCurr = other.head;
    while (otherCurr != nullptr) {
        Node* newNode = new Node(otherCurr->symbol);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        // Запоминаем позицию каретки в новой копии списка
        if (otherCurr == other.carriage) {
            carriage = newNode;
        }
        otherCurr = otherCurr->next;
    }
}

char Tape::read() const {
    return carriage->symbol;
}

void Tape::write(char sym) {
    carriage->symbol = sym;
}

Tape& Tape::operator++() {
    if (carriage->next == nullptr) {
        Node* newNode = new Node(blankSymbol);
        carriage->next = newNode;
        newNode->prev = carriage;
        tail = newNode;
    }
    carriage = carriage->next;
    return *this;
}

Tape Tape::operator++(int) {
    Tape temp(*this);
    ++(*this);
    return temp;
}

Tape& Tape::operator--() {
    if (carriage->prev == nullptr) {
        Node* newNode = new Node(blankSymbol);
        carriage->prev = newNode;
        newNode->next = carriage;
        head = newNode;
    }
    carriage = carriage->prev;
    return *this;
}

Tape Tape::operator--(int) {
    Tape temp(*this);
    --(*this);
    return temp;
}

void Tape::load(const std::string& data) {
    clear();
    if (data.empty()) {
        head = new Node(blankSymbol);
        tail = head;
        carriage = head;
        return;
    }

    for (size_t i = 0; i < data.length(); ++i) {
        Node* newNode = new Node(data[i]);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    carriage = head; // Ставим каретку на первый символ
}

std::string Tape::toString() const {
    // Отбрасываем пустые символы по краям для чистого представления результата
    Node* left = head;
    while (left != nullptr && left->symbol == blankSymbol) {
        left = left->next;
    }
    if (left == nullptr) return ""; // Вся лента пустая

    Node* right = tail;
    while (right != nullptr && right->symbol == blankSymbol) {
        right = right->prev;
    }

    std::string result;
    Node* curr = left;
    while (curr != nullptr) {
        result += curr->symbol;
        if (curr == right) break;
        curr = curr->next;
    }
    return result;
}

void Tape::printWithCarriage(std::ostream& os) const {
    Node* curr = head;
    while (curr != nullptr) {
        if (curr == carriage) {
            os << "[" << curr->symbol << "]";
        } else {
            os << " " << curr->symbol << " ";
        }
        curr = curr->next;
    }
}

// =========================================================================
// Реализация класса TuringMachine
// =========================================================================

TuringMachine::TuringMachine(char blank)
    : tape(blank), rules(nullptr), rulesCount(0), rulesCapacity(0),
      currentState("q0"), haltState("qf") {}

TuringMachine::TuringMachine(const TuringMachine& other)
    : tape(other.tape), rules(nullptr), rulesCount(other.rulesCount),
      rulesCapacity(other.rulesCapacity), currentState(other.currentState),
      haltState(other.haltState) {
    if (rulesCapacity > 0) {
        rules = new Rule[rulesCapacity];
        for (size_t i = 0; i < rulesCount; ++i) {
            rules[i] = other.rules[i];
        }
    }
}

TuringMachine& TuringMachine::operator=(const TuringMachine& other) {
    if (this != &other) {
        delete[] rules;
        tape = other.tape;
        rulesCount = other.rulesCount;
        rulesCapacity = other.rulesCapacity;
        currentState = other.currentState;
        haltState = other.haltState;

        if (rulesCapacity > 0) {
            rules = new Rule[rulesCapacity];
            for (size_t i = 0; i < rulesCount; ++i) {
                rules[i] = other.rules[i];
            }
        } else {
            rules = nullptr;
        }
    }
    return *this;
}

TuringMachine::~TuringMachine() {
    delete[] rules;
}

void TuringMachine::resizeRules(size_t newCapacity) {
    Rule* newArray = new Rule[newCapacity];
    for (size_t i = 0; i < rulesCount; ++i) {
        newArray[i] = rules[i];
    }
    delete[] rules;
    rules = newArray;
    rulesCapacity = newCapacity;
}

void TuringMachine::setStates(const std::string& startState, const std::string& finalState) {
    currentState = startState;
    haltState = finalState;
}

std::string TuringMachine::getCurrentState() const {
    return currentState;
}

std::string TuringMachine::getHaltState() const {
    return haltState;
}

void TuringMachine::loadTape(const std::string& initialData) {
    tape.load(initialData);
}

std::string TuringMachine::getTapeContent() const {
    return tape.toString();
}

Tape& TuringMachine::getTape() {
    return tape;
}

void TuringMachine::addRule(const Rule& rule) {
    // Если правило для такой пары (состояние, символ) уже есть — перезаписываем его
    for (size_t i = 0; i < rulesCount; ++i) {
        if (rules[i].matches(rule.currentState, rule.readSymbol)) {
            rules[i] = rule;
            return;
        }
    }
    if (rulesCount == rulesCapacity) {
        size_t nextCap = (rulesCapacity == 0) ? 4 : rulesCapacity * 2;
        resizeRules(nextCap);
    }
    rules[rulesCount++] = rule;
}

bool TuringMachine::removeRule(const std::string& currSt, char readSym) {
    for (size_t i = 0; i < rulesCount; ++i) {
        if (rules[i].matches(currSt, readSym)) {
            // Сдвигаем оставшиеся элементы влево, чтобы закрыть "дырку"
            for (size_t j = i; j + 1 < rulesCount; ++j) {
                rules[j] = rules[j + 1];
            }
            --rulesCount;
            return true;
        }
    }
    return false;
}

const Rule* TuringMachine::findRule(const std::string& state, char sym) const {
    for (size_t i = 0; i < rulesCount; ++i) {
        if (rules[i].matches(state, sym)) {
            return &rules[i];
        }
    }
    return nullptr;
}

size_t TuringMachine::getRulesCount() const {
    return rulesCount;
}

void TuringMachine::loadFromStream(std::istream& is) {
    std::string initTape, startSt, endSt;
    if (!(is >> initTape >> startSt >> endSt)) {
        throw std::runtime_error("Ошибка формата: не удалось прочитать шапку программы");
    }

    if (initTape == "_") initTape = "";
    loadTape(initTape);
    setStates(startSt, endSt);

    // Очищаем старые правила перед загрузкой новых
    rulesCount = 0;

    std::string cState, nState;
    char rSym, wSym, dirChar;
    while (is >> cState >> rSym >> nState >> wSym >> dirChar) {
        Direction dir = Direction::NONE;
        if (dirChar == 'L' || dirChar == 'l') dir = Direction::LEFT;
        else if (dirChar == 'R' || dirChar == 'r') dir = Direction::RIGHT;
        else if (dirChar == 'N' || dirChar == 'n') dir = Direction::NONE;
        else throw std::invalid_argument("Неизвестное направление сдвига: " + std::string(1, dirChar));

        addRule(Rule(cState, rSym, nState, wSym, dir));
    }
}

bool TuringMachine::step(bool log, std::ostream& os) {
    if (currentState == haltState) {
        return false;
    }

    char sym = tape.read();
    const Rule* rule = findRule(currentState, sym);

    if (rule == nullptr) {
        if (log) {
            os << "[СТОП] Нет правила для состояния '" << currentState
               << "' и символа '" << sym << "'\n";
        }
        return false;
    }

    if (log) {
        os << "Состояние: " << currentState << " | Лента: ";
        tape.printWithCarriage(os);
        os << " | Правило: " << *rule << "\n";
    }

    // Применяем правило
    tape.write(rule->writeSymbol);
    currentState = rule->nextState;

    if (rule->moveDir == Direction::RIGHT) {
        ++tape;
    } else if (rule->moveDir == Direction::LEFT) {
        --tape;
    }

    return true;
}

size_t TuringMachine::run(bool log, size_t maxSteps, std::ostream& os) {
    size_t steps = 0;
    if (log) {
        os << "=== ЗАПУСК МАШИНЫ ТЬЮРИНГА ===\n";
    }

    while (steps < maxSteps && currentState != haltState) {
        if (!step(log, os)) {
            break;
        }
        ++steps;
    }

    if (steps >= maxSteps && currentState != haltState) {
        throw std::runtime_error("Превышен лимит шагов (возможно зацикливание)");
    }

    if (log) {
        os << "=== ФИНАЛ (шагов: " << steps << ", состояние: " << currentState << ") ===\n";
        os << "Итоговая лента: ";
        tape.printWithCarriage(os);
        os << "\n";
    }

    return steps;
}