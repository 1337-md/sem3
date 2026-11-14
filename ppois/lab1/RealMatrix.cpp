/**
 * @file RealMatrix.cpp
 * @brief Реализация методов класса RealMatrix.
 */

#include "RealMatrix.h"
#include <fstream>
#include <algorithm>
#include <utility>

bool RealMatrix::areEqual(double a, double b) noexcept {
    return std::fabs(a - b) <= EPSILON;
}

RealMatrix::RealMatrix() noexcept : rows_(0), cols_(0), data_(nullptr) {}

RealMatrix::RealMatrix(size_t rows, size_t cols, double initValue) : rows_(0), cols_(0), data_(nullptr) {
    if ((rows == 0 && cols != 0) || (rows != 0 && cols == 0)) {
        throw std::invalid_argument("Оба размера должны быть либо положительными, либо нулевыми.");
    }
    if (rows > 0 && cols > 0) {
        rows_ = rows;
        cols_ = cols;
        data_ = new double[rows_ * cols_];
        std::fill(data_, data_ + rows_ * cols_, initValue);
    }
}

RealMatrix::RealMatrix(const RealMatrix& other)
    : rows_(other.rows_), cols_(other.cols_), data_(nullptr) {
    if (rows_ > 0 && cols_ > 0) {
        data_ = new double[rows_ * cols_];
        std::copy(other.data_, other.data_ + rows_ * cols_, data_);
    }
}

RealMatrix::RealMatrix(RealMatrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
}

RealMatrix::~RealMatrix() noexcept {
    delete[] data_;
}

RealMatrix& RealMatrix::operator=(const RealMatrix& other) {
    if (this != &other) {
        RealMatrix temp(other);
        std::swap(rows_, temp.rows_);
        std::swap(cols_, temp.cols_);
        std::swap(data_, temp.data_);
    }
    return *this;
}

RealMatrix& RealMatrix::operator=(RealMatrix&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = other.data_;
        other.rows_ = 0;
        other.cols_ = 0;
        other.data_ = nullptr;
    }
    return *this;
}

size_t RealMatrix::getRows() const noexcept { return rows_; }
size_t RealMatrix::getCols() const noexcept { return cols_; }

double& RealMatrix::at(size_t row, size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Индекс выходит за пределы матрицы.");
    }
    return data_[row * cols_ + col];
}

double RealMatrix::at(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("Индекс выходит за пределы матрицы.");
    }
    return data_[row * cols_ + col];
}

void RealMatrix::resize(size_t newRows, size_t newCols, double fillValue) {
    if ((newRows == 0 && newCols != 0) || (newRows != 0 && newCols == 0)) {
        throw std::invalid_argument("Некорректные размеры при изменении матрицы.");
    }
    if (newRows == rows_ && newCols == cols_) {
        return;
    }
    if (newRows == 0 && newCols == 0) {
        delete[] data_;
        data_ = nullptr;
        rows_ = 0;
        cols_ = 0;
        return;
    }

    RealMatrix newMatrix(newRows, newCols, fillValue);
    size_t minRows = std::min(rows_, newRows);
    size_t minCols = std::min(cols_, newCols);

    for (size_t i = 0; i < minRows; ++i) {
        for (size_t j = 0; j < minCols; ++j) {
            newMatrix.at(i, j) = this->at(i, j);
        }
    }
    *this = std::move(newMatrix);
}

void RealMatrix::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
    }
    file >> *this;
    if (file.fail()) {
        throw std::runtime_error("Ошибка формата данных при чтении матрицы из файла.");
    }
}

void RealMatrix::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    file << *this;
}

RealMatrix RealMatrix::getSubmatrix(size_t startRow, size_t startCol, size_t subRows, size_t subCols) const {
    if (subRows == 0 && subCols == 0) {
        return RealMatrix();
    }
    if (startRow + subRows > rows_ || startCol + subCols > cols_) {
        throw std::out_of_range("Запрошенная подматрица выходит за границы исходной матрицы.");
    }

    RealMatrix sub(subRows, subCols);
    for (size_t i = 0; i < subRows; ++i) {
        for (size_t j = 0; j < subCols; ++j) {
            sub.at(i, j) = this->at(startRow + i, startCol + j);
        }
    }
    return sub;
}

RealMatrix RealMatrix::transpose() const {
    if (rows_ == 0 || cols_ == 0) {
        return RealMatrix();
    }
    RealMatrix result(cols_, rows_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            result.at(j, i) = this->at(i, j);
        }
    }
    return result;
}

bool RealMatrix::isSquare() const noexcept {
    return rows_ > 0 && rows_ == cols_;
}

bool RealMatrix::isZero() const noexcept {
    if (rows_ == 0) return false;
    for (size_t i = 0; i < rows_ * cols_; ++i) {
        if (!areEqual(data_[i], 0.0)) return false;
    }
    return true;
}

bool RealMatrix::isDiagonal() const noexcept {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            if (i != j && !areEqual(at(i, j), 0.0)) {
                return false;
            }
        }
    }
    return true;
}

bool RealMatrix::isIdentity() const noexcept {
    if (!isDiagonal()) return false;
    for (size_t i = 0; i < rows_; ++i) {
        if (!areEqual(at(i, i), 1.0)) {
            return false;
        }
    }
    return true;
}

bool RealMatrix::isSymmetric() const noexcept {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = i + 1; j < cols_; ++j) {
            if (!areEqual(at(i, j), at(j, i))) {
                return false;
            }
        }
    }
    return true;
}

bool RealMatrix::isUpperTriangular() const noexcept {
    if (!isSquare()) return false;
    for (size_t i = 1; i < rows_; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (!areEqual(at(i, j), 0.0)) {
                return false;
            }
        }
    }
    return true;
}

bool RealMatrix::isLowerTriangular() const noexcept {
    if (!isSquare()) return false;
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = i + 1; j < cols_; ++j) {
            if (!areEqual(at(i, j), 0.0)) {
                return false;
            }
        }
    }
    return true;
}

bool RealMatrix::operator==(const RealMatrix& other) const noexcept {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    for (size_t i = 0; i < rows_ * cols_; ++i) {
        if (!areEqual(data_[i], other.data_[i])) {
            return false;
        }
    }
    return true;
}

bool RealMatrix::operator!=(const RealMatrix& other) const noexcept {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const RealMatrix& matrix) {
    os << matrix.rows_ << " " << matrix.cols_ << "\n";
    for (size_t i = 0; i < matrix.rows_; ++i) {
        for (size_t j = 0; j < matrix.cols_; ++j) {
            os << matrix.at(i, j) << (j + 1 < matrix.cols_ ? " " : "");
        }
        os << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream& is, RealMatrix& matrix) {
    size_t r = 0, c = 0;
    if (!(is >> r >> c)) {
        return is;
    }
    if ((r == 0 && c != 0) || (r != 0 && c == 0)) {
        is.setstate(std::ios::failbit);
        return is;
    }
    RealMatrix temp(r, c);
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < c; ++j) {
            if (!(is >> temp.at(i, j))) {
                return is;
            }
        }
    }
    matrix = std::move(temp);
    return is;
}