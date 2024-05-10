#pragma once

#include "Interface.hpp"
#include "p.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>

template <class T>
class Iterable : public Interface {

public:
  class Empty;
  class Single;
  class Sequence;
  class Union;

  virtual bool hasNext() const = 0;

  virtual T next() = 0;
};

template <class T>
class Iterable<T>::Empty : public Iterable {
public:
  bool hasNext() const override { return false; }

  T next() override {
    throw std::runtime_error("Iterable::Empty is always empty");
  }
};

template <class T>
class Iterable<T>::Single : public Iterable {
  std::optional<T> element;

public:
  Single(T element) : element(std::move(element)) {}

  bool hasNext() const override { return element.has_value(); }

  T next() override {
    if (not element.has_value())
      throw std::runtime_error("Iterable::Single already gave element away");
    auto value = std::move(element.value());
    element.reset();
    return value;
  }
};

template <class T>
class Iterable<T>::Sequence : public Iterable {
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

template <class T>
class Iterable<T>::Union : public Iterable {
  std::deque<p<Iterable>> sequences;

public:
  Union(std::deque<p<Iterable>> sequences) : sequences(std::move(sequences)) {}

  template <class... Sequences>
  Union(Sequences &&...sequences)
      : Union(std::deque<p<Iterable>>{sequences...}) {}

  bool hasNext() const override {
    for (const auto &sequence : sequences) {
      if (sequence->hasNext()) return true;
    }
    return false;
  }

  T next() override {
    while (not sequences.empty()) {
      auto &sequence = sequences.front();
      if (sequence->hasNext()) return sequence->next();
      sequences.pop_front();
    }
    throw std::runtime_error("Iterable::Union is exhausted");
  }
};
