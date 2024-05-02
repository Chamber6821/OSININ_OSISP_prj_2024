#pragma once

#include "class-file/ClassFile.hpp"
#include "class-file/attribute/AsParsed.hpp"
#include "class-file/attribute/Attributes.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/constant/pool/CpParsed.hpp"
#include "class-file/fields/Fields.hpp"
#include "class-file/fields/FsParsed.hpp"
#include "class-file/interfaces/IeParsed.hpp"
#include "class-file/interfaces/Interfaces.hpp"
#include "class-file/methods/Methods.hpp"
#include "class-file/methods/MsParsed.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/readInt.hpp"
#include "tool/verifyConstant.hpp"
#include <cstdint>
#include <istream>

class CfParsed : public ClassFile {
  p<ConstantPool> pool;
  std::uint16_t thisIndex;
  std::uint16_t superIndex;
  p<Interfaces> _interfaces;
  p<Fields> _fields;
  p<Methods> _methods;
  p<Attributes> _attributes;

public:
  CfParsed(std::istream &in) {
    in.seekg(8, std::ios::cur);
    pool = make<CpParsed>(in);
    in.seekg(2, std::ios::cur);
    readInt(in, thisIndex);
    readInt(in, superIndex);
    _interfaces = make<IeParsed>(in, pool);
    _fields = make<FsParsed>(in, pool);
    _methods = make<MsParsed>(in, pool);
    _attributes = make<AsParsed>(in, pool);
  }

  p<ConstantPool> constantPool() const override { return pool; }

  p<CoClass> thisClass() const override {
    return verifyConstant<CoClass>(Constant::Tag::Class, pool->at(thisIndex));
  }

  p<CoClass> superClass() const override {
    return verifyConstant<CoClass>(Constant::Tag::Class, pool->at(superIndex));
  }

  p<Interfaces> interfaces() const override { return _interfaces; }

  p<Fields> fields() const override { return _fields; }

  p<Methods> methods() const override { return _methods; }

  p<Attributes> attributes() const override { return _attributes; }
};
