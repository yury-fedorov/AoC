package aoc16.d25;

import aoc16.common.IOUtil;
import aoc16.d12.Day12Test;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Pattern;

public class Day25Test extends Day12Test {

    final static int ENOUGH_LENGTH = 1000;

    static boolean isGoodClockSignal(List<Integer> out) {
        int expected = 0;
        for (final var next : out) {
            if (expected != next) return false;
            expected = expected == 0 ? 1 : 0;
        }
        return true;
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d25");
        final var iset = new HashMap<>(initInstuctions());
        final List<Integer> out = new ArrayList<>();
        iset.put(Pattern.compile("^out b$"), (m, c, ci, r) -> {
            out.add(r.getOrDefault('b', 0));
            if (!isGoodClockSignal(out) || out.size() > ENOUGH_LENGTH) return input.size();
            return 1;
        });

        final var code = compile(iset, input);

        int answer1 = 1;
        for (; true; answer1++) {
            final var r = new HashMap<Character, Integer>();
            r.put('a', answer1);
            execute(code, r);
            if (isGoodClockSignal(out) && out.size() >= ENOUGH_LENGTH)
                break;
            out.clear();
        }
        Assert.assertEquals("answer1", 158, answer1);
    }
}
