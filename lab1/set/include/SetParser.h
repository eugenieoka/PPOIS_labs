#pragma once

#include <cstddef>
#include <string>

#include "Set.h"

class SetParser {
public:
    static Set parse(const std::string& text);  

private:
    Set parseSet();
    Element parseElement();
    std::string parseAtom();

    void skipWhitespace();
    char peek();
    void expect(char expected);

    std::string source_;
    std::size_t pos_ = 0;
};
