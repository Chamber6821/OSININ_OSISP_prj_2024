#pragma once

#include "Interface.hpp"
#include <algorithm>
#include <deque>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>

template <class T>
class Iterable : public Interface {

public:
  class Empty : public Iterable {
  public:
    bool hasNext() const override { return false; }

    T next() override {
      throw std::runtime_error("Iterable::Empty is always empty");
    }
  };

  class Single : public Iterable {
    std::optional<T> element;

  public:
    Single(T element) : element(std::move(element)) {}

    bool hasNext() const override { return element.has_value(); }

    T next() override {
      if (not element.has_value())
        throw std::runtime_error("Iterable::Single already gave element away");
      return std::move(element);
    }
  };

  class Sequence : public Iterable {
    std::queue<T> elements;

  public:
    Sequence(std::queue<T> elements) : elements(std::move(elements)) {}

    template <class... Elements>
    Sequence(Elements &&...elements) : Sequence(std::deque<T>{elements...}) {}

    bool hasNext() const override { return not elements.empty(); }

    T next() override {
      if (elements.empty())
        throw std::runtime_error("Iterable::Sequence is exhausted");
      T element = std::move(elements.front());
      elements.pop();
      return element;
    }
  };

  virtual bool hasNext() const = 0;

  virtual T next() = 0;
};
