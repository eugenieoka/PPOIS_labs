#include <gtest/gtest.h>

#include <initializer_list>
#include <memory>

#include "Element.h"
#include "Set.h"

namespace {

Set makeSet(std::initializer_list<const char*> atoms) {
    Set s;
    for (const char* a : atoms) {
        s.add(Element::atom(a));
    }
    return s;
}

}  

TEST(Set, AddAndContains) {
    Set s;
    EXPECT_TRUE(s.add(Element::atom("a")));
    EXPECT_TRUE(s.contains(Element::atom("a")));
    EXPECT_FALSE(s.contains(Element::atom("b")));
}

TEST(Set, AddRejectsDuplicates) {
    Set s;
    EXPECT_TRUE(s.add(Element::atom("a")));
    EXPECT_FALSE(s.add(Element::atom("a")));
    EXPECT_EQ(s.size(), 1u);
}

TEST(Set, RemoveExisting) {
    Set s = makeSet({"a", "b"});
    EXPECT_TRUE(s.remove(Element::atom("a")));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.contains(Element::atom("a")));
}

TEST(Set, RemoveMissing) {
    Set s = makeSet({"a"});
    EXPECT_FALSE(s.remove(Element::atom("z")));
    EXPECT_EQ(s.size(), 1u);
}

TEST(Set, SizeEmptyClear) {
    Set s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
    s.add(Element::atom("a"));
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 1u);
    s.clear();
    EXPECT_TRUE(s.empty());
}

TEST(Set, ItemsAccessor) {
    Set s = makeSet({"a", "b"});
    const Set& cs = s;
    EXPECT_EQ(cs.items().size(), 2u);
    EXPECT_EQ(cs.items()[0].asAtom(), "a");
}

TEST(Set, Unite) {
    Set a = makeSet({"1", "2"});
    Set b = makeSet({"2", "3"});
    EXPECT_EQ(a.unite(b), makeSet({"1", "2", "3"}));
}

TEST(Set, UniteWithEmptyKeepsOriginal) {
    Set a = makeSet({"1", "2"});
    EXPECT_EQ(a.unite(Set()), a);
}

TEST(Set, Intersect) {
    Set a = makeSet({"1", "2", "3"});
    Set b = makeSet({"2", "3", "4"});
    EXPECT_EQ(a.intersect(b), makeSet({"2", "3"}));
}

TEST(Set, IntersectWithEmptyIsEmpty) {
    Set a = makeSet({"1", "2"});
    EXPECT_TRUE(a.intersect(Set()).empty());
}

TEST(Set, Difference) {
    Set a = makeSet({"1", "2", "3"});
    Set b = makeSet({"3"});
    EXPECT_EQ(a.difference(b), makeSet({"1", "2"}));
    EXPECT_TRUE(a.difference(a).empty());
}

TEST(Set, SymmetricDifference) {
    Set a = makeSet({"1", "2", "3"});
    Set b = makeSet({"3", "4"});
    EXPECT_EQ(a.symmetricDifference(b), makeSet({"1", "2", "4"}));
}

TEST(Set, SymmetricDifferenceIdentities) {
    Set a = makeSet({"1", "2"});
    EXPECT_TRUE(a.symmetricDifference(a).empty());
    EXPECT_EQ(a.symmetricDifference(Set()), a);
    EXPECT_EQ(Set().symmetricDifference(a), a);
}

TEST(Set, EqualityIgnoresOrder) {
    EXPECT_EQ(makeSet({"a", "b"}), makeSet({"b", "a"}));
}

TEST(Set, EqualityDetectsDifferences) {
    EXPECT_NE(makeSet({"a"}), makeSet({"b"}));
    EXPECT_NE(makeSet({"a"}), makeSet({"a", "b"}));
    EXPECT_NE(Set(), makeSet({"a"}));
    EXPECT_FALSE(makeSet({"a"}) != makeSet({"a"}));
}

TEST(Set, ToStringEmpty) {
    EXPECT_EQ(Set().toString(), "{}");
}

TEST(Set, ToStringAtoms) {
    EXPECT_EQ(makeSet({"1", "2"}).toString(), "{1, 2}");
}

TEST(Set, ToStringNested) {
    Set outer;
    outer.add(Element::atom("1"));
    outer.add(Element::nested(std::make_shared<Set>(makeSet({"a", "b"}))));
    EXPECT_EQ(outer.toString(), "{1, {a, b}}");
}
