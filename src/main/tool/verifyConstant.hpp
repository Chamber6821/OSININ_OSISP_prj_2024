#pragma once

#include "class-file/constant/Constant.hpp"
#include "p.hpp"
#include <cstdint>
#include <format>
#include <memory>
#include <stdexcept>

class CoClass;
class CoDouble;
class CoFieldRef;
class CoString;
class CoFloat;
class CoInteger;
class CoInterfaceMethodRef;
class CoLong;
class CoMethodRef;
class CoUtf8;
class CoNameAndType;

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

template <class TConstant>
auto verifyConstant(p<Constant> constant);

template <>
inline auto verifyConstant<CoUtf8>(p<Constant> constant) {
  return verifyConstant<CoUtf8>(Constant::Tag::Utf8, constant);
}

template <>
inline auto verifyConstant<CoInteger>(p<Constant> constant) {
  return verifyConstant<CoInteger>(Constant::Tag::Integer, constant);
}

template <>
inline auto verifyConstant<CoFloat>(p<Constant> constant) {
  return verifyConstant<CoFloat>(Constant::Tag::Float, constant);
}

template <>
inline auto verifyConstant<CoLong>(p<Constant> constant) {
  return verifyConstant<CoLong>(Constant::Tag::Long, constant);
}

template <>
inline auto verifyConstant<CoDouble>(p<Constant> constant) {
  return verifyConstant<CoDouble>(Constant::Tag::Double, constant);
}

template <>
inline auto verifyConstant<CoClass>(p<Constant> constant) {
  return verifyConstant<CoClass>(Constant::Tag::Class, constant);
}

template <>
inline auto verifyConstant<CoString>(p<Constant> constant) {
  return verifyConstant<CoString>(Constant::Tag::String, constant);
}

template <>
inline auto verifyConstant<CoFieldRef>(p<Constant> constant) {
  return verifyConstant<CoFieldRef>(Constant::Tag::FieldRef, constant);
}

template <>
inline auto verifyConstant<CoMethodRef>(p<Constant> constant) {
  return verifyConstant<CoMethodRef>(Constant::Tag::MethodRef, constant);
}

template <>
inline auto verifyConstant<CoInterfaceMethodRef>(p<Constant> constant) {
  return verifyConstant<CoInterfaceMethodRef>(
    Constant::Tag::InterfaceMethodRef,
    constant
  );
}

template <>
inline auto verifyConstant<CoNameAndType>(p<Constant> constant) {
  return verifyConstant<CoNameAndType>(Constant::Tag::NameAndType, constant);
}
