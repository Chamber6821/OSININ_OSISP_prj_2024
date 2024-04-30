#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/constant/pool/CpParsed.hpp"
#include "tool/swapBytes.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <ostream>
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

std::wstring stringify(p<Constant> constant) {
#define VERIFY(TAG) (verifyConstant<Co##TAG>(Constant::Tag::TAG, constant))
#define TO_STRING(TAG, CALL)                                                   \
  (std::format(                                                                \
    L"{}",                                                                     \
    verifyConstant<Co##TAG>(Constant::Tag::TAG, constant)->CALL()              \
  ))
  switch (constant->tag()) {
  case Constant::Tag::Utf8: return TO_STRING(Utf8, value);
  case Constant::Tag::Integer: return TO_STRING(Integer, value);
  case Constant::Tag::Float: return TO_STRING(Float, value);
  case Constant::Tag::Long: return TO_STRING(Long, value);
  case Constant::Tag::Double: return TO_STRING(Double, value);
  case Constant::Tag::Class:
    return std::format(L"{}", stringify(VERIFY(Class)->name()));
  case Constant::Tag::String:
    return std::format(L"'{}'", stringify(VERIFY(String)->value()));
  case Constant::Tag::FieldRef:
    return std::format(
      L"FieldRef {}.{}",
      stringify(VERIFY(FieldRef)->clazz()),
      stringify(VERIFY(FieldRef)->type())
    );
  case Constant::Tag::MethodRef:
    return std::format(
      L"MethodRef {}.{}",
      stringify(VERIFY(MethodRef)->clazz()),
      stringify(VERIFY(MethodRef)->type())
    );
  case Constant::Tag::InterfaceMethodRef:
    return std::format(
      L"InterfaceMethodRef {}.{}",
      stringify(VERIFY(InterfaceMethodRef)->clazz()),
      stringify(VERIFY(InterfaceMethodRef)->type())
    );
  case Constant::Tag::NameAndType:
    return std::format(
      L"{}:{}",
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

    fin.read((char *)&prefix, sizeof(prefix));
    prefix.magic = reverseBytes(prefix.magic);
    prefix.minorVersion = reverseBytes(prefix.minorVersion);
    prefix.majorVersion = reverseBytes(prefix.majorVersion);
    std::cout << std::format(
      "Prefix: 0x{:04X} {}.{}\n",
      prefix.magic,
      prefix.majorVersion,
      prefix.minorVersion
    );

    CpParsed pool(fin);
    std::cout << fin.tellg() << std::endl;
    for (int i = 1; i < 100; i++) {
      std::wcout << std::format(L"#{}: {}\n", i, stringify(pool.at(i)));
    }

  } catch (const std::exception &e) {
    std::cerr << stringify(e);
  }
}
