#pragma once

#include <algorithm>
#include <vector>

template <typename T>
std::vector<T> intersectSorted(const std::vector<T>& left, const std::vector<T>& right) {
    std::vector<T> result;
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) {
            ++i;
        } else if (right[j] < left[i]) {
            ++j;
        } else {
            result.push_back(left[i]);
            ++i;
            ++j;
        }
    }
    return result;
}

template <typename T>
void insertUniqueSorted(std::vector<T>& sorted, const T& value) {
    auto it = std::lower_bound(sorted.begin(), sorted.end(), value);
    if (it == sorted.end() || !(*it == value)) {
        sorted.insert(it, value);
    }
}

template <typename T>
bool eraseSorted(std::vector<T>& sorted, const T& value) {
    auto it = std::lower_bound(sorted.begin(), sorted.end(), value);
    if (it == sorted.end() || !(*it == value)) {
        return false;
    }
    sorted.erase(it);
    return true;
}
