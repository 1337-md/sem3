/**
 * @file RealMatrix.h
 * @brief Заголовочный файл класса RealMatrix (Вещественная матрица).
 * @author Студент
 * @version 1.0
 */

#ifndef REAL_MATRIX_H
#define REAL_MATRIX_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>

/**
 * @class RealMatrix
 * @brief Класс для работы с вещественными матрицами произвольного размера.
 *
 * Обеспечивает хранение матрицы, динамическое изменение размеров,
 * извлечение подматриц, транспонирование, определение типов матрицы
 * и потоковый ввод-вывод.
 */
class RealMatrix {
private:
    size_t rows_;       ///< Количество строк
    size_t cols_;       ///< Количество столбцов
    double* data_;      ///< Одномерный массив элементов (размер rows_ * cols_)

    static constexpr double EPSILON = 1e-9; ///< Точность сравнения вещественных чисел

    /**
     * @brief Вспомогательная функция сравнения двух чисел double.
     */
    static bool areEqual(double a, double b) noexcept;

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустую матрицу 0x0.
     */
    RealMatrix() noexcept;

    /**
     * @brief Конструктор матрицы заданного размера.
     * @param rows Число строк.
     * @param cols Число столбцов.
     * @param initValue Начальное значение элементов (по умолчанию 0.0).
     * @throw std::invalid_argument Если только один из размеров равен 0.
     */
    RealMatrix(size_t rows, size_t cols, double initValue = 0.0);

    /**
     * @brief Конструктор копирования.
     * @param other Копируемая матрица.
     */
    RealMatrix(const RealMatrix& other);

    /**
     * @brief Конструктор перемещения.
     * @param other Перемещаемая матрица.
     */
    RealMatrix(RealMatrix&& other) noexcept;

    /**
     * @brief Деструктор. Освобождает выделенную динамическую память.
     */
    ~RealMatrix() noexcept;

    /**
     * @brief Оператор присваивания копированием.
     * @param other Копируемая матрица.
     * @return Ссылка на текущий объект.
     */
    RealMatrix& operator=(const RealMatrix& other);

    /**
     * @brief Оператор присваивания перемещением.
     * @param other Перемещаемая матрица.
     * @return Ссылка на текущий объект.
     */
    RealMatrix& operator=(RealMatrix&& other) noexcept;

    /**
     * @brief Получить количество строк.
     * @return Число строк матрицы.
     */
    size_t getRows() const noexcept;

    /**
     * @brief Получить количество столбцов.
     * @return Число столбцов матрицы.
     */
    size_t getCols() const noexcept;

    /**
     * @brief Доступ к элементу для чтения и записи.
     * @param row Индекс строки (с 0).
     * @param col Индекс столбца (с 0).
     * @return Ссылка на элемент матрицы.
     * @throw std::out_of_range Если индексы выходят за пределы матрицы.
     */
    double& at(size_t row, size_t col);

    /**
     * @brief Доступ к элементу только для чтения.
     * @param row Индекс строки (с 0).
     * @param col Индекс столбца (с 0).
     * @return Значение элемента матрицы.
     * @throw std::out_of_range Если индексы выходят за пределы матрицы.
     */
    double at(size_t row, size_t col) const;

    /**
     * @brief Изменение числа строк и столбцов матрицы с сохранением данных.
     * Новые элементы заполняются значением fillValue.
     * @param newRows Новое число строк.
     * @param newCols Новое число столбцов.
     * @param fillValue Значение для новых ячеек (по умолчанию 0.0).
     * @throw std::invalid_argument Если только один из новых размеров равен 0.
     */
    void resize(size_t newRows, size_t newCols, double fillValue = 0.0);

    /**
     * @brief Загрузка матрицы из текстового файла.
     * Формат файла: сначала два числа (rows cols), затем элементы матрицы.
     * @param filename Путь к файлу.
     * @throw std::runtime_error Если файл не открылся или данные некорректны.
     */
    void loadFromFile(const std::string& filename);

    /**
     * @brief Сохранение матрицы в текстовый файл.
     * @param filename Путь к файлу.
     * @throw std::runtime_error Если файл не удалось открыть для записи.
     */
    void saveToFile(const std::string& filename) const;

    /**
     * @brief Извлечение подматрицы заданного размера.
     * @param startRow Начальная строка.
     * @param startCol Начальный столбец.
     * @param subRows Количество строк подматрицы.
     * @param subCols Количество столбцов подматрицы.
     * @return Новый объект RealMatrix, содержащий подматрицу.
     * @throw std::out_of_range Если подматрица выходит за границы исходной.
     */
    RealMatrix getSubmatrix(size_t startRow, size_t startCol, size_t subRows, size_t subCols) const;

    /**
     * @brief Возвращает транспонированную матрицу.
     * @return Новая транспонированная матрица.
     */
    RealMatrix transpose() const;

    /** @brief Проверка: является ли матрица квадратной (непустой и rows == cols). */
    bool isSquare() const noexcept;

    /** @brief Проверка: является ли матрица нулевой (все элементы == 0). */
    bool isZero() const noexcept;

    /** @brief Проверка: является ли матрица диагональной. */
    bool isDiagonal() const noexcept;

    /** @brief Проверка: является ли матрица единичной. */
    bool isIdentity() const noexcept;

    /** @brief Проверка: является ли матрица симметрической ($A_{ij} = A_{ji}$). */
    bool isSymmetric() const noexcept;

    /** @brief Проверка: является ли матрица верхней треугольной (все элементы ниже главной диагонали == 0). */
    bool isUpperTriangular() const noexcept;

    /** @brief Проверка: является ли матрица нижней треугольной (все элементы выше главной диагонали == 0). */
    bool isLowerTriangular() const noexcept;

    /**
     * @brief Оператор сравнения на равенство.
     */
    bool operator==(const RealMatrix& other) const noexcept;

    /**
     * @brief Оператор сравнения на неравенство.
     */
    bool operator!=(const RealMatrix& other) const noexcept;

    /**
     * @brief Оператор вывода матрицы в поток.
     * Первая строка: "rows cols", далее строки матрицы.
     */
    friend std::ostream& operator<<(std::ostream& os, const RealMatrix& matrix);

    /**
     * @brief Оператор чтения матрицы из потока.
     */
    friend std::istream& operator>>(std::istream& is, RealMatrix& matrix);
};

#endif // REAL_MATRIX_H