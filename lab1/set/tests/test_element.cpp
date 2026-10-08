#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "Element.h"
#include "Set.h"

TEST(Element, AtomFactory) {
    Element e = Element::atom("a");
    EXPECT_TRUE(e.isAtom());
    EXPECT_FALSE(e.isNested());
    EXPECT_EQ(e.asAtom(), "a");
}

TEST(Element, NestedFactory) {
    auto s = std::make_shared<Set>();
    Element e = Element::nested(s);
    EXPECT_TRUE(e.isNested());
    EXPECT_FALSE(e.isAtom());
    EXPECT_EQ(e.asSet(), s);
}

TEST(Element, NestedNullptrThrows) {
    EXPECT_THROW(Element::nested(nullptr), std::invalid_argument);
}

TEST(Element, AsAtomOnNestedThrows) {
    Element e = Element::nested(std::make_shared<Set>());
    EXPECT_THROW(e.asAtom(), std::logic_error);
}

TEST(Element, AsSetOnAtomThrows) {
    Element e = Element::atom("a");
    EXPECT_THROW(e.asSet(), std::logic_error);
}

TEST(Element, EqualityOfAtoms) {
    EXPECT_EQ(Element::atom("a"), Element::atom("a"));
    EXPECT_NE(Element::atom("a"), Element::atom("b"));
    EXPECT_TRUE(Element::atom("x") != Element::atom("y"));
    EXPECT_FALSE(Element::atom("x") != Element::atom("x"));
}

TEST(Element, AtomNeverEqualsNested) {
    EXPECT_NE(Element::atom("a"), Element::nested(std::make_shared<Set>()));
}

TEST(Element, EqualityOfNested) {
    auto s1 = std::make_shared<Set>();
    s1->add(Element::atom("x"));
    auto s2 = std::make_shared<Set>();
    s2->add(Element::atom("x"));
    auto s3 = std::make_shared<Set>();
    s3->add(Element::atom("y"));

    EXPECT_EQ(Element::nested(s1), Element::nested(s2));
    EXPECT_NE(Element::nested(s1), Element::nested(s3));
}

TEST(Element, ToStringOfAtom) {
    EXPECT_EQ(Element::atom("abc").toString(), "abc");
}

TEST(Element, ToStringOfNested) {
    auto inner = std::make_shared<Set>();
    inner->add(Element::atom("y"));
    auto outer = std::make_shared<Set>();
    outer->add(Element::atom("x"));
    outer->add(Element::nested(inner));

    EXPECT_EQ(Element::nested(outer).toString(), "{x, {y}}");
}
