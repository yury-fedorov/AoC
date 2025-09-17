package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day10Test {

    public static final List<Integer> PRIMES = List.of(2, 3, 5, 7, 11, 13, 17, 19, 23);
    public static final char ASTEROID = '#';

    static int common_denominator(int a, int b) {
        final var aa = Math.abs(a);
        final var ab = Math.abs(b);
        var mi = Math.min(aa, ab);
        var ma = Math.max(aa, ab);
        if (mi == 0) return ma;
        if (mi == 1) return 1;
        final var mi0 = mi;
        for (int prime : PRIMES) {
            while (prime <= mi0) {
                if ((mi % prime) != 0) break;
                if ((ma % prime) != 0) break;
                mi = mi / prime;
                ma = ma / prime;
            }
        }
        assertTrue(mi0 > 0 && mi > 0);
        return (mi0 == mi) ? 1 : (mi0 / mi);
    }

    static Map<Point, List<Point>> group(HashSet<Point> asteroids, Point from) {
        final var result = new HashMap<Point, List<Point>>();
        for (var p : asteroids) {
            int dx = p.x() - from.x();
            int dy = p.y() - from.y();
            final int cd = common_denominator(dx, dy);
            if (cd == 0) continue;
            dx /= cd;
            dy /= cd;
            var direction = new Point(dx, dy);
            var list = result.getOrDefault(direction, new ArrayList<>());
            list.add(p);
            result.put(direction, list);
        }
        return result;
    }

    static int countSeen(HashSet<Point> asteroids, Point from) {
        final var passed = group(asteroids, from);
        return passed.size();
    }

    static HashSet<Point> loadMap(String data) {
        final var map = IOUtil.input(data);
        final var allPositions = new HashSet<Point>();
        for (int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map.get(0).length(); x++) {
                final char at = map.get(y).charAt(x);
                if (at == ASTEROID) allPositions.add(new Point(x, y));
            }
        }
        return allPositions;
    }

    static int maxSeen(HashSet<Point> map) {
        return map.stream().mapToInt(p -> countSeen(map, p)).max().orElse(0);
    }

    // [0 - 360)
    static double directionToDegree(Point direction) {
        var degrees = Math.toDegrees(Math.atan2(direction.y(), direction.x()));
        return degrees >= 0 ? degrees : 360 + degrees;
    }

    // clockwize
    static Point nextDirection(Collection<Point> allDirections, Point curDirection) {
        List<Point> list = new ArrayList<>(allDirections);
        list.sort(Comparator.comparingDouble(Day10Test::directionToDegree));
        var index = list.indexOf(curDirection);
        var nextIndex = (index + 1) < list.size() ? (index + 1) : 0;
        return list.get(nextIndex);
    }

    static int distance(Point a, Point b) {
        return Math.abs(a.x() - b.x()) + Math.abs(a.y() - b.y());
    }

    static Optional<Point> getNearest(Point from, List<Point> options) {
        var min = options.stream().mapToInt(o -> distance(from, o)).min().orElse(0);
        return options.stream().filter(o -> distance(from, o) == min).findFirst();
    }

    @Test
    public void test() {
        final var allPositions = loadMap("day10-sample");
        assertEquals("answer 1", 8, maxSeen(allPositions));
    }

    public Point nth(HashSet<Point> all, Point from, int nth) {
        List<Point> removed = new ArrayList<>();
        var direction = new Point(0, -1); // we point up
        while (removed.size() < nth) {
            var targets = group(all, from);
            var options = targets.getOrDefault(direction, Collections.EMPTY_LIST);
            Optional<Point> nearest = getNearest(from, options);
            if (nearest.isPresent()) {
                all.remove(nearest.get());
                removed.add(nearest.get());
            }
            // move forward lazer
            direction = nextDirection(targets.keySet(), direction);
        }
        return removed.get(removed.size() - 1);
    }

    @Test
    public void solution() {
        final var allPositions = loadMap("day10");
        final var maxSeen = maxSeen(allPositions);
        assertEquals("answer 1", 269, maxSeen);
        final var position =
                allPositions.stream().filter(p -> countSeen(allPositions, p) == maxSeen)
                        .findFirst().get();
        final var point = nth(allPositions, position, 200);
        final var answer2 = 100 * point.x() + point.y();
        assertEquals("answer 2", 612, answer2);
    }

    public record Point(int x, int y) {
    }
}
