#pragma once

#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include <exception>
#include <format>
#include <iostream>
#include <map>
#include <stdexcept>

class CpMutable : public ConstantPool {
  std::map<int, p<Constant>> map;

public:
  CpMutable(std::map<int, p<Constant>> map) : map(std::move(map)) {}

  CpMutable() : CpMutable(std::map<int, p<Constant>>{}) {}

  p<Constant> at(int index) const override try {
    std::cout << std::format("Get constant #{}\n", index);
    return map.at(index);
  } catch (...) {

    std::throw_with_nested(std::runtime_error(
      std::format("Constant with index {} does not exists", index)
    ));
  }

  void add(int index, p<Constant> constant) { map[index] = constant; }
};
