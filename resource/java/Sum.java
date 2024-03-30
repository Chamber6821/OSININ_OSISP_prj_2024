package resource.java;

public class Sum {
  private int a;
  private int b;

  public Sum(int a, int b) {
    this.a = a;
    this.b = b;
  }

  public int value() {
    return a + b;
  }
}