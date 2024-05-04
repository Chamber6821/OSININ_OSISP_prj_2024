#include "valueOfConstant.hpp"
#include "class-file/constant/CoDouble.hpp"
#include "class-file/constant/CoFloat.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoLong.hpp"
#include "make.hpp"
#include "tool/verifyConstant.hpp"

p<JavaValue> valueOfConstant(p<Constant> constant) {
  switch (constant->tag()) {
  case Constant::Tag::Integer:
    return make<JavaValue>(
      verifyConstant<CoInteger>(Constant::Tag::Integer, constant)->value()
    );
  case Constant::Tag::Float:
    return make<JavaValue>(
      verifyConstant<CoFloat>(Constant::Tag::Float, constant)->value()
    );
  case Constant::Tag::Long:
    return make<JavaValue>(
      verifyConstant<CoLong>(Constant::Tag::Long, constant)->value()
    );
  case Constant::Tag::Double:
    return make<JavaValue>(
      verifyConstant<CoDouble>(Constant::Tag::Double, constant)->value()
    );
  case Constant::Tag::String:
    throw std::runtime_error(
      "Covertion from constant to String object not implemented :("
    );
  default:
    throw std::runtime_error(std::format(
      "Constant with tag {} can't be converted to value",
      (int)constant->tag()
    ));
  }
}
