import core.Runtime;

public class PrimeNumbers {
  public static void main(String[] args) throws Exception {
    var maxNumber = 1000;

    var waitGroup = new WaitGroup(maxNumber);
    for (int i = 0; i < maxNumber; i++) {
      Runtime.launch(new Task(i + 1, waitGroup));
    }
    waitGroup.waitAll();

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
    private WaitGroup waitGroup;

    public Task(int number, WaitGroup waitGroup) {
      this.number = number;
      this.waitGroup = waitGroup;
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
        waitGroup.done();
      }
    }
  }

  static class WaitGroup {
    private int tasks;

    public WaitGroup(int tasks) {
      this.tasks = tasks;
    }

    public void done() {
      synchronized (this) {
        tasks--;
      }
    }

    public void waitAll() {
      while (tasks > 0)
        Runtime.suspend();
    }
  }
}
