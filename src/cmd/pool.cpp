#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/CpParsed.hpp"
#include "tool/readInt.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

std::string stringify(const std::exception &e, int indent = 0) {
  auto part = std::string(indent, ' ') + "exception: " + e.what() + '\n';
  try {
    std::rethrow_if_nested(e);
    return part;
  } catch (const std::exception &nestedException) {
    return part + stringify(nestedException, indent + 1);
  }
}

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

int main() {
  try {
    const char *classPath = "resource/java/Sum.class";
    std::ifstream fin(classPath, std::ios::binary);

    struct {
      std::uint32_t magic;
      std::uint16_t minorVersion;
      std::uint16_t majorVersion;
    } prefix;

    readInt(fin, prefix.magic);
    readInt(fin, prefix.minorVersion);
    readInt(fin, prefix.majorVersion);
    std::cout << std::format(
      "Prefix: 0x{:04X} {}.{}\n",
      prefix.magic,
      prefix.majorVersion,
      prefix.minorVersion
    );

    CpParsed pool(fin);
    for (int i = 1; i < 100; i++) {
      std::cout << std::format("#{}: {}\n", i, stringify(pool.at(i)));
    }

  } catch (const std::exception &e) {
    std::cerr << stringify(e);
  }
}
