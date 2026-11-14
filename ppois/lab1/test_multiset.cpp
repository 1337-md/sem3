#include "Multiset.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(MultisetTest, ParseComplexNestedStringFromTask) {
    // Пример напрямую из задания: {a, a, c, {a, b, b}, {}, {a, {c, c}}}
    Multiset ms("{a, a, c, {a, b, b}, {}, {a, {c, c}}}");

    EXPECT_FALSE(ms.isEmpty());
    EXPECT_EQ(ms.size(), 6);        // 6 элементов верхнего уровня: a, a, c, {a,b,b}, {}, {a,{c,c}}
    EXPECT_EQ(ms.uniqueSize(), 5);  // 5 уникальных
    EXPECT_EQ(ms.getAtomCount("a"), 2);
    EXPECT_EQ(ms.getAtomCount("c"), 1);
    EXPECT_EQ(ms.getAtomCount("z"), 0);

    Multiset emptySub("{}");
    Multiset abbSub("{b, a, b}"); // Порядок внутри подмножества не должен влиять!
    EXPECT_EQ(ms.getSubsetCount(emptySub), 1);
    EXPECT_EQ(ms.getSubsetCount(abbSub), 1);
}

TEST(MultisetTest, UnorderedEqualityAndCopy) {
    Multiset ms1("{a, {b, c, b}, a, {}}");
    Multiset ms2("{{}, a, {c, b, b}, a}");
    EXPECT_EQ(ms1, ms2);

    Multiset copyMs(ms1);
    EXPECT_EQ(copyMs, ms1);

    Multiset assignedMs;
    assignedMs = ms2;
    EXPECT_EQ(assignedMs, ms1);

    assignedMs.addAtom("new_el");
    EXPECT_NE(assignedMs, ms1); // Проверка глубокого копирования
}

TEST(MultisetTest, AddRemoveAndOperations) {
    Multiset ms;
    ms.addAtom("x", 3);
    ms.addAtom("y", 1);
    EXPECT_EQ(ms.size(), 4);

    EXPECT_TRUE(ms.removeAtom("x", 2));
    EXPECT_EQ(ms.getAtomCount("x"), 1);
    EXPECT_TRUE(ms.removeAtom("x", 5));
    EXPECT_EQ(ms.getAtomCount("x"), 0);
    EXPECT_FALSE(ms.removeAtom("non_existent"));

    Multiset a("{a, a, b, {c}}");
    Multiset b("{a, b, b, {c}}");

    EXPECT_EQ(a.unionWith(b), Multiset("{a, a, a, b, b, b, {c}, {c}}"));
    EXPECT_EQ(a.intersectWith(b), Multiset("{a, b, {c}}"));
    EXPECT_EQ(a.differenceWith(b), Multiset("{a}"));

    a.clear();
    EXPECT_TRUE(a.isEmpty());
}

TEST(MultisetTest, StreamIOAndInvalidSyntax) {
    std::stringstream ss;
    Multiset original("{a, {b, b}, {}}");
    ss << original;

    Multiset loaded;
    ss >> loaded;
    EXPECT_EQ(original, loaded);

    // Проверка исключений при некорректных строках
    EXPECT_THROW(Multiset("a, b, c"), std::invalid_argument);
    EXPECT_THROW(Multiset("{a, {b}"), std::invalid_argument);
    EXPECT_THROW(Multiset("{a, b}}"), std::invalid_argument);
    EXPECT_THROW(Multiset("{a, , b}"), std::invalid_argument);
    EXPECT_THROW(Multiset("{a{b}}"), std::invalid_argument);

    std::stringstream badStream("invalid_input");
    Multiset failMs;
    badStream >> failMs;
    EXPECT_TRUE(badStream.fail());
}