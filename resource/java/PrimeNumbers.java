import core.BiFunction;
import core.Flow;
import core.Function;
import core.OutputStream;

public class PrimeNumbers {
  public static void main(String[] args) {
    var candidates = new Integer[1000];
    for (int i = 0; i < candidates.length; i++) {
      candidates[i] = i + 1;
    }

    Integer primes[] = Flow.from(candidates)
                         .filter(new IsPrime<Integer>())
                         .map(new ToArray<Integer>())
                         .reduce(new MergeArrays<Integer>());

    for (int i = 0; i < primes.length; i++) {
      OutputStream.stdout.put("Prime: " + primes[i] + "\n");
    }
  }

  static class IsPrime<T> implements Function<T, Boolean> {
    public Boolean call(T arg) { return true; }
  }

  static class ToArray<T> implements Function<T, T[]> {
    public T[] call(T arg) {
      @SuppressWarnings("unchecked") var array = (T[]) new Object[] {arg};
      return array;
    }
  }

  static class MergeArrays<T> implements BiFunction<T[], T[], T[]> {
    public T[] call(T[] a, T[] b) {
      @SuppressWarnings("unchecked")
      var merged = (T[]) new Object[a.length + b.length];
      for (int i = 0; i < a.length; i++) {
        merged[i] = a[i];
      }
      for (int i = 0; i < b.length; i++) {
        merged[i + a.length] = b[i];
      }
      return merged;
    }
  }
}
