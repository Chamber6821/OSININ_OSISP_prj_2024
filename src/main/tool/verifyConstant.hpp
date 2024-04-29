#pragma once

#include "class-file/constant/Constant.hpp"
#include "p.hpp"
#include <cstdint>
#include <format>
#include <memory>
#include <stdexcept>

inline auto tagToString(Constant::Tag tag) {
  switch (tag) {
  case Constant::Tag::Utf8: return "Utf8";
  case Constant::Tag::Integer: return "Integer";
  case Constant::Tag::Float: return "Float";
  case Constant::Tag::Long: return "Long";
  case Constant::Tag::Double: return "Double";
  case Constant::Tag::Class: return "Class";
  case Constant::Tag::String: return "String";
  case Constant::Tag::FieldRef: return "FieldRef";
  case Constant::Tag::MethodRef: return "MethodRef";
  case Constant::Tag::InterfaceMethodRef: return "InterfaceMethodRef";
  case Constant::Tag::NameAndType: return "NameAndType";
  case Constant::Tag::MethodHandle: return "MethodHandle";
  case Constant::Tag::MethodType: return "MethodType";
  case Constant::Tag::InvokeDynamic: return "InvokeDynamic";
  }
  throw std::runtime_error(
    std::format("Unexpected value of Constant::Tag: {}", (int)tag)
  );
}

template <class TConstant>
auto verifyConstant(Constant::Tag tag, p<Constant> constant) {
  if (constant->tag() != tag)
    throw std::runtime_error(std::format(
      "Got constant {:X} with tag {}, but expected {}",
      (intptr_t)constant.get(),
      tagToString(constant->tag()),
      tagToString(tag)
    ));
  return std::dynamic_pointer_cast<TConstant>(constant);
}
