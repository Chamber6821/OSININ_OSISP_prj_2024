#include "CfParsed.hpp"
#include "class-file/constant/pool/CpParsed.hpp"
#include "class-file/fields/FsParsed.hpp"
#include "class-file/interfaces/IeParsed.hpp"
#include "class-file/methods/MsParsed.hpp"

CfParsed::CfParsed(std::istream &in) {
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

p<CoClass> CfParsed::thisClass() const {
  return verifyConstant<CoClass>(Constant::Tag::Class, pool->at(thisIndex));
}

p<CoClass> CfParsed::superClass() const {
  return verifyConstant<CoClass>(Constant::Tag::Class, pool->at(superIndex));
}
