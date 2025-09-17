package aoc19;

import org.junit.Test;

import java.util.*;

import static org.junit.Assert.assertEquals;

public class Day20Test {

    static Set<Point> step(Point p) {
        return Set.of(new Point(p.x, p.y + 1), new Point(p.x, p.y - 1),
                new Point(p.x - 1, p.y), new Point(p.x + 1, p.y));
    }

    static Maze createMaze(String file) {
        final var input = IOUtil.input(file);
        final var walkable = new HashSet<Point>();
        final var letters = new HashMap<Point, Character>();

        final var yMax = input.size();
        for (var y = 0; y < yMax; y++) {
            final var line = input.get(y);
            final var xMax = line.length();
            for (var x = 0; x < xMax; x++) {
                final var c = line.charAt(x);
                if (c == '.') walkable.add(new Point(x, y));
                else if (Character.isUpperCase(c)) letters.put(new Point(x, y), c);
            }
        }
        return new Maze(walkable, createDoors(letters, walkable));
    }

    static boolean isClose(Point a, Point b) {
        return step(a).contains(b);
    }

    static Point getPoint(Point a, Point b, Set<Point> walkable) {
        final var s = new HashSet<>(step(a));
        s.addAll(step(b));
        s.retainAll(walkable);
        return s.iterator().next();
    }

    static String getName(Point ap, char ac, Point bp, char bc) {
        final var isSameX = ap.x == bp.x;
        final var isAb = isSameX ? ap.y < bp.y : ap.x < bp.x;
        return isAb ? ("" + ac) + bc : ("" + bc) + ac;
    }

    static Map<String, List<Point>> createDoors(Map<Point, Character> letters, Set<Point> walkable) {
        final var doors = new HashMap<String, List<Point>>();
        final var points = new ArrayList<>(letters.keySet());
        while (!points.isEmpty()) {
            final var a = points.removeFirst();
            Point b = null;
            for (var i : points) {
                if (isClose(a, i)) {
                    b = i;
                    points.remove(b);
                    break;
                }
            }
            final var name = getName(a, letters.get(a), b, letters.get(b));
            final var d = doors.getOrDefault(name, new ArrayList<>(2));
            d.add(getPoint(a, b, walkable));
            doors.put(name, d);
        }
        return doors;
    }

    static List<Portal> getPortals(Maze maze) {
        final var allDoors = maze.doors.values().stream().flatMap(Collection::stream).toList();
        final var minX = allDoors.stream().mapToInt(p -> p.x).min().getAsInt();
        final var maxX = allDoors.stream().mapToInt(p -> p.x).max().getAsInt();
        final var minY = allDoors.stream().mapToInt(p -> p.y).min().getAsInt();
        final var maxY = allDoors.stream().mapToInt(p -> p.y).max().getAsInt();
        final var outerX = Set.of(minX, maxX);
        final var outerY = Set.of(minY, maxY);
        final var pairPortals = maze.doors.entrySet().stream()
                .filter(e -> e.getValue().size() == 2)
                .map(e -> {
                    var ab = e.getValue();
                    var a = ab.get(0);
                    var b = ab.get(1);
                    var isAOuter = outerX.contains(a.x) || outerY.contains(a.y);
                    var outer = isAOuter ? a : b;
                    var inner = isAOuter ? b : a;
                    return new Portal(e.getKey(), outer, inner);
                }).toList();
        return pairPortals;
    }

    static int solution(boolean isPart1, Maze maze) {
        final var aa = maze.doors.get("AA").getFirst();
        var next = Map.of(new Point3D(aa, 0), 0); // generalized next
        var minDistance = new HashMap<Point3D, Integer>(); // generalized min distance
        final var zz = maze.doors.get("ZZ").getFirst();
        final var portals = getPortals(maze);
        final var allPairDoors = maze.doors.values().stream().filter(c -> c.size() == 2).toList();
        final var allInnerDoors = portals.stream().map(p -> p.inner).toList();
        final var allOuterDoors = portals.stream().map(p -> p.outer).toList();
        // outer doors are absent on level 0 for part 2
        // aa and zz are absent on level 1+ for part 2

        // 1 is it possible to walk from AA to ZZ directly without portals?
        // this is the maximum path required to arrive
        // after this options are: look in the range of minimal path all reachable portals
        // try to go measure paths using them
        while (!next.isEmpty()) {
            final var next1 = new HashMap<Point3D, Integer>();
            for (var n : next.entrySet()) {
                final var p = n.getKey();
                final var minD = minDistance.getOrDefault(p, Integer.MAX_VALUE);
                final var d = n.getValue();
                if (d < minD) {
                    minDistance.put(p, d);
                } else {
                    continue; // no sense to continue
                }
                final var p1 = new HashSet<>(step(p.point));
                p1.retainAll(maze.walkable);
                if (!isPart1 && p.level == 0) {
                    allOuterDoors.forEach(p1::remove);
                }
                final var p13d = new ArrayList<>(p1.stream().map(i -> new Point3D(i, p.level)).toList());
                final var di = allPairDoors.stream().filter(c -> c.contains(p.point)).iterator();
                if (di.hasNext()) {
                    // we found a portal
                    int level = p.level;
                    if (!isPart1) {
                        final var isInnerDoor = allInnerDoors.contains(p.point);
                        level += isInnerDoor ? 1 : -1;
                    }
                    if (level <= 100) { // we put the maximum depth
                        final var portalDoors = di.next();
                        final var otherPoint = new ArrayList<>(portalDoors);
                        otherPoint.remove(p.point);
                        p13d.add(new Point3D(otherPoint.getFirst(), level));
                    }
                }

                final var d1 = d + 1;
                for (var p1i : p13d) {
                    final var d1i = next1.getOrDefault(p1i, Integer.MAX_VALUE);
                    if (d1 < d1i) {
                        next1.put(p1i, d1);
                    }
                }
            }
            next = next1;
        }
        return minDistance.getOrDefault(new Point3D(zz, 0), 0);
    }

    @Test
    public void solution() {
        final var maze = createMaze("day20");
        assertEquals("answer 1", 580, solution(true, maze));
        assertEquals("answer 2", 6362, solution(false, maze));
    }

    record Point(int x, int y) {
    }

    record Point3D(Point point, int level) {
    } // used in part 2

    record Maze(Set<Point> walkable, Map<String, List<Point>> doors) {
    }

    record Portal(String label, Point outer, Point inner) {
    }
}
