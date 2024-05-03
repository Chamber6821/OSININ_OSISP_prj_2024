package core;

public interface Function<F, T> {
  T call(F arg);
}
