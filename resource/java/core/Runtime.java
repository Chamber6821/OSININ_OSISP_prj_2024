package core;

public final class Runtime {
  public static native void suspend();

  public static native void launch(Runnable task);

  public static native String join(String... strings);

  public static native Ostream stdout();

  public interface Ostream {
    void put(String string);
  }
}
