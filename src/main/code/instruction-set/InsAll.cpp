#include "InsAll.hpp"
#include "class-file/constant/CoMethodRef.hpp"
#include "class-file/constant/Constant.hpp"
#include "class-file/constant/pool/ConstantPool.hpp"
#include "code/context/Context.hpp"
#include "code/context/instruction-pointer/InstructionPointer.hpp"
#include "code/instruction-set/InsWrap.hpp"
#include "java/class/JavaClasses.hpp"
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

InsAll::InsAll(p<JavaClasses> classes)
    : InsMappedByOpcode(std::map<std::uint8_t, p<InstructionSet>>{
        {0xB8,
         make<InsWrap>([classes](
                         std::span<const std::uint8_t> bytes,
                         p<ConstantPool> pool
                       ) {
           auto methodConstant = verifyConstant<CoMethodRef>(
             Constant::Tag::MethodRef,
             pool->at(mergeBytes(bytes[0], bytes[1]))
           );
           return make<Code::Wrap>([classes,
                                    methodConstant](p<Context> context, auto) {
             auto argumentCount =
               countArguments(methodConstant->type()->type()->value());
             auto arguments = make<JvsVector>(argumentCount);
             for (int i = argumentCount - 1; i >= 0; i--) {
               arguments->at(i) = context->stack()->pop();
             }
             jumpForward(context->instructionPointer(), 3);
             return Code::Call{
               .type = classes->type(methodConstant->clazz()->name()->value()),
               .method =
                 {.name = methodConstant->type()->name()->value(),
                  .signature = methodConstant->type()->type()->value()},
               .arguments = arguments,
             };
           });
         })},
        {0x12, make<InsWrap>([classes](auto bytes, p<ConstantPool> pool) {
           auto constant = valueOfConstant(pool->at(bytes[0]), classes);
           return make<Code::Wrap>([constant](p<Context> context, auto) {
             context->stack()->push(constant);
             jumpForward(context->instructionPointer(), 2);
             return Code::Next{};
           });
         })}
      }) {}
