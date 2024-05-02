#pragma once

#include "class-file/ClassFile.hpp"
#include "class-file/attribute/Attributes.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/fields/Fields.hpp"
#include "class-file/interfaces/Interfaces.hpp"
#include "class-file/methods/Methods.hpp"
#include "p.hpp"
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
  CfParsed(std::istream &in);

  p<ConstantPool> constantPool() const override { return pool; }

  p<CoClass> thisClass() const override;

  p<CoClass> superClass() const override;

  p<Interfaces> interfaces() const override { return _interfaces; }

  p<Fields> fields() const override { return _fields; }

  p<Methods> methods() const override { return _methods; }

  p<Attributes> attributes() const override { return _attributes; }
};
