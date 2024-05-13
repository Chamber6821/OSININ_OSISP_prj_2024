package core;

public final class Runtime {
  public static native void suspend();

  public static native void launch(Runnable task);

  public static native byte[] join(byte[][] arrays);

  public static native <T> T[] join(T[][] arrays);

  public static native Ostream stdout();

  public interface Ostream {
    void put(String string);
  }
}
