#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "Set.h"
#include "SetParser.h"

namespace {

std::string parseError(const std::string& input) {
    try {
        SetParser::parse(input);
    } catch (const std::invalid_argument& e) {
        return e.what();
    }
    return "";
}

}  

TEST(SetParser, EmptySet) {
    EXPECT_EQ(SetParser::parse("{}"), Set());
    EXPECT_EQ(SetParser::parse("{ }"), Set());
}

TEST(SetParser, SingleAtom) {
    Set expected;
    expected.add(Element::atom("1"));
    EXPECT_EQ(SetParser::parse("{1}"), expected);
}

TEST(SetParser, AtomsSeparatedByCommas) {
    Set s = SetParser::parse("{1,2,3}");
    EXPECT_EQ(s.size(), 3u);
    EXPECT_TRUE(s.contains(Element::atom("1")));
    EXPECT_TRUE(s.contains(Element::atom("3")));
}

TEST(SetParser, SkipsWhitespace) {
    Set s = SetParser::parse("\n  { a ,\tb }  \n");
    EXPECT_TRUE(s.contains(Element::atom("a")));
    EXPECT_TRUE(s.contains(Element::atom("b")));
    EXPECT_EQ(s.size(), 2u);
}

TEST(SetParser, NestedSets) {
    Set s = SetParser::parse("{1, {a, {b}}, 2}");
    EXPECT_EQ(s.size(), 3u);
    EXPECT_TRUE(s.contains(Element::atom("1")));
    EXPECT_TRUE(s.contains(Element::atom("2")));
    bool hasNested = false;
    for (const Element& e : s.items()) {
        if (e.isNested()) {
            hasNested = true;
        }
    }
    EXPECT_TRUE(hasNested);
}

TEST(SetParser, DeeplyNestedEmpty) {
    Set s = SetParser::parse("{{}}");
    EXPECT_EQ(s.toString(), "{{}}");
}

TEST(SetParser, DuplicatesAreCollapsed) {
    EXPECT_EQ(SetParser::parse("{1, 1, 2}").size(), 2u);
    EXPECT_EQ(SetParser::parse("{{}, {}}").size(), 1u);
}

TEST(SetParser, IdentifiersAsAtoms) {
    Set s = SetParser::parse("{x1, y_2, z-3}");
    EXPECT_EQ(s.size(), 3u);
    EXPECT_TRUE(s.contains(Element::atom("y_2")));
}

TEST(SetParser, RoundTripToString) {
    const std::string text = "{1, {a, b}}";
    EXPECT_EQ(SetParser::parse(text).toString(), text);
}

TEST(SetParser, EmptyInputThrows) {
    EXPECT_THROW(SetParser::parse(""), std::invalid_argument);
    EXPECT_THROW(SetParser::parse("   \n"), std::invalid_argument);
    EXPECT_NE(parseError("").find("empty input"), std::string::npos);
}

TEST(SetParser, MissingOpeningBraceThrows) {
    EXPECT_THROW(SetParser::parse("1, 2"), std::invalid_argument);
    const std::string msg = parseError("1, 2");
    EXPECT_NE(msg.find("expected '{'"), std::string::npos);
    EXPECT_NE(msg.find("position 0"), std::string::npos);
}

TEST(SetParser, UnexpectedEndOfInputThrows) {
    EXPECT_THROW(SetParser::parse("{1"), std::invalid_argument);
    EXPECT_THROW(SetParser::parse("{a, {b}"), std::invalid_argument);
    EXPECT_NE(parseError("{1").find("unexpected end of input"), std::string::npos);
}

TEST(SetParser, MissingAtomThrows) {
    EXPECT_THROW(SetParser::parse("{1,}"), std::invalid_argument);
    EXPECT_THROW(SetParser::parse("{,1}"), std::invalid_argument);
    EXPECT_NE(parseError("{1,}").find("expected atom"), std::string::npos);
}

TEST(SetParser, MissingCommaThrows) {
    EXPECT_THROW(SetParser::parse("{1 2}"), std::invalid_argument);
    const std::string msg = parseError("{1 2}");
    EXPECT_NE(msg.find("expected ',' or '}'"), std::string::npos);
    EXPECT_NE(msg.find("position 3"), std::string::npos);
}

TEST(SetParser, TrailingCharactersThrow) {
    EXPECT_THROW(SetParser::parse("{1} {2}"), std::invalid_argument);
    EXPECT_THROW(SetParser::parse("{ } }"), std::invalid_argument);
    EXPECT_THROW(SetParser::parse("{1}junk"), std::invalid_argument);
    EXPECT_NE(parseError("{1} {2}").find("unexpected trailing characters"),
              std::string::npos);
}
