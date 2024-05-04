#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/exception-table/ExceptionTable.hpp"
#include "java/class/JavaClass.hpp"
#include "p.hpp"
#include "tool/mergeBytes.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <format>
#include <stdexcept>
#include <vector>

class EtFromBytes : public ExceptionTable {
  std::vector<std::uint8_t> bytes;
  p<ConstantPool> pool;

public:
  EtFromBytes(std::vector<std::uint8_t> bytes, p<ConstantPool> pool)
      : bytes(std::move(bytes)), pool(std::move(pool)) {}

  bool canCatch(std::uint16_t address, p<JavaClass> type) const override {
    for (int offset = 0; offset < (int)bytes.size(); offset += 8) {
      auto start = mergeBytes(bytes[offset + 0], bytes[offset + 1]);
      auto end = mergeBytes(bytes[offset + 2], bytes[offset + 3]);
      auto typeIndex = mergeBytes(bytes[offset + 6], bytes[offset + 7]);
      if (not(start <= address && address < end)) continue;
      auto handledTypeName =
        verifyConstant<CoClass>(Constant::Tag::Class, pool->at(typeIndex))
          ->name()
          ->value();
      if (handledTypeName != type->name()) continue;
      return true;
    }
    return false;
  }

  std::uint16_t
  handlerFor(std::uint16_t address, p<JavaClass> type) const override {
    for (int offset = 0; offset < (int)bytes.size(); offset += 8) {
      auto start = mergeBytes(bytes[offset + 0], bytes[offset + 1]);
      auto end = mergeBytes(bytes[offset + 2], bytes[offset + 3]);
      auto handler = mergeBytes(bytes[offset + 4], bytes[offset + 5]);
      auto typeIndex = mergeBytes(bytes[offset + 6], bytes[offset + 7]);
      if (not(start <= address && address < end)) continue;
      auto handledTypeName =
        verifyConstant<CoClass>(Constant::Tag::Class, pool->at(typeIndex))
          ->name()
          ->value();
      if (handledTypeName != type->name()) continue;
      return handler;
    }
    throw std::runtime_error(std::format(
      "Can't find handler for exception {:X} by address {}",
      (std::intptr_t)type.get(),
      address
    ));
    return false;
  }
};
