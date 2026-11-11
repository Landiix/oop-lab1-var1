#pragma once

#include <iostream>
#include <string>

inline int g_passed = 0;
inline int g_failed = 0;

inline void check(bool ok, const std::string& name) {
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << '\n';
    if (ok) {
        ++g_passed;
    } else {
        ++g_failed;
    }
}

inline void printSummary(const std::string& suite) {
    std::cout << "\nИтого (" << suite << "): passed=" << g_passed
              << " failed=" << g_failed << '\n';
}
