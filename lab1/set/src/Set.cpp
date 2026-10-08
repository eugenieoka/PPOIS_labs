#include "Set.h"

#include <algorithm>

bool Set::add(const Element& e) {
    if (contains(e)) {
        return false;
    }
    elements_.push_back(e);
    return true;
}

bool Set::remove(const Element& e) {
    auto it = std::find_if(elements_.begin(), elements_.end(), [&e](const Element& current) { return current == e; });
    if (it == elements_.end()) {
        return false;
    }
    elements_.erase(it);
    return true;
}

bool Set::contains(const Element& e) const {
    return std::find_if(elements_.begin(), elements_.end(), [&e](const Element& current) { return current == e; }) != elements_.end();
}

std::size_t Set::size() const noexcept {
    return elements_.size();
}

bool Set::empty() const noexcept {
    return elements_.empty();
}

void Set::clear() noexcept {
    elements_.clear();
}

const std::vector<Element>& Set::items() const {
    return elements_;
}

Set Set::unite(const Set& other) const {
    Set result = *this;
    for (const Element& e : other.elements_) {
        result.add(e);
    }
    return result;
}

Set Set::intersect(const Set& other) const {
    Set result;
    for (const Element& e : elements_) {
        if (other.contains(e)) {
            result.add(e);
        }
    }
    return result;
}

Set Set::difference(const Set& other) const {
    Set result;
    for (const Element& e : elements_) {
        if (!other.contains(e)) {
            result.add(e);
        }
    }
    return result;
}

Set Set::symmetricDifference(const Set& other) const {
    return difference(other).unite(other.difference(*this));
}

bool Set::operator==(const Set& other) const {
    if (size() != other.size()) {
        return false;
    }
    for (const Element& e : elements_) {
        if (!other.contains(e)) {
            return false;
        }
    }
    for (const Element& e : other.elements_) {
        if (!contains(e)) {
            return false;
        }
    }
    return true;
}

bool Set::operator!=(const Set& other) const {
    return !(*this == other);
}

std::string Set::toString() const {
    std::string result = "{";
    for (std::size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += elements_[i].toString();
    }
    result += "}";
    return result;
}
