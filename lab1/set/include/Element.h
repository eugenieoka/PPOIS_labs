#pragma once

#include <memory>
#include <string>

class Set; 

class Element {
public:
    static Element atom(std::string value);
    static Element nested(std::shared_ptr<Set> set);

    bool isAtom() const noexcept;
    bool isNested() const noexcept;

    const std::string& asAtom() const;
    const std::shared_ptr<Set>& asSet() const;

    bool operator==(const Element& other) const;
    bool operator!=(const Element& other) const;

    std::string toString() const;

private:
    Element() = default;

    bool isAtom_ = true;
    std::string atom_;
    std::shared_ptr<Set> nested_;
};