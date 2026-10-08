#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Element.h"

class Set {
public:
    Set() = default;

    bool add(const Element& e);
    bool remove(const Element& e);
    bool contains(const Element& e) const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

    const std::vector<Element>& items() const;

    Set unite(const Set& other) const;
    Set intersect(const Set& other) const;
    Set difference(const Set& other) const;

    bool operator==(const Set& other) const;
    bool operator!=(const Set& other) const;

    std::string toString() const;

private:
    std::vector<Element> elements_;
};
