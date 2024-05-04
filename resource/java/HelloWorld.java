import core.OutputStream;

public class HelloWorld {
  public static void main(String[] args) {
    OutputStream.Default.stdout().put("Hello World!\n");
  }
}
