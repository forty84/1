#include <gtest/gtest.h>
#include <sstream>
#include "TuringMachine.h"

// Тест динамического расширения ленты влево и вправо
TEST(TapeTest, ExpandLeftAndRight) {
    Tape tape('_');
    tape.write('A');
    ++tape;
    tape.write('B');
    --tape;
    --tape; // Выход за левую границу — должен создаться новый узел
    tape.write('Z');

    EXPECT_EQ(tape.toString(), "ZAB");
}

// Тест глубокого копирования ленты
TEST(TapeTest, DeepCopy) {
    Tape t1('_');
    t1.load("101");
    ++t1; // Каретка на '0'

    Tape t2 = t1; // Конструктор копирования
    t2.write('9');

    EXPECT_EQ(t1.toString(), "101");
    EXPECT_EQ(t2.toString(), "191");
}

// Тест управления правилами (добавление, перезапись, удаление)
TEST(MachineTest, AddAndRemoveRules) {
    TuringMachine tm;
    tm.addRule(Rule("q0", '0', "q1", '1', Direction::RIGHT));
    tm.addRule(Rule("q0", '1', "q1", '0', Direction::RIGHT));
    EXPECT_EQ(tm.getRulesCount(), 2);

    // Перезапись существующего правила не должна увеличивать счетчик
    tm.addRule(Rule("q0", '0', "q2", '1', Direction::LEFT));
    EXPECT_EQ(tm.getRulesCount(), 2);
    EXPECT_EQ(tm.findRule("q0", '0')->nextState, "q2");

    // Удаление правила
    EXPECT_TRUE(tm.removeRule("q0", '0'));
    EXPECT_EQ(tm.getRulesCount(), 1);
    EXPECT_EQ(tm.findRule("q0", '0'), nullptr);
}

// Тест полного цикла работы: инверсия битов и загрузка из потока
TEST(MachineTest, StreamLoadAndBitInversion) {
    std::stringstream ss(
        "10100 q0 qf\n"
        "q0 1 q0 0 R\n"
        "q0 0 q0 1 R\n"
        "q0 _ qf _ N\n"
    );

    TuringMachine tm;
    tm.loadFromStream(ss);

    std::ostringstream logStream;
    size_t steps = tm.run(true, 100, logStream);

    EXPECT_EQ(steps, 6);
    EXPECT_EQ(tm.getCurrentState(), "qf");
    EXPECT_EQ(tm.getTapeContent(), "01011");
    EXPECT_FALSE(logStream.str().empty()); // Лог записался
}

// Тест защиты от зацикливания
TEST(MachineTest, InfiniteLoopProtection) {
    TuringMachine tm;
    tm.loadTape("1");
    tm.setStates("q0", "qf");
    // Бесконечный шаг на месте
    tm.addRule(Rule("q0", '1', "q0", '1', Direction::NONE));

    EXPECT_THROW(tm.run(false, 50), std::runtime_error);
}