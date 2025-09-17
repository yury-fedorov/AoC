package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.stream.*;

import static org.junit.Assert.assertEquals;

public class Day12Test {
    public static final List<Integer> PRIMES = List.of(2, 3, 5, 7, 11, 13, 17, 19, 23);
    static final int[][] DEMO = {{-1, 0, 2}, {2, -10, -7}, {4, -8, 8}, {3, 5, -1}};
    static final int[][] DEMO1 = {{-8, -10, 0}, {5, 5, 10}, {2, -7, 3}, {9, -8, -3}};
    static final int[][] TASK = {{6, -2, -7}, {-6, -7, -4}, {-9, 11, 0}, {-3, -4, 6}};
    static final Velocity VELOCITY_ZERO = new Velocity(0, 0, 0);

    static int dv(int a, int b) {
        return (int) Math.signum(b - a);
    }

    static Velocity velocity(Position a, Position b) {
        return new Velocity(dv(a.x, b.x), dv(a.y, b.y), dv(a.z, b.z));
    }

    static Velocity sum(Velocity a, Velocity b) {
        return new Velocity(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    static Velocity inverse(Velocity v) {
        return new Velocity(-v.x, -v.y, -v.z);
    }

    static Position move(Position a, Velocity b) {
        return new Position(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    static long totalSystemEnergy(List<Moon> moons) {
        return moons.stream().mapToLong(Day12Test::totalMoonEnergy).sum();
    }

    static long totalMoonEnergy(Moon m) {
        final long potentialEnergy = Math.abs(m.position.x) + Math.abs(m.position.y) + Math.abs(m.position.z);
        final long kineticEnergy = Math.abs(m.velocity.x) + Math.abs(m.velocity.y) + Math.abs(m.velocity.z);
        return potentialEnergy * kineticEnergy;
    }

    static List<Moon> init(int[][] data) {
        return Stream.of(data).map(p -> new Moon(new Position(p[0], p[1], p[2]), VELOCITY_ZERO))
                .collect(Collectors.toList());
    }

    static long answer1(List<Moon> moons, int steps) {
        for (var step = 0; step < steps; step++) {
            moonsMakeStep(moons);
        }
        return totalSystemEnergy(moons);
    }

    static State state(Moon m, Dimension d) {
        return switch (d) {
            case X -> new State(m.position.x, m.velocity.x);
            case Y -> new State(m.position.y, m.velocity.y);
            case Z -> new State(m.position.z, m.velocity.z);
        };
    }

    static String state(List<Moon> moons, Dimension d) {
        final var result = new StringBuilder();
        for (var m : moons) {
            result.append('*').append(state(m, d));
        }
        return result.toString();
    }

    static int answer2(List<Moon> moons, Dimension d) {
        var history = new HashMap<String, Integer>();
        for (var step = 0; true; step++) {
            final var ms = state(moons, d);
            var prevStep = history.getOrDefault(ms, -1);
            if (prevStep >= 0) return step - prevStep;
            history.put(ms, step);
            moonsMakeStep(moons);
        }
    }

    static long commonDenominator(long x, long y) {
        long result = 1;
        for (int prime : PRIMES) {
            while (true) {
                if ((x % prime) != 0) break;
                if ((y % prime) != 0) break;
                x /= prime;
                y /= prime;
                result *= prime;
            }
        }
        return result;
    }

    private static void moonsMakeStep(List<Moon> moons) {
        for (int i = 0; i < moons.size(); i++) {
            for (int j = i + 1; j < moons.size(); j++) {
                var a = moons.get(i);
                var b = moons.get(j);
                var dv = velocity(a.position, b.position);
                a.velocity = sum(a.velocity, dv); // no way
                b.velocity = sum(b.velocity, inverse(dv)); // no way
            }
        }
        for (var m : moons) {
            m.position = move(m.position, m.velocity);
        }
    }

    static long answer2(int[][] input) {
        final long x = answer2(init(input), Dimension.X);
        final long y = answer2(init(input), Dimension.Y);
        final long z = answer2(init(input), Dimension.Z);
        final long cd1 = commonDenominator(x, y);
        long xy = (x / cd1) * y;
        final long cd2 = commonDenominator(xy, z);
        return (xy / cd2) * z;
    }

    @Test
    public void demo() {
        assertEquals("answer 1", 179, answer1(init(DEMO), 10));
        assertEquals("answer 2", 2772, answer2(DEMO));
    }

    @Test
    public void demo1() {
        assertEquals("answer 1", 1940, answer1(init(DEMO1), 100));
        assertEquals("answer 2", 4_686_774_924L, answer2(DEMO1));
    }

    @Test
    public void solution() {
        assertEquals("answer 1", 7098, answer1(init(TASK), 1000));
        assertEquals("answer 2", 400128139852752L, answer2(TASK));
    }

    enum Dimension {X, Y, Z}

    // records cannot be inherited but a lot of functionality further are similar, common abstraction is wanted
    record Position(int x, int y, int z) {
    }

    record Velocity(int x, int y, int z) {
    }

    static class Moon {
        Position position;
        Velocity velocity;

        Moon(Position p, Velocity v) {
            position = p;
            velocity = v;
        }
    }

    record State(int position, int velocity) {
    }
}
