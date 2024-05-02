#pragma once

#include <exception>
#include <string>

inline std::string stringify(const std::exception &e, int indent = 0) {
  auto part = std::string(indent, ' ') + "exception: " + e.what() + '\n';
  try {
    std::rethrow_if_nested(e);
    return part;
  } catch (const std::exception &nestedException) {
    return part + stringify(nestedException, indent + 1);
  }
}
