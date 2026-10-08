#include "SetParser.h"

#include <cctype>
#include <stdexcept>

namespace {

bool isAtomChar(char c) {
    return c != '{' && c != '}' && c != ',' && !std::isspace(static_cast<unsigned char>(c));
}

}  // namespace

Set SetParser::parse(const std::string& text) {
    SetParser parser;
    parser.source_ = text;
    parser.pos_ = 0;

    parser.skipWhitespace();
    if (parser.pos_ >= parser.source_.size()) {
        throw std::invalid_argument("SetParser: empty input");
    }

    Set result = parser.parseSet();

    parser.skipWhitespace();
    if (parser.pos_ != parser.source_.size()) {
        throw std::invalid_argument(
            "SetParser: unexpected trailing characters at position " + std::to_string(parser.pos_));
    }
    return result;
}

Set SetParser::parseSet() {
    expect('{');
    Set set;

    skipWhitespace();
    if (pos_ < source_.size() && source_[pos_] == '}') {
        ++pos_;
        return set;
    }

    while (true) {
        set.add(parseElement());
        skipWhitespace();

        char c = peek();
        if (c == ',') {
            ++pos_;
            continue;
        }
        if (c == '}') {
            ++pos_;
            return set;
        }
        throw std::invalid_argument(
            "SetParser: expected ',' or '}' but got '" + std::string(1, c) + "' at position " +
            std::to_string(pos_));
    }
}

Element SetParser::parseElement() {
    skipWhitespace();
    if (pos_ < source_.size() && source_[pos_] == '{') {
        return Element::nested(std::make_shared<Set>(parseSet()));
    }
    return Element::atom(parseAtom());
}

std::string SetParser::parseAtom() {
    skipWhitespace();
    std::size_t start = pos_;
    while (pos_ < source_.size() && isAtomChar(source_[pos_])) {
        ++pos_;
    }
    if (pos_ == start) {
        throw std::invalid_argument(
            "SetParser: expected atom at position " + std::to_string(start));
    }
    return source_.substr(start, pos_ - start);
}

void SetParser::skipWhitespace() {
    while (pos_ < source_.size() && std::isspace(static_cast<unsigned char>(source_[pos_]))) {
        ++pos_;
    }
}

char SetParser::peek() {
    if (pos_ >= source_.size()) {
        throw std::invalid_argument(
            "SetParser: unexpected end of input at position " + std::to_string(pos_));
    }
    return source_[pos_];
}

void SetParser::expect(char expected) {
    if (pos_ >= source_.size()) {
        throw std::invalid_argument(
            "SetParser: expected '" + std::string(1, expected) + "' but reached end of input");
    }
    if (source_[pos_] != expected) {
        throw std::invalid_argument(
            "SetParser: expected '" + std::string(1, expected) + "' but got '" +
            std::string(1, source_[pos_]) + "' at position " + std::to_string(pos_));
    }
    ++pos_;
}
