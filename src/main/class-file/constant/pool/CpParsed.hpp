#pragma once

#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoDouble.hpp"
#include "class-file/constant/CoFieldRef.hpp"
#include "class-file/constant/CoFloat.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/CoLong.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/CoNameAndType.hpp"
#include "class-file/constant/CoString.hpp"
#include "class-file/constant/CoUtf8.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "class-file/constant/pool/CpMutable.hpp"
#include "class-file/constant/pool/CpWeakWrap.hpp"
#include "class-file/parser/Parser.hpp"
#include "class-file/parser/PsCast.hpp"
#include "class-file/parser/PsVariant.hpp"
#include "class-file/parser/constant/CoClassParser.hpp"
#include "class-file/parser/constant/CoDoubleParser.hpp"
#include "class-file/parser/constant/CoFieldRefParser.hpp"
#include "class-file/parser/constant/CoFloatParser.hpp"
#include "class-file/parser/constant/CoIntegerParser.hpp"
#include "class-file/parser/constant/CoInterfaceMethodRefParser.hpp"
#include "class-file/parser/constant/CoLongParser.hpp"
#include "class-file/parser/constant/CoMethodRefParser.hpp"
#include "class-file/parser/constant/CoNameAndTypeParser.hpp"
#include "class-file/parser/constant/CoStringParser.hpp"
#include "class-file/parser/constant/CoUtf8Parser.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/readInt.hpp"
#include <cstdint>
#include <iostream>
#include <istream>
#include <vector>

class CpParsed : public ConstantPool {
  p<CpMutable> cp = make<CpMutable>();

  template <class T>
  using upCast = PsCast<p<T>, p<Constant>>;

public:
  CpParsed(std::istream &in) {
    auto wrap = make<CpWeakWrap>(cp);
    auto parser = PsVariant(std::vector<p<Parser<p<Constant>>>>{
      make<upCast<CoUtf8>>(make<CoUtf8Parser>()),
      make<upCast<CoInteger>>(make<CoIntegerParser>()),
      make<upCast<CoFloat>>(make<CoFloatParser>()),
      make<upCast<CoLong>>(make<CoLongParser>()),
      make<upCast<CoDouble>>(make<CoDoubleParser>()),
      make<upCast<CoClass>>(make<CoClassParser>(wrap)),
      make<upCast<CoString>>(make<CoStringParser>(wrap)),
      make<upCast<CoFieldRef>>(make<CoFieldRefParser>(wrap)),
      make<upCast<CoMethodRef>>(make<CoMethodRefParser>(wrap)),
      make<upCast<CoInterfaceMethodRef>>(make<CoInterfaceMethodRefParser>(wrap)
      ),
      make<upCast<CoNameAndType>>(make<CoNameAndTypeParser>(wrap))
    });
    std::uint16_t count;
    readInt(in, count);
    for (int i = 1; i < count; i++) {
      cp->add(i, parser.parsed(in));
    }
  }

  p<Constant> at(int index) const override { return cp->at(index); }
};
