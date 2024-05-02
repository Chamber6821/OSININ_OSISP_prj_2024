#include "constant.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoDouble.hpp"
#include "class-file/constant/CoFieldRef.hpp"
#include "class-file/constant/CoFloat.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/CoLong.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/CoString.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include <format>

std::string stringify(p<Constant> constant) {
#define VERIFY(TAG) (verifyConstant<Co##TAG>(Constant::Tag::TAG, constant))
#define TO_STRING(TAG, CALL)                                                   \
  (std::format(                                                                \
    "{}",                                                                      \
    verifyConstant<Co##TAG>(Constant::Tag::TAG, constant)->CALL()              \
  ))
  switch (constant->tag()) {
  case Constant::Tag::Utf8: return std::format("'{}'", VERIFY(Utf8)->value());
  case Constant::Tag::Integer: return TO_STRING(Integer, value);
  case Constant::Tag::Float: return TO_STRING(Float, value);
  case Constant::Tag::Long: return TO_STRING(Long, value);
  case Constant::Tag::Double: return TO_STRING(Double, value);
  case Constant::Tag::Class:
    return std::format("{}", stringify(VERIFY(Class)->name()));
  case Constant::Tag::String:
    return std::format("'{}'", stringify(VERIFY(String)->value()));
  case Constant::Tag::FieldRef:
    return std::format(
      "FieldRef {}.{}",
      stringify(VERIFY(FieldRef)->clazz()),
      stringify(VERIFY(FieldRef)->type())
    );
  case Constant::Tag::MethodRef:
    return std::format(
      "MethodRef {}.{}",
      stringify(VERIFY(MethodRef)->clazz()),
      stringify(VERIFY(MethodRef)->type())
    );
  case Constant::Tag::InterfaceMethodRef:
    return std::format(
      "InterfaceMethodRef {}.{}",
      stringify(VERIFY(InterfaceMethodRef)->clazz()),
      stringify(VERIFY(InterfaceMethodRef)->type())
    );
  case Constant::Tag::NameAndType:
    return std::format(
      "{}:{}",
      stringify(VERIFY(NameAndType)->type()),
      stringify(VERIFY(NameAndType)->name())
    );
  default:
    throw std::runtime_error(
      std::format("Unsupported constant with tag {}", (int)constant->tag())
    );
  }
#undef TO_STRING
#undef VERIFY
}
