/**
 * @file Matrix.h
 * @brief Заголовочный файл класса вещественной матрицы.
 * Содержит объявление полей и методов класса.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>

/**
 * @class Matrix
 * @brief Класс "Вещественная матрица" с ручным управлением памятью (без std::vector).
 */

class Matrix {
private:
    double** data;    ///< Указатель на двумерный массив элементов типа double
    size_t rows;      ///< Количество строк
    size_t cols;      ///< Количество столбцов

    /**
     * @brief Вспомогательный метод для выделения памяти.
     */
    void allocateMemory();

    /**
     * @brief Вспомогательный метод для освобождения памяти.
     */
    void freeMemory();

public:
     
    // 1. Управление памятью (Правило трёх)
     
    
    Matrix();                                     ///< Конструктор по умолчанию (пустая матрица)
    Matrix(size_t r, size_t c);                   ///< Конструктор с заданными размерами
    Matrix(const Matrix& other);                  ///< Конструктор копирования
    Matrix& operator=(const Matrix& other);       ///< Оператор присваивания копированием
    ~Matrix();                                    ///< Деструктор

     
    // 2. Базовый доступ к данным
     
    
    size_t getRows() const;
    size_t getCols() const;
    double getValue(size_t r, size_t c) const;
    void setValue(size_t r, size_t c, double val);

     
    // 3. Требования варианта 1.7.1
     
    
    void resize(size_t newRows, size_t newCols);
    void loadFromFile(const std::string& filename);
    Matrix getSubmatrix(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const;
    Matrix transpose() const;

    bool isSquare() const;
    bool isDiagonal() const;
    bool isNull() const;
    bool isIdentity() const;
    bool isSymmetric() const;
    bool isUpperTriangular() const;
    bool isLowerTriangular() const;

     
    // 4. Перегрузка операторов (Требования п. 1.7 и общие)
     
    
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;

    Matrix& operator++();       ///< Преинкремент (++m)
    Matrix operator++(int);     ///< Постинкремент (m++)
    Matrix& operator--();       ///< Предекремент (--m)
    Matrix operator--(int);     ///< Постдекремент (m--)

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
    friend std::istream& operator>>(std::istream& is, Matrix& m);
};

#endif // MATRIX_H
