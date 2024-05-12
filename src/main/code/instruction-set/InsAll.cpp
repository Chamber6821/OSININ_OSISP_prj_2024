#include "InsAll.hpp"
#include "class-file/constant/CoClass.hpp"
#include "class-file/constant/CoInterfaceMethodRef.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/Code.hpp"
#include "code/MethodReference.hpp"
#include "code/context/Context.hpp"
#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include "code/context/stack/StackFrame.hpp"
#include "code/instruction-set/InsWrap.hpp"
#include "java/class/JavaClasses.hpp"
#include "java/value/JavaValue.hpp"
#include "java/value/JavaValues.hpp"
#include "java/value/JvsVector.hpp"
#include "make.hpp"
#include "p.hpp"
#include "tool/mergeBytes.hpp"
#include "tool/valueOfConstant.hpp"
#include "tool/verifyConstant.hpp"
#include <iterator>
#include <regex>

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
  auto arguments = make<JvsVector>(count);
  for (int i = count - 1; i >= 0; i--) {
    *arguments->at(i) = *stack->pop();
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

InsAll::InsAll(p<JavaClasses> classes)
    : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{
        {0xB8, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto method =
             verifyConstant<CoMethodRef>(pool->at(mergeBytes(bytes[0], bytes[1])
             ));
           auto methodRef = referenceOf(method);
           auto type = classes->type(method->clazz()->name()->value());
           return make<Code::Wrap>([=](p<Context> context, auto) {
             jumpForward(context->instructionPointer(), 3);
             return Code::Call{
               .type = type,
               .method = methodRef,
               .arguments = popArguments(
                 context->stack(),
                 countArguments(methodRef.signature)
               ),
             };
           });
         })},
        {0x12, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto constant = valueOfConstant(pool->at(bytes[0]), classes);
           return make<Code::Wrap>([=](p<Context> context, auto) {
             context->stack()->push(constant);
             jumpForward(context->instructionPointer(), 2);
             return Code::Next{};
           });
         })},
        {0xB9, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto methodRef = referenceOf(verifyConstant<CoInterfaceMethodRef>(
             pool->at(mergeBytes(bytes[0], bytes[1]))
           ));
           return make<Code::Wrap>([=](p<Context> context, auto) {
             auto arguments = popArguments(
               context->stack(),
               countArguments(methodRef.signature) + 1
             );
             jumpForward(context->instructionPointer(), 5);
             return Code::Call{
               .type = std::get<p<JavaObject>>(*arguments->at(0))->type(),
               .method = methodRef,
               .arguments = arguments,
             };
           });
         })},
        {0xB1, make<InsWrap>([](auto, auto) {
           return make<Code::Wrap>([](auto, auto) { return Code::ReturnVoid{}; }
           );
         })},
        {0x11, make<InsWrap>([](auto bytes, auto) {
           auto constant =
             make<JavaValue>(std::int32_t(mergeBytes(bytes[0], bytes[1])));
           return make<Code::Wrap>([=](p<Context> context, auto) {
             context->stack()->push(constant);
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0x3C, make<InsWrap>([](auto, auto) {
           return make<Code::Wrap>([](p<Context> context, auto) {
             *context->locals()->at(1) = *context->stack()->pop();
             jumpForward(context->instructionPointer(), 1);
             return Code::Next{};
           });
         })},
        {0xBB, make<InsWrap>([=](auto bytes, p<ConstantPool> pool) {
           auto type = classes->type(
             verifyConstant<CoClass>(pool->at(mergeBytes(bytes[0], bytes[1])))
               ->name()
               ->value()
           );
           return make<Code::Wrap>([=](p<Context> context, auto) {
             context->stack()->push(make<JavaValue>(type->newObject(type)));
             jumpForward(context->instructionPointer(), 3);
             return Code::Next{};
           });
         })},
        {0x59, make<InsWrap>([=](auto, auto) {
           return make<Code::Wrap>([=](p<Context> context, auto) {
             auto value = context->stack()->pop();
             context->stack()->push(value);
             context->stack()->push(value);
             jumpForward(context->instructionPointer(), 1);
             return Code::Next{};
           });
         })},
        {0x1B, make<InsWrap>([=](auto, auto) {
           return make<Code::Wrap>([=](p<Context> context, auto) {
             context->stack()->push(context->locals()->at(1));
             jumpForward(context->instructionPointer(), 1);
             return Code::Next{};
           });
         })}
      }) {}
