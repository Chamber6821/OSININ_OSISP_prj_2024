package core;

public final class Flow<T> {
  public static native <T> Flow<T> from(T array[]);

  public native <F> Flow<F> map(Function<T, F> mapper);

  public native Flow<T> filter(Function<T, Boolean> filter);

  public native T reduce(BiFunction<T, T, T> reducer);
}
