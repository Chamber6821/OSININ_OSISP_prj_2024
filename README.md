# fiber-jvm
JVM prototype on fibers. Minimal SDK

# TODOs
- [x] Read `.class` file
  - [x] Read constant pool
    - [x] Read constant Utf8
    - [x] Read constant Integer
    - [x] Read constant Float
    - [x] Read constant Long
    - [x] Read constant Double
    - [x] Read constant Class
    - [x] Read constant String
    - [x] Read constant FieldRef
    - [x] Read constant MethodRef
    - [x] Read constant InterfaceMethodRef
    - [x] Read constant NameAndType
  - [x] Read attributes
  - [x] Read fields
  - [x] Read methods
- [ ] Implement attributes
  - [ ] Implement ConstantValue
  - [ ] Implement Code
  - [ ] Implement LineNumberTable
  - [ ] Implement SourceFile
- [ ] Implement bytecode instructions
- [ ] Execute program 'a+b'
- [ ] Define compiler dependent classes (may be `String`?)
- [ ] Implement compiler dependent classes
- [ ] Add `println` to SDK
- [ ] Execute program that prints Hello World
