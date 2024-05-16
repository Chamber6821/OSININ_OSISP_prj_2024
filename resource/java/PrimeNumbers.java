import core.Runtime;

public class PrimeNumbers {
  public static void main(String[] args) throws Exception {
    var maxNumber = 10000;

    var finish = new Finish(maxNumber);
    for (int i = 0; i < maxNumber; i++) {
      Runtime.launch(new Task(i + 1000000000, finish.markerFor(i)));
    }
    finish.waitAll();

    Runtime.stdout().put("Done!\n");
  }

  static String join(String... strings) {
    var arrayOfBytes = new byte[strings.length][];
    for (int i = 0; i < strings.length; i++) {
      arrayOfBytes[i] = strings[i].getBytes();
    }
    return new String(Runtime.join(arrayOfBytes));
  }

  static class Task implements Runnable {
    private int number;
    private Finish.Marker marker;

    public Task(int number, Finish.Marker marker) {
      this.number = number;
      this.marker = marker;
    }

    public void run() {
      try {
        if (number < 2)
          return;
        for (int i = 2; i * i <= number; i++)
          if (number % i == 0)
            return;
        Runtime.stdout().put(
            join("Prime: ", Integer.valueOf(number).toString(), "\n"));
      } finally {
        marker.done();
      }
    }
  }

  static class Finish {
    private boolean dones[];

    public Finish(int tasks) {
      dones = new boolean[tasks];
    }

    public void waitAll() {
      while (!allDone()) {
        Runtime.suspend();
      }
    }

    public boolean allDone() {
      for (var done : dones) {
        if (!done)
          return false;
      }
      return true;
    }

    public Marker markerFor(int index) {
      return new Marker(index);
    }

    public class Marker {
      private int index;

      Marker(int index) {
        this.index = index;
      }

      public void done() {
        dones[index] = true;
      }
    }
  }
}
