package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day18Test {
    static final char PATH = '.';
    static final char START = '@';

    static boolean isKey(char c) {
        return Character.isLowerCase(c);
    }

    static boolean isDoor(char c) {
        return Character.isUpperCase(c);
    }

    static char doorToKey(char door) {
        return Character.toLowerCase(door);
    }

    static String normalizeKey(String keys) {
        // really matters that the last key is the last
        final var last = lastKey(keys);
        final var list = new ArrayList<>(keys.chars().boxed().map(c -> (char) c.intValue()).toList());
        list.remove((Character) last);
        Collections.sort(list);
        final var nk = new StringBuilder(keys.length());
        for (var k : list) nk.append(k);
        nk.append(last);
        return nk.toString();
    }

    static String normalizeKey2(Step2 step2) {
        final var keys = step2.step.keys;
        final var list = new ArrayList<>(keys.chars().boxed().map(c -> (char) c.intValue()).toList());
        Collections.sort(list);
        final var nk = new StringBuilder(keys.length() + 5);
        for (var k : list) nk.append(k);
        // tail - last doors (where robots are staying)
        nk.append('/');
        var list2 = new ArrayList<String>(4);
        for (var rp : step2.robots) {
            final var pointAsString = String.format("%s,%s;", rp.x, rp.y);
            list2.add(pointAsString);
        }
        Collections.sort(list2);
        for (var rp1 : list2) {
            nk.append(rp1);
        }
        return nk.toString();
    }

    static char lastKey(String keys) {
        return keys.charAt(keys.length() - 1);
    }

    static TunnelMap readMap(String file) {
        final var input = IOUtil.input(file);
        final var maxY = input.size();
        final var mapKey = new HashMap<Character, Point>();
        final var mapDoor = new HashMap<Character, Point>();
        final var walkable = new HashSet<Point>();
        Point start = null;
        for (int y = 0; y < maxY; y++) {
            final var row = input.get(y);
            final var maxX = row.length();
            for (int x = 0; x < maxX; x++) {
                final var p = new Point(x, y);
                final var m = row.charAt(x);
                boolean isWalkable = false;
                if (m == START) {
                    start = p;
                    isWalkable = true;
                } else if (isDoor(m)) {
                    mapDoor.put(m, p);
                    isWalkable = true;
                } else if (isKey(m)) {
                    mapKey.put(m, p);
                    isWalkable = true;
                } else if (m == PATH) {
                    isWalkable = true;
                }
                if (isWalkable) walkable.add(p);
            }
        }
        return new TunnelMap(mapKey, mapDoor, walkable, start);
    }

    static int solution1(String file) {
        return solution1(readMap(file));
    }

    static int solution1(TunnelMap map) {
        final var calculator = createPathCalculator(map);

        var answer1 = Integer.MAX_VALUE;
        final var ALL_KEYS = map.keys.size();

        final var queue = createQueue();

        queue.addAll(calculator.nextStep(Optional.empty(), map.entrance));
        final var minPathForKeys = new HashMap<String, Integer>();
        while (!queue.isEmpty()) {
            final var s = queue.poll();
            final var keys = s.keys;
            final var totalDistance = s.totalDistance;
            if (keys.length() == ALL_KEYS) {
                // solved
                answer1 = Math.min(answer1, totalDistance);
            } else {
                if (totalDistance >= answer1) continue;

                final var nk = normalizeKey(keys);
                final var minTotalDistance = minPathForKeys.getOrDefault(nk, Integer.MAX_VALUE);
                if (minTotalDistance <= totalDistance) continue;
                minPathForKeys.put(nk, totalDistance);

                final var s1 = calculator.nextStep(Optional.of(s), map.keys.get(lastKey(keys)));
                queue.addAll(s1);
            }
        }

        // How many steps is the shortest path that collects all of the keys?
        return answer1;
    }

    static PathCalculator createPathCalculator(TunnelMap map) {
        return new PathCalculator(
                Collections.unmodifiableSet(map.walkable),
                Collections.unmodifiableMap(map.keys),
                Collections.unmodifiableMap(map.doors));
    }

    static void addToQueue(Collection<Point> robots, PathCalculator calculator, Optional<Step> from,
                           Map<Character, Point> keys, Collection<Step2> queue) {
        for (var r : robots) {
            final var s1 = calculator.nextStep(from, r);
            final var rl = new ArrayList<>(robots);
            rl.remove(r);
            for (var s1i : s1) {
                final var sp = keys.get(lastKey(s1i.keys));
                final var r1 = new ArrayList<>(rl);
                r1.add(sp);
                queue.add(new Step2(s1i, r1));
            }
        }
    }

    static int solution2(final TunnelMap map, final int answer1) {
        var answer2 = answer1; // expected to be faster
        final var e = map.entrance;
        // first need to adjust the map
        final var toRemove = List.of(e,
                new Point(e.x - 1, e.y), new Point(e.x + 1, e.y),
                new Point(e.x, e.y - 1), new Point(e.x, e.y + 1));
        map.walkable.removeAll(toRemove);
        final var entrances = Set.of(
                new Point(e.x - 1, e.y - 1), new Point(e.x + 1, e.y + 1),
                new Point(e.x + 1, e.y - 1), new Point(e.x - 1, e.y + 1));

        final var calculator = createPathCalculator(map);
        final var ALL_KEYS = map.keys.size();

        final var queue = createQueue2();
        addToQueue(entrances, calculator, Optional.empty(), map.keys, queue);
        final var minPathForKeys = new HashMap<String, Integer>();
        while (!queue.isEmpty()) {
            final var s = queue.poll();
            final var keys = s.step.keys;
            final var totalDistance = s.step.totalDistance;
            if (keys.length() == ALL_KEYS) {
                // solved
                if (totalDistance < answer2) {
                    answer2 = totalDistance;
                }
            } else {
                if (totalDistance >= answer2) continue;
                final var nk = normalizeKey2(s);
                final var minTotalDistance = minPathForKeys.getOrDefault(nk, Integer.MAX_VALUE);
                if (minTotalDistance <= totalDistance) continue;
                minPathForKeys.put(nk, totalDistance);
                addToQueue(s.robots, calculator, Optional.of(s.step), map.keys, queue);
            }
        }
        return answer2;
    }

    private static Queue<Step> createQueue() {
        return new PriorityQueue<>(100, (a, b) -> {
            // The head of this queue is the LEAST element with respect to the specified ordering.
            return b.priority.compareTo(a.priority);
        });
    }

    private static Queue<Step2> createQueue2() {
        return new PriorityQueue<>(100, (a, b) -> {
            // The head of this queue is the LEAST element with respect to the specified ordering.
            return b.step.priority.compareTo(a.step.priority);
        });
    }

    static List<Point> nextPoint(Point from, Set<Point> walkable, Map<Point, Integer> distance) {
        var result = new ArrayList<Point>(4);
        result.add(new Point(from.x - 1, from.y));
        result.add(new Point(from.x + 1, from.y));
        result.add(new Point(from.x, from.y - 1));
        result.add(new Point(from.x, from.y + 1));
        result.retainAll(walkable);
        result.removeAll(distance.keySet());
        return result;
    }

    int solution2(String file) {
        final var map = readMap(file);
        final var answer1 = solution1(map);
        return solution2(map, answer1);
    }

    @Test
    public void solution() {
        assertEquals("sample 1", 8, solution1("day18-sample1"));
        assertEquals("sample 2", 86, solution1("day18-sample2"));
        assertEquals("sample 3", 132, solution1("day18-sample3"));
        assertEquals("sample 4", 136, solution1("day18-sample4"));
        assertEquals("sample 5", 81, solution1("day18-sample5"));

        if (Config.isFast()) return; // solve the second part takes 12 seconds, in total takes 18 seconds

        final var map = readMap("day18");
        final var answer1 = solution1(map);
        // How many steps is the shortest path that collects all the keys?
        assertEquals("answer 1", 5262, answer1);
        /*
        assertEquals("sample 6 (2/1)", 8, solution2("day18-sample6") );
        assertEquals("sample 6 (2/2)", 24, solution2("day18-sample7") );
        assertEquals("sample 6 (2/3)", 32, solution2("day18-sample8") );
        assertEquals("sample 6 (2/4)", 72, solution2("day18-sample9") );
        */
        final var ANSWER2MAX = 2180;
        assertEquals("answer 2", 2136, solution2(map, ANSWER2MAX));
    }

    record Point(int x, int y) {
    }

    record TunnelMap(Map<Character, Point> keys, Map<Character, Point> doors, Set<Point> walkable, Point entrance) {
    }

    static class Step {
        final int totalDistance;
        final Float priority;
        final String keys;

        Step(String keys, int distance) {
            totalDistance = distance;
            this.keys = keys;
            // higher if more keys and less distance
            final var kl = keys.length();
            priority = kl - totalDistance / (kl + 1f);
        }
    }

    record Step2(Step step, Collection<Point> robots) {
    }

    record PathCalculator(Set<Point> walkable, Map<Character, Point> keys, Map<Character, Point> doors) {
        List<Step> nextStep(Optional<Step> prev, Point start) {
            final var prevKeys = prev.map(step -> step.keys).orElse("");
            final var prevDistance = prev.map(step -> step.totalDistance).orElse(0);

            final var keyPoint = new HashMap<Point, Character>();
            for (var de : doors.entrySet()) {
                if (prevKeys.indexOf(doorToKey(de.getKey())) < 0)
                    keyPoint.put(de.getValue(), de.getKey());
            }
            for (var ke : keys.entrySet()) {
                if (prevKeys.indexOf(ke.getKey()) < 0)
                    keyPoint.put(ke.getValue(), ke.getKey());
            }
            var next = Set.of(start);
            var distance = 0;
            final var distanceMap = new HashMap<Point, Integer>();
            final var nextKeys = new HashMap<Character, Integer>();
            while (!next.isEmpty()) {
                final var next1 = new HashSet<Point>();
                for (var n : next) {
                    boolean isNext = true;
                    final var id = keyPoint.getOrDefault(n, ' ');
                    if (isDoor(id)) {
                        // this is a door (we stop here)
                        // WITHOUT_DOORS nextDoors.put( doorId.get(), distance );
                        isNext = false; // we stop analyses here (at least for now)
                        if (prev.isPresent()) {
                            isNext = prev.get().keys.indexOf(doorToKey(id)) >= 0;
                        }
                    } else if (isKey(id)) {
                        // this is a key
                        nextKeys.put(id, distance);
                        isNext = false; // we approach further steps as an independent step, key is a milestone
                    }
                    if (isNext) {
                        distanceMap.put(n, distance);
                        final var n1 = nextPoint(n, walkable, distanceMap);
                        next1.addAll(n1);
                    }
                }
                next = next1;
                distance++;
            }

            final var result = new ArrayList<Step>();
            for (var ke : nextKeys.entrySet()) {
                result.add(new Step(prevKeys + ke.getKey(), prevDistance + ke.getValue()));
            }
            return result;
        }
    }
}
