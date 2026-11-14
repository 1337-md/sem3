#include "Multiset.h"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Multiset ms;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n=== МЕНЮ: НЕОРИЕНТИРОВАННОЕ МУЛЬТИМНОЖЕСТВО ===\n"
                  << "Текущее мультимножество: " << ms << " (мощность: " << ms.size() << ")\n"
                  << "1. Сформировать из строки (например: {a, a, c, {a, b, b}, {}, {a, {c, c}}})\n"
                  << "2. Добавить атомарный элемент\n"
                  << "3. Добавить вложенное мультимножество\n"
                  << "4. Удалить атомарный элемент\n"
                  << "5. Узнать кратность элемента\n"
                  << "6. Сравнить с другим мультимножеством (==)\n"
                  << "7. Объединение / Пересечение / Разность с другим мультимножеством\n"
                  << "8. Очистить\n"
                  << "0. Выход\n"
                  << "Выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n'); // Очистка буфера перевода строки

        try {
            switch (choice) {
                case 1: {
                    std::cout << "Введите строку мультимножества: ";
                    std::string line;
                    std::getline(std::cin, line);
                    ms.parseFromString(line);
                    break;
                }
                case 2: {
                    std::string atom;
                    size_t cnt;
                    std::cout << "Введите имя атома и количество: ";
                    std::cin >> atom >> cnt;
                    ms.addAtom(atom, cnt);
                    break;
                }
                case 3: {
                    std::cout << "Введите вложенное мультимножество (например, {x, y}): ";
                    Multiset sub;
                    std::cin >> sub;
                    ms.addSubset(sub);
                    break;
                }
                case 4: {
                    std::string atom;
                    size_t cnt;
                    std::cout << "Введите имя атома и количество для удаления: ";
                    std::cin >> atom >> cnt;
                    if (ms.removeAtom(atom, cnt))
                        std::cout << "Элемент удален/уменьшен.\n";
                    else
                        std::cout << "Элемент не найден.\n";
                    break;
                }
                case 5: {
                    std::string atom;
                    std::cout << "Введите имя атома: ";
                    std::cin >> atom;
                    std::cout << "Кратность '" << atom << "': " << ms.getAtomCount(atom) << "\n";
                    break;
                }
                case 6: {
                    std::cout << "Введите второе мультимножество для сравнения: ";
                    Multiset other;
                    std::cin >> other;
                    std::cout << "Результат: " << (ms == other ? "РАВНЫ" : "НЕ РАВНЫ") << "\n";
                    break;
                }
                case 7: {
                    std::cout << "Введите второе мультимножество: ";
                    Multiset other;
                    std::cin >> other;
                    std::cout << "Объединение: " << ms.unionWith(other) << "\n"
                              << "Пересечение: " << ms.intersectWith(other) << "\n"
                              << "Разность:    " << ms.differenceWith(other) << "\n";
                    break;
                }
                case 8:
                    ms.clear();
                    break;
                case 0:
                    break;
                default:
                    std::cout << "Неверный пункт меню.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return 0;
}