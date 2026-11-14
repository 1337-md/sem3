/**
 * @file Multiset.cpp
 * @brief Реализация методов класса Multiset.
 */

#include "Multiset.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>

// --- Реализация Multiset::Element ---

Multiset::Element::Element(const std::string& val)
    : isSubset(false), atom(val), subset(nullptr) {
    if (atom.empty()) {
        throw std::invalid_argument("Атомарный элемент не может быть пустой строкой.");
    }
}

Multiset::Element::Element(const Multiset& sub)
    : isSubset(true), atom(""), subset(std::make_shared<Multiset>(sub)) {}

std::string Multiset::Element::toString() const {
    return isSubset ? subset->toString() : atom;
}

bool Multiset::Element::operator<(const Element& other) const {
    if (isSubset != other.isSubset) {
        return !isSubset; // Атомы располагаются перед подмножествами
    }
    if (!isSubset) {
        return atom < other.atom;
    }
    return *subset < *(other.subset);
}

bool Multiset::Element::operator==(const Element& other) const {
    if (isSubset != other.isSubset) return false;
    if (!isSubset) return atom == other.atom;
    return *subset == *(other.subset);
}

// --- Реализация Multiset ---

std::string Multiset::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

Multiset::Multiset(const std::string& str) {
    parseFromString(str);
}

Multiset::Multiset(const Multiset& other) {
    for (const auto& [elem, count] : other.elements_) {
        if (elem.isSubset) {
            elements_.emplace(Element(*(elem.subset)), count);
        } else {
            elements_.emplace(Element(elem.atom), count);
        }
    }
}

Multiset& Multiset::operator=(const Multiset& other) {
    if (this != &other) {
        Multiset temp(other);
        std::swap(elements_, temp.elements_);
    }
    return *this;
}

void Multiset::parseFromString(const std::string& str) {
    std::string s = trim(str);
    if (s.size() < 2 || s.front() != '{' || s.back() != '}') {
        throw std::invalid_argument("Строка мультимножества должна начинаться с '{' и заканчиваться '}'.");
    }

    Multiset temp;
    std::string content = trim(s.substr(1, s.size() - 2));
    if (content.empty()) {
        *this = std::move(temp);
        return;
    }

    int depth = 0;
    size_t tokenStart = 0;

    for (size_t i = 0; i <= content.size(); ++i) {
        char c = (i < content.size()) ? content[i] : ',';
        if (c == '{') {
            depth++;
        } else if (c == '}') {
            depth--;
            if (depth < 0) {
                throw std::invalid_argument("Нарушен баланс фигурных скобок: лишняя '}'.");
            }
        } else if (c == ',' && depth == 0) {
            std::string token = trim(content.substr(tokenStart, i - tokenStart));
            if (token.empty()) {
                throw std::invalid_argument("Пустой элемент между запятыми.");
            }
            if (token.front() == '{') {
                temp.addSubset(Multiset(token));
            } else {
                if (token.find('{') != std::string::npos || token.find('}') != std::string::npos) {
                    throw std::invalid_argument("Некорректные скобки внутри атомарного элемента: " + token);
                }
                temp.addAtom(token);
            }
            tokenStart = i + 1;
        }
    }

    if (depth != 0) {
        throw std::invalid_argument("Нарушен баланс фигурных скобок: не закрыта '{'.");
    }

    *this = std::move(temp);
}

void Multiset::addAtom(const std::string& atom, size_t count) {
    std::string cleaned = trim(atom);
    if (cleaned.empty() || cleaned.find_first_of("{},") != std::string::npos) {
        throw std::invalid_argument("Недопустимое имя атомарного элемента.");
    }
    if (count > 0) {
        elements_[Element(cleaned)] += count;
    }
}

void Multiset::addSubset(const Multiset& sub, size_t count) {
    if (count > 0) {
        elements_[Element(sub)] += count;
    }
}

bool Multiset::removeAtom(const std::string& atom, size_t count) {
    Element key(trim(atom));
    auto it = elements_.find(key);
    if (it == elements_.end()) {
        return false;
    }
    if (count >= it->second) {
        elements_.erase(it);
    } else {
        it->second -= count;
    }
    return true;
}

size_t Multiset::getAtomCount(const std::string& atom) const {
    std::string cleaned = trim(atom);
    if (cleaned.empty()) return 0;
    auto it = elements_.find(Element(cleaned));
    return (it != elements_.end()) ? it->second : 0;
}

size_t Multiset::getSubsetCount(const Multiset& sub) const {
    auto it = elements_.find(Element(sub));
    return (it != elements_.end()) ? it->second : 0;
}

size_t Multiset::size() const noexcept {
    size_t total = 0;
    for (const auto& [elem, count] : elements_) {
        total += count;
    }
    return total;
}

size_t Multiset::uniqueSize() const noexcept {
    return elements_.size();
}

bool Multiset::isEmpty() const noexcept {
    return elements_.empty();
}

void Multiset::clear() noexcept {
    elements_.clear();
}

Multiset Multiset::unionWith(const Multiset& other) const {
    Multiset result(*this);
    for (const auto& [elem, count] : other.elements_) {
        result.elements_[elem] += count;
    }
    return result;
}

Multiset Multiset::intersectWith(const Multiset& other) const {
    Multiset result;
    for (const auto& [elem, count] : elements_) {
        auto it = other.elements_.find(elem);
        if (it != other.elements_.end()) {
            result.elements_[elem] = std::min(count, it->second);
        }
    }
    return result;
}

Multiset Multiset::differenceWith(const Multiset& other) const {
    Multiset result;
    for (const auto& [elem, count] : elements_) {
        auto it = other.elements_.find(elem);
        size_t otherCount = (it != other.elements_.end()) ? it->second : 0;
        if (count > otherCount) {
            result.elements_[elem] = count - otherCount;
        }
    }
    return result;
}

std::string Multiset::toString() const {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& [elem, count] : elements_) {
        std::string repr = elem.toString();
        for (size_t i = 0; i < count; ++i) {
            if (!first) oss << ", ";
            oss << repr;
            first = false;
        }
    }
    oss << "}";
    return oss.str();
}

bool Multiset::operator==(const Multiset& other) const noexcept {
    return elements_ == other.elements_;
}

bool Multiset::operator!=(const Multiset& other) const noexcept {
    return !(*this == other);
}

bool Multiset::operator<(const Multiset& other) const noexcept {
    return elements_ < other.elements_;
}

std::ostream& operator<<(std::ostream& os, const Multiset& ms) {
    os << ms.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Multiset& ms) {
    char ch;
    while (is.get(ch) && std::isspace(static_cast<unsigned char>(ch))) {}
    if (!is) return is;

    if (ch != '{') {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::string buffer = "{";
    int depth = 1;
    while (depth > 0 && is.get(ch)) {
        buffer += ch;
        if (ch == '{') depth++;
        else if (ch == '}') depth--;
    }

    if (depth != 0) {
        is.setstate(std::ios::failbit);
        return is;
    }

    try {
        ms.parseFromString(buffer);
    } catch (...) {
        is.setstate(std::ios::failbit);
    }
    return is;
}