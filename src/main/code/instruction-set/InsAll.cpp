#include "InsAll.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoFieldRef.hpp"
#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "code/instruction-set/InsWrap.hpp"
#include "code/instruction-set/InstructionSet.hpp"
#include "java/class/JavaClasses.hpp"
#include "java/object/JavaObject.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "java/value/JvDouble.hpp"
#include "java/value/JvFloat.hpp"
#include "java/value/JvInt.hpp"
#include "java/value/JvLong.hpp"
#include "java/value/JvObject.hpp"
#include "java/value/JvsAutoExtendable.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/mergeBytes.hpp"
#include "tool/valueOfConstant.hpp"
#include "tool/verifyConstant.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <functional>
#include <iterator>
#include <regex>
#include <stdexcept>

int countArguments(const std::string &signature) {
  std::regex argumentPattern(R"(\[*B|C|D|F|I|J|S|Z|V|L[^;]+;)");
  return std::distance(
    std::sregex_iterator(
      signature.begin() + signature.find('(') + 1,
      signature.begin() + signature.find(')'),
      argumentPattern
    ),
    std::sregex_iterator()
  );
}

void jumpForward(p<InstructionPointer> pointer, int offset) {
  pointer->gotoAddress(pointer->address() + offset);
}

p<JavaValues> popArguments(p<StackFrame> stack, int count) {
  auto arguments = make<JvsAutoExtendable>();
  for (int i = count - 1; i >= 0; i--) {
    arguments->put(i, stack->pop());
  }
  return arguments;
}

template <class TConstant>
MethodReference referenceOf(p<TConstant> constant) {
  return MethodReference{
    .name = constant->type()->name()->value(),
    .signature = constant->type()->type()->value()
  };
}

template <class T>
p<InstructionSet> stackInstruction(
  std::function<T(p<Context>)> action, int instructionLength = 1
) {
  return make<InsWrap>([=](auto, auto) {
    return make<Code::Wrap>([=](p<Context> context) {
      jumpForward(context->instructionPointer(), instructionLength);
      return action(std::move(context));
    });
  });
}

p<InstructionSet> stackInstruction(
  std::function<void(p<Context>)> action, int instructionLength = 1
) {
  return stackInstruction<Code::Next>(
    [=](p<Context> context) {
      action(std::move(context));
      return Code::Next{};
    },
    instructionLength
  );
}

p<InstructionSet> fromStackToLocal(int index) {
  return stackInstruction([=](p<Context> context) {
    context->locals()->put(index, context->stack()->pop());
  });
}

p<InstructionSet> fromLocalToStack(int index) {
  return stackInstruction([=](p<Context> context) {
    context->stack()->push(context->locals()->at(index));
  });
}

p<InstructionSet> fromStackToLocal() {
  return make<InsWrap>([](auto bytes, auto) {
    auto index = bytes[0];
    return make<Code::Wrap>([=](p<Context> context) {
      jumpForward(context->instructionPointer(), 2);
      context->locals()->put(index, context->stack()->pop());
      return Code::Next{};
    });
  });
}

p<InstructionSet> fromLocalToStack() {
  return make<InsWrap>([](auto bytes, auto) {
    auto index = bytes[0];
    return make<Code::Wrap>([=](p<Context> context) {
      jumpForward(context->instructionPointer(), 2);
      context->stack()->push(context->locals()->at(index));
      return Code::Next{};
    });
  });
}

p<InstructionSet> loadValue(p<JavaValue> value) {
  return stackInstruction([=](p<Context> context) {
    context->stack()->push(value);
  });
}

p<JavaValue> makeValue(std::int32_t value) { return make<JvInt>(value); }

p<JavaValue> makeValue(std::int64_t value) { return make<JvLong>(value); }

p<JavaValue> makeValue(float value) { return make<JvFloat>(value); }

p<JavaValue> makeValue(double value) { return make<JvDouble>(value); }

p<JavaValue> makeValue(p<JavaObject> value) { return make<JvObject>(value); }

p<InstructionSet> loadValue(auto value) { return loadValue(makeValue(value)); }

p<InstructionSet>
jumpIf(std::function<bool(p<JavaValue> a, p<JavaValue> b)> comparator) {
  return make<InsWrap>([=](auto bytes, auto) {
    auto offset = std::int16_t(mergeBytes(bytes[0], bytes[1]));
    return make<Code::Wrap>([=](p<Context> context) {
      auto value2 = context->stack()->pop();
      auto value1 = context->stack()->pop();
      jumpForward(
        context->instructionPointer(),
        comparator(std::move(value1), std::move(value2)) ? offset : 3
      );
      return Code::Next{};
    });
  });
}

p<InstructionSet>
jumpIf(std::function<bool(std::int32_t a, std::int32_t b)> comparator) {
  return jumpIf([=](p<JavaValue> a, p<JavaValue> b) {
    return comparator(a->asInt(), b->asInt());
  });
}

p<InstructionSet>
jumpIf(std::function<bool(p<JavaObject> a, p<JavaObject> b)> comparator) {
  return jumpIf([=](p<JavaValue> a, p<JavaValue> b) {
    return comparator(a->asObject(), b->asObject());
  });
}

p<InstructionSet> jumpIf(std::function<bool(p<JavaValue>)> comparator) {
  return make<InsWrap>([=](auto bytes, auto) {
    auto offset = std::int16_t(mergeBytes(bytes[0], bytes[1]));
    return make<Code::Wrap>([=](p<Context> context) {
      auto value = context->stack()->pop();
      jumpForward(
        context->instructionPointer(),
        comparator(std::move(value)) ? offset : 3
      );
      return Code::Next{};
    });
  });
}

p<InstructionSet> jumpIf(std::function<bool(std::int32_t)> comparator) {
  return jumpIf([=](p<JavaValue> x) { return comparator(x->asInt()); });
}

p<InstructionSet> jumpIf(std::function<bool(p<JavaObject>)> comparator) {
  return jumpIf([=](p<JavaValue> x) { return comparator(x->asObject()); });
}

template <class R, class A, class B>
p<InstructionSet> calc(std::function<R(A, B)> action) {
  return stackInstruction([=](p<Context> context) {
    auto stack = context->stack();
    auto value2 = stack->pop()->as<B>();
    auto value1 = stack->pop()->as<A>();
    stack->push(makeValue(action(std::move(value1), std::move(value2))));
  });
}

p<InstructionSet> calc(auto action) {
  std::function func = std::move(action);
  return calc(func);
}

p<InstructionSet> loadFromArray() {
  return stackInstruction([](p<Context> context) {
    auto stack = context->stack();
    auto index = stack->pop()->asInt();
    auto array = stack->pop()->asObject();
    stack->push(array->field(std::format("${}", index)));
  });
}

p<InstructionSet> storeToArray() {
  return stackInstruction([](p<Context> context) {
    auto stack = context->stack();
    auto value = stack->pop();
    auto index = stack->pop()->asInt();
    auto array = stack->pop()->asObject();
    array->setField(std::format("${}", index), std::move(value));
  });
}

p<JavaValue> defaultValueForType(int atype) {
  switch (atype) {
  case 4:
  case 5:
  case 8:
  case 9:
  case 10: return make<JvInt>(0);
  case 6: return make<JvFloat>(0);
  case 7: return make<JvDouble>(0);
  case 11: return make<JvLong>(0);
  default:
    throw std::runtime_error(std::format("Impossible array type: {}", atype));
  }
}

p<InstructionSet>
invokeStaticMethod(p<JavaClasses> classes, int instructionLength) {
  return make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
    auto method =
      verifyConstant<CoMethodRef>(pool->at(mergeBytes(bytes[0], bytes[1])));
    auto methodRef = referenceOf(method);
    auto type = classes->type(method->clazz()->name()->value());
    return make<Code::Wrap>([=](p<Context> context) {
      jumpForward(context->instructionPointer(), instructionLength);
      return Code::Call{
        .type = type,
        .method = methodRef,
        .arguments =
          popArguments(context->stack(), countArguments(methodRef.signature)),
      };
    });
  });
}

template <class TConstant>
p<InstructionSet> invokeMethod(int instructionLength) {
  return make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
    auto methodRef = referenceOf(
      verifyConstant<TConstant>(pool->at(mergeBytes(bytes[0], bytes[1])))
    );
    return make<Code::Wrap>([=](p<Context> context) {
      auto arguments =
        popArguments(context->stack(), countArguments(methodRef.signature) + 1);
      jumpForward(context->instructionPointer(), instructionLength);
      return Code::Call{
        .type = arguments->at(0)->asObject()->type(),
        .method = methodRef,
        .arguments = arguments,
      };
    });
  });
}

p<InstructionSet> returnVoid() {
  return make<InsWrap>([](auto, auto) {
    return make<Code::Wrap>([](auto) { return Code::ReturnVoid{}; });
  });
}

p<InstructionSet> returnValue() {
  return make<InsWrap>([](auto, auto) {
    return make<Code::Wrap>([](p<Context> context) {
      return Code::ReturnValue{context->stack()->pop()};
    });
  });
}

InsAll::InsAll(p<JavaClasses> classes)
    : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{
        {0x12, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto constant = valueOfConstant(pool->at(bytes[0]), classes);
           return make<Code::Wrap>([=](p<Context> context) {
             context->stack()->push(constant);
             jumpForward(context->instructionPointer(), 2);
             return Code::Next{};
           });
         })},
        {0xBB, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto type = classes->type(
             verifyConstant<CoClass>(pool->at(mergeBytes(bytes[0], bytes[1])))
               ->name()
               ->value()
           );
           return make<Code::Wrap>([=](p<Context> context) {
             context->stack()->push(make<JvObject>(type->newObject(type)));
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0x59, stackInstruction([](p<Context> context) {
           auto value = context->stack()->pop();
           context->stack()->push(value);
           context->stack()->push(value);
         })},
        {0xB4, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto fieldName =
             verifyConstant<CoFieldRef>(pool->at(mergeBytes(bytes[0], bytes[1]))
             )
               ->type()
               ->type()
               ->value();
           return make<Code::Wrap>([=](p<Context> context) {
             auto stack = context->stack();
             auto object = stack->pop()->asObject();
             stack->push(object->field(fieldName));
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0xB5, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto fieldName =
             verifyConstant<CoFieldRef>(pool->at(mergeBytes(bytes[0], bytes[1]))
             )
               ->type()
               ->type()
               ->value();
           return make<Code::Wrap>([=](p<Context> context) {
             auto stack = context->stack();
             auto value = stack->pop();
             auto object = stack->pop()->asObject();
             object->setField(fieldName, std::move(value));
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0x01, loadValue(nullptr)},
        {0x02, loadValue(std::int32_t(-1))},
        {0x03, loadValue(std::int32_t(0))},
        {0x04, loadValue(std::int32_t(1))},
        {0x05, loadValue(std::int32_t(2))},
        {0x06, loadValue(std::int32_t(3))},
        {0x07, loadValue(std::int32_t(4))},
        {0x08, loadValue(std::int32_t(5))},
        {0x09, loadValue(std::int64_t(0))},
        {0x0A, loadValue(std::int64_t(1))},
        {0x0B, loadValue(float(0))},
        {0x0C, loadValue(float(1))},
        {0x0D, loadValue(float(2))},
        {0x0E, loadValue(double(0))},
        {0x0F, loadValue(double(1))},
        {0x10, make<InsWrap>([](auto bytes, auto) {
           auto constant = make<JvInt>(bytes[0]);
           return make<Code::Wrap>([=](p<Context> context) {
             jumpForward(context->instructionPointer(), 2);
             context->stack()->push(constant);
             return Code::Next{};
           });
         })},
        {0x11, make<InsWrap>([](auto bytes, auto) {
           auto constant = make<JvInt>(mergeBytes(bytes[0], bytes[1]));
           return make<Code::Wrap>([=](p<Context> context) {
             jumpForward(context->instructionPointer(), 3);
             context->stack()->push(constant);
             return Code::Next{};
           });
         })},
        {0x15, fromLocalToStack()},
        {0x16, fromLocalToStack()},
        {0x17, fromLocalToStack()},
        {0x18, fromLocalToStack()},
        {0x19, fromLocalToStack()},
        {0x1A, fromLocalToStack(0)},
        {0x1B, fromLocalToStack(1)},
        {0x1C, fromLocalToStack(2)},
        {0x1D, fromLocalToStack(3)},
        {0x2A, fromLocalToStack(0)},
        {0x2B, fromLocalToStack(1)},
        {0x2C, fromLocalToStack(2)},
        {0x2D, fromLocalToStack(3)},
        {0x2E, loadFromArray()},
        {0x2F, loadFromArray()},
        {0x30, loadFromArray()},
        {0x31, loadFromArray()},
        {0x32, loadFromArray()},
        {0x33, loadFromArray()},
        {0x34, loadFromArray()},
        {0x35, loadFromArray()},
        {0x36, fromStackToLocal()},
        {0x37, fromStackToLocal()},
        {0x38, fromStackToLocal()},
        {0x39, fromStackToLocal()},
        {0x3A, fromStackToLocal()},
        {0x3B, fromStackToLocal(0)},
        {0x3C, fromStackToLocal(1)},
        {0x3D, fromStackToLocal(2)},
        {0x3E, fromStackToLocal(3)},
        {0x4B, fromStackToLocal(0)},
        {0x4C, fromStackToLocal(1)},
        {0x4D, fromStackToLocal(2)},
        {0x4E, fromStackToLocal(3)},
        {0x4F, storeToArray()},
        {0x50, storeToArray()},
        {0x51, storeToArray()},
        {0x52, storeToArray()},
        {0x53, storeToArray()},
        {0x54, storeToArray()},
        {0x55, storeToArray()},
        {0x56, storeToArray()},
        {0x60, calc([](std::int32_t a, std::int32_t b) { return a + b; })},
        {0x61, calc([](std::int64_t a, std::int64_t b) { return a + b; })},
        {0x62, calc([](float a, float b) { return a + b; })},
        {0x63, calc([](double a, double b) { return a + b; })},
        {0x64, calc([](std::int32_t a, std::int32_t b) { return a - b; })},
        {0x65, calc([](std::int64_t a, std::int64_t b) { return a - b; })},
        {0x66, calc([](float a, float b) { return a - b; })},
        {0x67, calc([](double a, double b) { return a - b; })},
        {0x68, calc([](std::int32_t a, std::int32_t b) { return a * b; })},
        {0x69, calc([](std::int64_t a, std::int64_t b) { return a * b; })},
        {0x6A, calc([](float a, float b) { return a * b; })},
        {0x6B, calc([](double a, double b) { return a * b; })},
        {0x70, calc([](std::int32_t a, std::int32_t b) { return a % b; })},
        {0x71, calc([](std::int64_t a, std::int64_t b) { return a % b; })},
        {0x72, calc([](float a, float b) { return std::fmod(a, b); })},
        {0x73, calc([](double a, double b) { return std::fmod(a, b); })},
        {0x84, make<InsWrap>([](auto bytes, auto) {
           auto localIndex = bytes[0];
           auto delta = std::int8_t(bytes[1]);
           return make<Code::Wrap>([=](p<Context> context) {
             context->locals()->put(
               localIndex,
               make<JvInt>(delta + context->locals()->at(localIndex)->asInt())
             );
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0x99, jumpIf([](std::int32_t value) { return value == 0; })},
        {0x9A, jumpIf([](std::int32_t value) { return value != 0; })},
        {0x9B, jumpIf([](std::int32_t value) { return value < 0; })},
        {0x9C, jumpIf([](std::int32_t value) { return value >= 0; })},
        {0x9D, jumpIf([](std::int32_t value) { return value > 0; })},
        {0x9E, jumpIf([](std::int32_t value) { return value <= 0; })},
        {0x9F, jumpIf([](std::int32_t a, std::int32_t b) { return a == b; })},
        {0xA0, jumpIf([](std::int32_t a, std::int32_t b) { return a != b; })},
        {0xA1, jumpIf([](std::int32_t a, std::int32_t b) { return a < b; })},
        {0xA2, jumpIf([](std::int32_t a, std::int32_t b) { return a >= b; })},
        {0xA3, jumpIf([](std::int32_t a, std::int32_t b) { return a > b; })},
        {0xA4, jumpIf([](std::int32_t a, std::int32_t b) { return a <= b; })},
        {0xA5, jumpIf([](p<JavaObject> a, p<JavaObject> b) { return a == b; })},
        {0xA6, jumpIf([](p<JavaObject> a, p<JavaObject> b) { return a != b; })},
        {0xA7, make<InsWrap>([](auto bytes, auto) {
           auto offset = std::int16_t(mergeBytes(bytes[0], bytes[1]));
           return make<Code::Wrap>([=](p<Context> context) {
             jumpForward(context->instructionPointer(), offset);
             return Code::Next{};
           });
         })},
        {0xAC, returnValue()},
        {0xAD, returnValue()},
        {0xAE, returnValue()},
        {0xAF, returnValue()},
        {0xB0, returnValue()},
        {0xB1, returnVoid()},
        {0xB6, invokeMethod<CoMethodRef>(3)},
        {0xB7, invokeMethod<CoMethodRef>(3)},
        {0xB8, invokeStaticMethod(classes, 3)},
        {0xB9, invokeMethod<CoInterfaceMethodRef>(5)},
        {0xBA, invokeStaticMethod(classes, 5)},
        {0xBC, make<InsWrap>([=](auto bytes, auto) {
           auto value = defaultValueForType(bytes[0]);
           return make<Code::Wrap>([=](p<Context> context) {
             auto length = context->stack()->pop()->asInt();
             auto objectClass = classes->type("java/lang/Object");
             auto array = objectClass->newObject(objectClass);
             array->setField("$length", make<JvInt>(length));
             for (int i = 0; i < length; i++) {
               array->setField(std::format("${}", i), value);
             }
             context->stack()->push(make<JvObject>(array));
             jumpForward(context->instructionPointer(), 2);
             return Code::Next{};
           });
         })},
        {0xBD, make<InsWrap>([=](auto, auto) {
           auto value = make<JvObject>(nullptr);
           return make<Code::Wrap>([=](p<Context> context) {
             auto length = context->stack()->pop()->asInt();
             auto objectClass = classes->type("java/lang/Object");
             auto array = objectClass->newObject(objectClass);
             array->setField("$length", make<JvInt>(length));
             for (int i = 0; i < length; i++) {
               array->setField(std::format("${}", i), value);
             }
             context->stack()->push(make<JvObject>(array));
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0xBE, stackInstruction([](p<Context> context) {
           context->stack()->push(
             context->stack()->pop()->asObject()->field("$length")
           );
         })},
        {0xC2, stackInstruction([=](p<Context> context) -> Code::Result {
           auto value = context->stack()->pop();
           if (value->asObject()->tryLock()) return Code::Next{};
           context->stack()->push(std::move(value));
           jumpForward(context->instructionPointer(), -1);
           return Code::Call{
             .type = classes->type("core/Runtime"),
             .method = {.name = "suspend", .signature = "()V"},
             .arguments = make<JvsAutoExtendable>()
           };
         })},
        {0xC3, stackInstruction([](p<Context> context) {
           context->stack()->pop()->asObject()->unlock();
         })},
        {0xC6, jumpIf([](p<JavaObject> a) { return a == nullptr; })},
        {0xC7, jumpIf([](p<JavaObject> a) { return a != nullptr; })},
      }) {}
