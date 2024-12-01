package aoc19.computer;

import java.util.Queue;

public abstract class AsciiUtil {
    public static CharSequence readOut(Queue<Long> out) {
        final var text = new StringBuilder();
        while (!out.isEmpty()) {
            final long c = out.poll();
            text.append((char) c);
        }
        return text;
    }

    public static void out(Queue<Long> out) {
        System.out.println(readOut(out));
    }

    public static void toIn(Queue<Long> in, CharSequence text) {
        for (var c : text.toString().toCharArray()) {
            in.add((long) c);
        }
    }
}
