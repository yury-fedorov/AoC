package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day19Test {

    static final ArrayList<Long> PROGRAM = IntcodeComputer.loadMemory(IOUtil.input("day19").getFirst());
    static final Map<Point, Boolean> MAP = new HashMap<>();

    static boolean isTractorBeam(Point p) {
        final var cp = MAP.getOrDefault(p, null);
        if (cp != null) return cp;
        final var memory = new ArrayList<>(PROGRAM);
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var c = new IntcodeComputer(memory, in, out);
        c.in(p.x);
        c.in(p.y);
        c.run();
        final var r = c.out();
        final var isBeam = r == 1;
        MAP.put(p, isBeam);
        return isBeam;
    }

    static Optional<Range> getBeamLine(long line) {
        final var y = line;
        Optional<Long> min = Optional.empty();
        Optional<Long> max = Optional.empty();
        final var maxX = 3 * line;
        for (var x = 0L; x < maxX; x++) {
            final var p = new Point(x, y);
            final var isBeam = isTractorBeam(p);
            if (min.isEmpty() && isBeam) {
                min = Optional.of(x);
                max = Optional.of(x);
            } else if (min.isPresent() && isBeam) max = Optional.of(x);
            else if (max.isPresent() && !isBeam) break;
        }
        if (min.isPresent() && max.isPresent())
            return Optional.of(new Range(min.get(), max.get()));
        return Optional.empty();
    }

    @Test
    public void solution() {

        final var N = 50;
        var answer1 = 0L;
        for (var y = 0L; y < N; y++) {
            for (var x = 0L; x < N; x++) {
                final var r = isTractorBeam(new Point(x, y));
                answer1 += r ? 1 : 0;
            }
        }

        assertEquals("answer 1", 131, answer1);

        if (Config.isFast()) return; // takes 168 sec
        final var SIZE = 100L;
        long answer2 = 0;
        for (var y = 0L; true; y++) {
            final var r0 = getBeamLine(y);
            if (r0.isEmpty()) continue;
            final var l0 = r0.get().max - r0.get().min + 1;
            if (l0 < SIZE) continue;
            final var r1 = getBeamLine(y + SIZE - 1);
            if (r1.isEmpty()) continue;
            // min r0 max r0
            // min r1 max r1
            if (r0.get().max - r1.get().min >= (SIZE - 1)) {
                // found
                final var x0 = r1.get().min;
                answer2 = (10000 * x0) + y;
                break;
            }
        }
        assertEquals("answer 2", 15231022, answer2);
    }

    record Point(long x, long y) {
    }

    record Range(long min, long max) {
    }
}
