#include "valueOfConstant.hpp"
#include "class-file/constant/CoDouble.hpp"
#include "class-file/constant/CoFloat.hpp"
#include "class-file/constant/CoInteger.hpp"
#include "class-file/constant/CoLong.hpp"
#include "class-file/constant/CoString.hpp"
#include "java/class/JavaClasses.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JvDouble.hpp"
#include "java/value/JvFloat.hpp"
#include "java/value/JvInt.hpp"
#include "java/value/JvLong.hpp"
#include "java/value/JvObject.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/verifyConstant.hpp"
#include <format>
#include <string>

p<JavaValue> valueOfConstant(p<Constant> constant, p<JavaClasses> classes) {
  switch (constant->tag()) {
  case Constant::Tag::Integer:
    return make<JvInt>(
      verifyConstant<CoInteger>(Constant::Tag::Integer, constant)->value()
    );
  case Constant::Tag::Float:
    return make<JvFloat>(
      verifyConstant<CoFloat>(Constant::Tag::Float, constant)->value()
    );
  case Constant::Tag::Long:
    return make<JvLong>(
      verifyConstant<CoLong>(Constant::Tag::Long, constant)->value()
    );
  case Constant::Tag::Double:
    return make<JvDouble>(
      verifyConstant<CoDouble>(Constant::Tag::Double, constant)->value()
    );
  case Constant::Tag::String: {
    auto stringClass = classes->type("java/lang/String");
    auto objectClass = classes->type("java/lang/Object");
    auto object = stringClass->newObject(stringClass);
    auto content = verifyConstant<CoString>(Constant::Tag::String, constant)
                     ->value()
                     ->value();
    auto array = objectClass->newObject(objectClass);
    array->setField("$length", make<JvInt>(content.size()));
    for (int i = 0; i < (int)content.size(); i++) {
      array->setField(std::format("${}", i), make<JvInt>(content[i]));
    }
    object->setField("$content", make<JvObject>(array));
    return make<JvObject>(object);
  }
  default:
    throw std::runtime_error(std::format(
      "Constant with tag {} can't be converted to value",
      (int)constant->tag()
    ));
  }
}
