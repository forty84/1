/**
 * @file Matrix.cpp
 * @brief Реализация методов класса Matrix.
 */

#include "Matrix.h"
#include <fstream>
#include <stdexcept>
#include <cmath>

const double EPSILON = 1e-9;


// 1. Управление памятью

void Matrix::allocateMemory() {
    if (rows == 0 || cols == 0) {
        data = nullptr;
        return;
    }
    data = new double*[rows];
    for (size_t i = 0; i < rows; ++i) {
        data[i] = new double[cols]{0.0};
    }
}

void Matrix::freeMemory() {
    if (data != nullptr) {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }
}

Matrix::Matrix() : data(nullptr), rows(0), cols(0) {}

Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c) {
    allocateMemory();
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    allocateMemory();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) { // Защита от самоприсваивания (m = m)
        freeMemory();
        rows = other.rows;
        cols = other.cols;
        allocateMemory();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}

Matrix::~Matrix() {
    freeMemory();
}


// 2. Базовый доступ к данным


size_t Matrix::getRows() const { return rows; }
size_t Matrix::getCols() const { return cols; }

double Matrix::getValue(size_t r, size_t c) const {
    if (r >= rows || c >= cols) throw std::out_of_range("Индекс вне диапазона");
    return data[r][c];
}

void Matrix::setValue(size_t r, size_t c, double val) {
    if (r >= rows || c >= cols) throw std::out_of_range("Индекс вне диапазона");
    data[r][c] = val;
}


// 3. Требования варианта 1.7.1


void Matrix::resize(size_t newRows, size_t newCols) {
    Matrix temp(newRows, newCols);
    size_t minRows = (rows < newRows) ? rows : newRows;
    size_t minCols = (cols < newCols) ? cols : newCols;

    for (size_t i = 0; i < minRows; ++i) {
        for (size_t j = 0; j < minCols; ++j) {
            temp.data[i][j] = data[i][j];
        }
    }
    *this = temp; // Используем перегруженный оператор присваивания
}

void Matrix::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Не удалось открыть файл");
    
    size_t r, c;
    if (file >> r >> c) {
        resize(r, c);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                file >> data[i][j];
            }
        }
    }
    file.close();
}

Matrix Matrix::getSubmatrix(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const {
    if (startRow + numRows > rows || startCol + numCols > cols) {
        throw std::out_of_range("Подматрица выходит за границы матрицы");
    }
    Matrix sub(numRows, numCols);
    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < numCols; ++j) {
            sub.data[i][j] = data[startRow + i][startCol + j];
        }
    }
    return sub;
}

Matrix Matrix::transpose() const {
    Matrix transposed(cols, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            transposed.data[j][i] = data[i][j];
        }
    }
    return transposed;
}

bool Matrix::isSquare() const {
    return rows == cols && rows > 0;
}

bool Matrix::isDiagonal() const {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (i != j && std::abs(data[i][j]) > EPSILON) return false;
        }
    }
    return true;
}

bool Matrix::isNull() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (std::abs(data[i][j]) > EPSILON) return false;
        }
    }
    return true;
}

bool Matrix::isIdentity() const {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (i == j) {
                if (std::abs(data[i][j] - 1.0) > EPSILON) return false;
            } else {
                if (std::abs(data[i][j]) > EPSILON) return false;
            }
        }
    }
    return true;
}

bool Matrix::isSymmetric() const {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = i + 1; j < cols; ++j) {
            if (std::abs(data[i][j] - data[j][i]) > EPSILON) return false;
        }
    }
    return true;
}

bool Matrix::isUpperTriangular() const {
    if (!isSquare()) return false;
    for (size_t i = 1; i < rows; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (std::abs(data[i][j]) > EPSILON) return false;
        }
    }
    return true;
}

bool Matrix::isLowerTriangular() const {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = i + 1; j < cols; ++j) {
            if (std::abs(data[i][j]) > EPSILON) return false;
        }
    }
    return true;
}


// Перегрузка операторов

bool Matrix::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (std::abs(data[i][j] - other.data[i][j]) > EPSILON) return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const {
    return !(*this == other);
}

Matrix& Matrix::operator++() {
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            data[i][j] += 1.0;
    return *this;
}

Matrix Matrix::operator++(int) {
    Matrix temp = *this;
    ++(*this);
    return temp;
}

Matrix& Matrix::operator--() {
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            data[i][j] -= 1.0;
    return *this;
}

Matrix Matrix::operator--(int) {
    Matrix temp = *this;
    --(*this);
    return temp;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            os << m.data[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& m) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            is >> m.data[i][j];
        }
    }
    return is;
}