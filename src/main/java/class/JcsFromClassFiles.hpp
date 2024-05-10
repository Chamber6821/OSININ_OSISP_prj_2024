#pragma once

#include "class-file/ClassFile.hpp"
#include "java/class/JavaClass.hpp"
#include "java/class/JcFromClassFile.hpp"
#include "java/class/JcsMap.hpp"
#include "make.hpp"
#include "p.hpp"
#include <csignal>
#include <set>
#include <utility>

class JcsFromClassFiles : public JavaClasses {
  p<JcsMap> origin;

public:
  JcsFromClassFiles(p<JcsMap> origin, std::set<p<ClassFile>> files)
      : origin(origin) {
    while (not files.empty()) {
      std::set<p<ClassFile>> toRemove;
      for (const auto &file : files) {
        auto superName = file->superClass()->name()->value();
        if (not origin->has(superName)) continue;
        auto super = origin->type(superName);
        origin->add(
          file->thisClass()->name()->value(),
          make<JcFromClassFile>(file, origin)
        );
        toRemove.insert(file);
      }
      if (toRemove.empty())
        throw std::runtime_error("Can't build Java class tree");
      for (auto file : toRemove)
        files.erase(file);
    }
  }

  bool has(std::string name) const override {
    return origin->has(std::move(name));
  }

  p<JavaClass> type(std::string name) const override {
    return origin->type(std::move(name));
  }
};
