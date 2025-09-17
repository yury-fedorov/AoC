package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.*;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day15Test {

    static final long WALL = 0;
    static final long PATH = 1;
    static final long OXYGEN_SYSTEM = 2;
    static final long NORTH = 1;
    static final long SOUTH = 2;
    static final long WEST = 3;
    static final long EAST = 4;

    static Set<Point> options(Point from, Collection<Point> path) {
        var n = move(from, NORTH);
        var s = move(from, SOUTH);
        var w = move(from, WEST);
        var e = move(from, EAST);
        var result = new HashSet<>(List.of(n, s, w, e));
        result.retainAll(path); // only path
        return result;
    }

    static Point move(Point from, long direction) {
        var dx = 0;
        var dy = 0;
        switch ((int) direction) {
            case (int) NORTH -> dy = -1;
            case (int) SOUTH -> dy = 1;
            case (int) WEST -> dx = -1;
            case (int) EAST -> dx = 1;
            default -> throw new IllegalStateException("Unexpected direction: " + direction);
        }
        return new Point(from.x + dx, from.y + dy);
    }

    @Test
    public void solution() {
        if (Config.isFast()) return; // takes 30 seconds and could fail on answer 2 as based on random
        final var memory = IntcodeComputer.loadMemory(IOUtil.input("day15").getFirst());
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(memory, in, out);
        final var map = new HashMap<Point, Long>();
        final var start = new Point(0, 0);
        var position = start;
        map.put(position, PATH);
        var direction = NORTH;
        comp.in(direction);
        boolean isOxygenSystemFound = false;
        long i = 0;
        long lastUpdated = -1;
        while (comp.run() != IntcodeComputer.RunPhase.HALT) {
            var answer = comp.out();
            var position1 = move(position, direction);
            final var isAdded = map.put(position1, answer) == null;
            if (isAdded) lastUpdated = i;
            if (answer == PATH) {
                position = position1;
            } else if (answer == OXYGEN_SYSTEM) {
                position = position1;
                isOxygenSystemFound = true;
            }
            direction = ((long) (Math.random() * 4_000) % 4) + 1;
            comp.in(direction);
            i++;
            if ((i - lastUpdated) > 1_000_000 && isOxygenSystemFound) break;
        }
        position = map.entrySet().stream().filter(e -> e.getValue() == OXYGEN_SYSTEM)
                .map(Map.Entry::getKey)
                .findFirst().get();
        var path = map.entrySet().stream()
                .filter(e -> e.getValue() == PATH).map(Map.Entry::getKey).toList();
        var maxPossible = path.size();
        var minPath = new HashMap<Point, Long>();
        var head = Set.of(start);
        var tail = Set.of(position);
        long answer1 = -1;
        for (var step = 0L; step <= maxPossible; step++) {
            var head1 = new HashSet<Point>();
            for (var h : head) {
                final var h1 = options(h, path);
                head1.addAll(h1);
                if (minPath.containsKey(h)) {
                    final var marker = minPath.get(h);
                    if (marker < 0) {
                        answer1 = step + (-marker);
                        break;
                    }
                } else {
                    minPath.put(h, step);
                }
            }
            if (answer1 > 0) break;
            var tail1 = new HashSet<Point>();
            for (var t : tail) {
                final var t1 = options(t, path);
                tail1.addAll(t1);
                if (minPath.containsKey(t)) {
                    final var marker = minPath.get(t);
                    if (marker > 0) {
                        answer1 = step + marker;
                        break;
                    }
                } else {
                    minPath.put(t, -step);
                }
            }
            if (answer1 > 0) break;
            var known = minPath.keySet();
            head1.removeAll(known);
            tail1.removeAll(known);
            head = head1;
            tail = tail1;
        }
        assertEquals("answer 1", 204, answer1);

        var last = Set.of(position);
        var oxygenMap = new HashSet<Point>();
        long answer2 = 0L;
        for (; answer2 <= path.size() && oxygenMap.size() < path.size(); answer2++) {
            final var last1 = new HashSet<Point>();
            for (var p : last) {
                oxygenMap.add(p);
                var p1 = options(p, path);
                last1.addAll(p1);
            }
            last1.removeAll(oxygenMap);
            last = last1;
        }
        assertEquals("answer 2", 340, answer2);
    }

    record Point(long x, long y) {
    }
}
