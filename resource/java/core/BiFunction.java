package core;

public interface BiFunction<A, B, T> {
  T call(A a, B b);
}
