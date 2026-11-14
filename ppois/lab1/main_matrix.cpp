#include "RealMatrix.h"
#include <iostream>
#include <windows.h>

void printTypes(const RealMatrix& m) {
    std::cout << "Характеристики матрицы:\n"
              << " - Квадратная:          " << (m.isSquare() ? "Да" : "Нет") << "\n"
              << " - Нулевая:             " << (m.isZero() ? "Да" : "Нет") << "\n"
              << " - Диагональная:        " << (m.isDiagonal() ? "Да" : "Нет") << "\n"
              << " - Единичная:           " << (m.isIdentity() ? "Да" : "Нет") << "\n"
              << " - Симметрическая:      " << (m.isSymmetric() ? "Да" : "Нет") << "\n"
              << " - Верхняя треугольная: " << (m.isUpperTriangular() ? "Да" : "Нет") << "\n"
              << " - Нижняя треугольная:  " << (m.isLowerTriangular() ? "Да" : "Нет") << "\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    RealMatrix matrix;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n=== МЕНЮ: ВЕЩЕСТВЕННАЯ МАТРИЦА ===\n"
                  << "1. Ввести матрицу с клавиатуры (формат: rows cols и элементы)\n"
                  << "2. Вывести матрицу на экран\n"
                  << "3. Изменить размер (resize)\n"
                  << "4. Загрузить из файла\n"
                  << "5. Сохранить в файл\n"
                  << "6. Извлечь подматрицу\n"
                  << "7. Транспонировать матрицу\n"
                  << "8. Проверить тип матрицы\n"
                  << "0. Выход\n"
                  << "Выбор: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        try {
            switch (choice) {
                case 1:
                    std::cout << "Введите число строк, столбцов и сами элементы:\n";
                    if (!(std::cin >> matrix)) {
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                        std::cout << "Ошибка ввода!\n";
                    }
                    break;
                case 2:
                    std::cout << "Текущая матрица:\n" << matrix;
                    break;
                case 3: {
                    size_t r, c;
                    double val;
                    std::cout << "Новые строки, столбцы и значение заполнения: ";
                    std::cin >> r >> c >> val;
                    matrix.resize(r, c, val);
                    break;
                }
                case 4: {
                    std::string fname;
                    std::cout << "Имя файла: ";
                    std::cin >> fname;
                    matrix.loadFromFile(fname);
                    std::cout << "Успешно загружено.\n";
                    break;
                }
                case 5: {
                    std::string fname;
                    std::cout << "Имя файла: ";
                    std::cin >> fname;
                    matrix.saveToFile(fname);
                    std::cout << "Успешно сохранено.\n";
                    break;
                }
                case 6: {
                    size_t sr, sc, r, c;
                    std::cout << "Введите startRow, startCol, subRows, subCols: ";
                    std::cin >> sr >> sc >> r >> c;
                    RealMatrix sub = matrix.getSubmatrix(sr, sc, r, c);
                    std::cout << "Подматрица:\n" << sub;
                    break;
                }
                case 7:
                    matrix = matrix.transpose();
                    std::cout << "Матрица транспонирована.\n";
                    break;
                case 8:
                    printTypes(matrix);
                    break;
                case 0:
                    break;
                default:
                    std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Исключение: " << e.what() << "\n";
        }
    }
    return 0;
}