/**
 * @file Multiset.h
 * @brief Заголовочный файл класса Multiset (Неориентированное мультимножество).
 * @author Студент
 * @version 1.0
 */

#ifndef MULTISET_H
#define MULTISET_H

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

/**
 * @class Multiset
 * @brief Класс «Неориентированное мультимножество» с поддержкой вложенных мультимножеств.
 *
 * Поддерживает повторяющиеся элементы (кратность > 1), вложенные структуры
 * вида {a, {b, b}, {}}, парсинг из строки и потоковый ввод-вывод.
 */
class Multiset {
public:
    /**
     * @struct Element
     * @brief Представляет элемент мультимножества: либо атомарную строку, либо вложенное мультимножество.
     */
    struct Element {
        bool isSubset;                          ///< Флаг: true — вложенное множество, false — атом
        std::string atom;                       ///< Значение атомарного элемента
        std::shared_ptr<Multiset> subset;       ///< Указатель на вложенное мультимножество

        /** @brief Конструктор атомарного элемента. */
        explicit Element(const std::string& val);

        /** @brief Конструктор элемента-подмножества. */
        explicit Element(const Multiset& sub);

        /** @brief Строковое представление элемента. */
        std::string toString() const;

        /** @brief Оператор строгого порядка для хранения в std::map. */
        bool operator<(const Element& other) const;

        /** @brief Оператор сравнения элементов на равенство. */
        bool operator==(const Element& other) const;
    };

private:
    std::map<Element, size_t> elements_; ///< Словарь элементов и их кратностей

    /** @brief Удаление пробельных символов по краям строки. */
    static std::string trim(const std::string& str);

public:
    /** @brief Конструктор по умолчанию (создает пустое мультимножество {}). */
    Multiset() = default;

    /**
     * @brief Конструктор формирования мультимножества из строки.
     * @param str Строковое представление (например, "{a, a, {b, c}, {}}").
     * @throw std::invalid_argument При нарушении баланса скобок или синтаксиса.
     */
    explicit Multiset(const std::string& str);

    /** @brief Конструктор копирования (выполняет глубокое копирование). */
    Multiset(const Multiset& other);

    /** @brief Конструктор перемещения. */
    Multiset(Multiset&& other) noexcept = default;

    /** @brief Деструктор. */
    ~Multiset() noexcept = default;

    /** @brief Оператор присваивания копированием. */
    Multiset& operator=(const Multiset& other);

    /** @brief Оператор присваивания перемещением. */
    Multiset& operator=(Multiset&& other) noexcept = default;

    /**
     * @brief Разбор строки и заполнение мультимножества.
     * @param str Строка вида "{a, a, c, {a, b, b}, {}, {a, {c, c}}}".
     * @throw std::invalid_argument При некорректном формате строки.
     */
    void parseFromString(const std::string& str);

    /**
     * @brief Добавление атомарного элемента.
     * @param atom Имя атомарного элемента.
     * @param count Количество добавляемых экземпляров (по умолчанию 1).
     */
    void addAtom(const std::string& atom, size_t count = 1);

    /**
     * @brief Добавление вложенного мультимножества как одного элемента.
     * @param sub Вложенное мультимножество.
     * @param count Количество добавляемых экземпляров (по умолчанию 1).
     */
    void addSubset(const Multiset& sub, size_t count = 1);

    /**
     * @brief Удаление атомарного элемента.
     * @param atom Имя атома.
     * @param count Количество удаляемых экземпляров (если >= текущей кратности, удаляется полностью).
     * @return true, если элемент был найден и уменьшен/удален.
     */
    bool removeAtom(const std::string& atom, size_t count = 1);

    /**
     * @brief Получить кратность (число вхождений) атомарного элемента.
     * @param atom Имя атома.
     * @return Кратность элемента (0, если отсутствует).
     */
    size_t getAtomCount(const std::string& atom) const;

    /**
     * @brief Получить кратность вложенного мультимножества.
     * @param sub Искомое подмножество.
     * @return Кратность подмножества (0, если отсутствует).
     */
    size_t getSubsetCount(const Multiset& sub) const;

    /**
     * @brief Общая мощность мультимножества (сумма кратностей всех элементов верхнего уровня).
     */
    size_t size() const noexcept;

    /**
     * @brief Количество уникальных элементов верхнего уровня.
     */
    size_t uniqueSize() const noexcept;

    /**
     * @brief Проверка на пустоту.
     */
    bool isEmpty() const noexcept;

    /**
     * @brief Очистка мультимножества.
     */
    void clear() noexcept;

    /**
     * @brief Объединение двух мультимножеств (суммирование кратностей).
     */
    Multiset unionWith(const Multiset& other) const;

    /**
     * @brief Пересечение двух мультимножеств (минимум кратностей).
     */
    Multiset intersectWith(const Multiset& other) const;

    /**
     * @brief Разность двух мультимножеств.
     */
    Multiset differenceWith(const Multiset& other) const;

    /**
     * @brief Преобразование мультимножества в каноническую строку.
     */
    std::string toString() const;

    /** @brief Сравнение мультимножеств на равенство. */
    bool operator==(const Multiset& other) const noexcept;

    /** @brief Сравнение мультимножеств на неравенство. */
    bool operator!=(const Multiset& other) const noexcept;

    /** @brief Строгий порядок для вложенных мультимножеств. */
    bool operator<(const Multiset& other) const noexcept;

    /** @brief Запись мультимножества в поток. */
    friend std::ostream& operator<<(std::ostream& os, const Multiset& ms);

    /** @brief Чтение мультимножества из потока (считывает строку до перевода строки или по балансу скобок). */
    friend std::istream& operator>>(std::istream& is, Multiset& ms);
};

#endif // MULTISET_H