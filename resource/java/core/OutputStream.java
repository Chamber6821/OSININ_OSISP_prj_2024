package core;

public interface OutputStream {

  void put(String string);

  public class Default {
    public static native OutputStream stdout();
  }
}
