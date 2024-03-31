#pragma once

#include <memory>

template <class T, class... Args>
auto make(Args &&...args) {
    return std::make_shared<T>(std::forward<T>(args)...);
}