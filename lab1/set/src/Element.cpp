#include "Element.h"

#include <stdexcept>

#include "Set.h"

Element Element::atom(std::string value) {
    Element e;
    e.isAtom_ = true;
    e.atom_ = std::move(value);
    return e;
}

Element Element::nested(std::shared_ptr<Set> set) {
    if (!set) {
        throw std::invalid_argument("Element::nested: set is null");
    }
    Element e;
    e.isAtom_ = false;
    e.nested_ = std::move(set);
    return e;
}

bool Element::isAtom() const noexcept {
    return isAtom_;
}

bool Element::isNested() const noexcept {
    return !isAtom_;
}

const std::string& Element::asAtom() const {
    if (!isAtom_) {
        throw std::logic_error("Element::asAtom: element is not an atom");
    }
    return atom_;
}

const std::shared_ptr<Set>& Element::asSet() const {
    if (isAtom_) {
        throw std::logic_error("Element::asSet: element is not a set");
    }
    return nested_;
}

bool Element::operator==(const Element& other) const {
    if (isAtom_ != other.isAtom_) {
        return false;
    }
    if (isAtom_) {
        return atom_ == other.atom_;
    }
    return *nested_ == *other.nested_;
}

bool Element::operator!=(const Element& other) const {
    return !(*this == other);
}

std::string Element::toString() const {
    if (isAtom_) {
        return atom_;
    }
    return nested_->toString();
}