#include <cinttypes>
#include <iostream>

struct JavaClassPrefix {
    std::uint32_t magic;
    std::uint16_t minor_version;
    std::uint16_t major_version;
};

struct JavaClassConstantPool {
    struct Constant {
        std::uint8_t tag;
        std::uint8_t info[];
    };

    std::uint16_t constant_pool_count;
    Constant *constant_pool; // [constant_pool_count-1]
};

struct JavaClassHead {
    std::uint16_t access_flags;
    std::uint16_t this_class;
    std::uint16_t super_class;
    std::uint16_t interfaces_count;
    std::uint16_t interfaces[];
};

struct Attribute {
    std::uint16_t attribute_name_index;
    std::uint32_t attribute_length;
    std::uint8_t info[];
};

struct JavaClassFields {
    struct Field {
        std::uint16_t access_flags;
        std::uint16_t name_index;
        std::uint16_t descriptor_index;
        std::uint16_t attributes_count;
        Attribute *attributes;
    };

    std::uint16_t fields_count;
    Field *fields;
};

struct JavaClassMethods {
    struct Method {
        std::uint16_t access_flags;
        std::uint16_t name_index;
        std::uint16_t descriptor_index;
        std::uint16_t attributes_count;
        Attribute *attributes;
    };

    std::uint16_t methods_count;
    Method *methods;
};

struct JavaClassAttributes {
    std::uint16_t attributes_count;
    Attribute *attributes;
};

int main() { std::cout << "Hello World!\n"; }