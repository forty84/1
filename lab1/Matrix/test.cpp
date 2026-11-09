/**
 * @file test.cpp
 * @brief Модульные тесты для класса Matrix (GoogleTest).
 */

#include <gtest/gtest.h>
#include "Matrix.h"

// Проверка конструкторов и базовых геттеров
TEST(MatrixTest, ConstructorsAndGetters) {
    Matrix m1;
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
    EXPECT_TRUE(m1.isNull());

    Matrix m2(3, 4);
    EXPECT_EQ(m2.getRows(), 3);
    EXPECT_EQ(m2.getCols(), 4);
    EXPECT_DOUBLE_EQ(m2.getValue(2, 3), 0.0);
}

// Проверка правила трех (копирование и присваивание)
TEST(MatrixTest, RuleOfThree) {
    Matrix m1(2, 2);
    m1.setValue(0, 0, 1.5);
    m1.setValue(1, 1, 2.5);

    Matrix m2 = m1; // Конструктор копирования
    EXPECT_DOUBLE_EQ(m2.getValue(0, 0), 1.5);
    EXPECT_DOUBLE_EQ(m2.getValue(1, 1), 2.5);
    EXPECT_TRUE(m1 == m2);

    Matrix m3;
    m3 = m1; // Оператор присваивания
    EXPECT_DOUBLE_EQ(m3.getValue(1, 1), 2.5);
    EXPECT_TRUE(m1 == m3);
}

// Проверка изменения размера и исключений при выходе за границы
TEST(MatrixTest, ResizeAndBounds) {
    Matrix m(2, 2);
    m.setValue(0, 0, 5.0);
    m.resize(3, 3);
    
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);
    EXPECT_DOUBLE_EQ(m.getValue(0, 0), 5.0);
    EXPECT_DOUBLE_EQ(m.getValue(2, 2), 0.0);
    
    EXPECT_THROW(m.getValue(3, 3), std::out_of_range);
    EXPECT_THROW(m.setValue(5, 5, 1.0), std::out_of_range);
}

// Проверка логических свойств матрицы
TEST(MatrixTest, MatrixProperties) {
    Matrix m(3, 3);
    EXPECT_TRUE(m.isSquare());
    EXPECT_TRUE(m.isNull());

    for (size_t i = 0; i < 3; ++i) m.setValue(i, i, 1.0);
    EXPECT_TRUE(m.isIdentity());
    EXPECT_TRUE(m.isDiagonal());
    EXPECT_TRUE(m.isSymmetric());

    m.setValue(0, 2, 4.0);
    EXPECT_FALSE(m.isIdentity());
    EXPECT_FALSE(m.isSymmetric());
    EXPECT_TRUE(m.isUpperTriangular());
}

// Проверка транспонирования
TEST(MatrixTest, Transpose) {
    Matrix m(2, 3);
    m.setValue(0, 1, 5.0);
    m.setValue(1, 2, 7.0);

    Matrix t = m.transpose();
    EXPECT_EQ(t.getRows(), 3);
    EXPECT_EQ(t.getCols(), 2);
    EXPECT_DOUBLE_EQ(t.getValue(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(t.getValue(2, 1), 7.0);
}

// Проверка инкрементов и декрементов
TEST(MatrixTest, IncrementDecrement) {
    Matrix m(2, 2);
    ++m;
    EXPECT_DOUBLE_EQ(m.getValue(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.getValue(1, 1), 1.0);
    
    Matrix temp = m++;
    EXPECT_DOUBLE_EQ(temp.getValue(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.getValue(0, 0), 2.0);

    --m;
    EXPECT_DOUBLE_EQ(m.getValue(0, 0), 1.0);
}