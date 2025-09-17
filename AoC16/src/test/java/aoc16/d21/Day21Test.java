package aoc16.d21;

import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Day21Test {

    // swap position 4 with position 0
    final static Pattern SWAP_POS = Pattern.compile("^swap position (\\d+) with position (\\d+)$");
    // swap letter d with letter b
    final static Pattern SWAP_LETTER = Pattern.compile("^swap letter (\\w) with letter (\\w)$");
    final static Pattern REV_POS = Pattern.compile("^reverse positions (\\d+) through (\\d+)$");
    // rotate left 1 step
    final static Pattern ROTATE_LEFT = Pattern.compile("^rotate left (\\d+) step");
    final static Pattern ROTATE_RIGHT = Pattern.compile("^rotate right (\\d+) step");
    // rotate based on position of letter b
    final static Pattern ROTATE_BY_POS = Pattern.compile("^rotate based on position of letter (\\w)$");
    // move position 1 to position 4
    final static Pattern MOVE_POS = Pattern.compile("^move position (\\d+) to position (\\d+)$");
    static final Map<Pattern, Transformation> txMap = initMap();

    static StringBuilder swapPosition(CharSequence t, int a, int b) {
        final var t1 = new StringBuilder(t);
        t1.setCharAt(a, t.charAt(b));
        t1.setCharAt(b, t.charAt(a));
        return t1;
    }

    static StringBuilder swapLetter(CharSequence t, Character a, Character b) {
        final var s = new StringBuilder(t);
        final int ai = s.indexOf(a.toString());
        final int bi = s.indexOf(b.toString());
        return swapPosition(t, ai, bi);
    }

    static StringBuilder reversePosition(CharSequence t, int a, int b) {
        final var result = new StringBuilder();
        for (int i = 0; i < a; i++) result.append(t.charAt(i));
        for (int i = a; i <= b; i++) result.append(t.charAt(b - (i - a)));
        for (int i = b + 1; i < t.length(); i++) result.append(t.charAt(i));
        return result;
    }

    static StringBuilder rotateLeft(CharSequence t, int s) {
        final var result = new StringBuilder(t);
        while (--s >= 0) {
            final char ch = result.charAt(0);
            result.deleteCharAt(0);
            result.append(ch);
        }
        return result;
    }

    static StringBuilder rotateRight(CharSequence t, int s) {
        final int lastIndex = t.length() - 1;
        final var result = new StringBuilder(t);
        while (--s >= 0) {
            final char ch = result.charAt(lastIndex);
            result.deleteCharAt(lastIndex);
            result.insert(0, ch);
        }
        return result;
    }

    static StringBuilder rotateByPosition(CharSequence t, Character c) {
        final var s = new StringBuilder(t);
        final int i = s.indexOf(c.toString());
        return rotateRight(t, 1 + i + (i >= 4 ? 1 : 0));
    }

    static StringBuilder movePosition(CharSequence t, int a, int b) {
        final var s = new StringBuilder(t);
        s.deleteCharAt(a);
        s.insert(b, t.charAt(a));
        return s;
    }

    private static Map<Pattern, Transformation> initMap() {
        final var map = new HashMap<Pattern, Transformation>();
        map.put(SWAP_POS, (m, s) -> swapPosition(s, Integer.parseInt(m.group(1)), Integer.parseInt(m.group(2))));
        map.put(SWAP_LETTER, (m, s) -> swapLetter(s, m.group(1).charAt(0), m.group(2).charAt(0)));
        map.put(REV_POS, (m, s) -> reversePosition(s, Integer.parseInt(m.group(1)), Integer.parseInt(m.group(2))));
        map.put(ROTATE_LEFT, (m, s) -> rotateLeft(s, Integer.parseInt(m.group(1))));
        map.put(ROTATE_RIGHT, (m, s) -> rotateRight(s, Integer.parseInt(m.group(1))));
        map.put(ROTATE_BY_POS, (m, s) -> rotateByPosition(s, m.group(1).charAt(0)));
        map.put(MOVE_POS, (m, s) -> movePosition(s, Integer.parseInt(m.group(1)), Integer.parseInt(m.group(2))));
        return map;
    }

    static StringBuilder apply(String operation, StringBuilder input) {
        for (final var e : txMap.entrySet()) {
            final var m = e.getKey().matcher(operation);
            if (m.find()) return e.getValue().apply(m, input);
        }
        throw new RuntimeException("No match for: " + operation);
    }

    static String scramble(String input, Collection<String> operations) {
        var result = new StringBuilder(input);
        for (final var o : operations) {
            result = apply(o, result);
        }
        return result.toString();
    }

    static Optional<String> answer2(String head, String tail, String answer, Collection<String> operations) {
        if (!tail.isEmpty()) {
            for (final Character c : tail.toCharArray()) {
                final var b = new StringBuilder(tail);
                final var index = b.indexOf(c.toString());
                b.deleteCharAt(index);
                final var o = answer2(head + c, b.toString(), answer, operations);
                if (o.isPresent()) return o;
            }
        } else if (answer.equals(scramble(head, operations))) return Optional.of(head);
        return Optional.empty();
    }

    @Test
    public void test() {
        Assert.assertEquals("ebcda", swapPosition("abcde", 0, 4).toString());
        Assert.assertEquals("edcba", swapLetter("ebcda", 'b', 'd').toString());
        Assert.assertEquals("abcde", reversePosition("edcba", 0, 4).toString());
        Assert.assertEquals("bcdea", rotateLeft("abcde", 1).toString());
        Assert.assertEquals("deabc", rotateRight("abcde", 2).toString());
        Assert.assertEquals("ecabd", rotateByPosition("abdec", 'b').toString());
        Assert.assertEquals("decab", rotateByPosition("ecabd", 'd').toString());

        Assert.assertEquals("decab", scramble("abcde", IOUtil.inputByPath("d21/sample.txt")));
    }

    @Test
    public void solution() {
        final var operations = IOUtil.input("d21");
        final var input = "abcdefgh";
        Assert.assertEquals("answer 1", "aefgbcdh", scramble(input, operations));
        // brute force takes 2 seconds
        Assert.assertEquals("answer 2", "egcdahbf",
                answer2("", input, "fbgdceah", operations).get());
    }

    @FunctionalInterface
    interface Transformation {
        StringBuilder apply(Matcher m, StringBuilder t);
    }
}
