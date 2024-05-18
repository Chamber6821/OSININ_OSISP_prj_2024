#pragma once

#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "p.hpp"

class JvObject : public JavaValue {
  p<JavaObject> value;

public:
  JvObject(p<JavaObject> value) : value(std::move(value)) {}

  p<JavaObject> asObject() const override { return value; }
};
