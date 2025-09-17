package aoc16.d22;

import aoc16.common.IOUtil;
import aoc16.common.Point;
import org.junit.Assert;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.regex.Pattern;

public class Day22Test {

    static int available(Node node) {
        return node.size() - node.used();
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d22");
        // /dev/grid/node-x0-y0     85T   67T    18T   78%
        final var pattern = Pattern.compile("^/dev/grid/node-x(\\d+)-y(\\d+)\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)T\\s+(\\d+)%$");
        final var grid = new HashMap<Point, Node>();
        for (final var l : input) {
            final var m = pattern.matcher(l);
            if (m.find()) {
                grid.put(Point.with(Integer.parseInt(m.group(1)), Integer.parseInt(m.group(2))), // x, y
                        new Node(Integer.parseInt(m.group(3)), Integer.parseInt(m.group(4)))); // Size  Used
            }
        }
        Assert.assertEquals(input.size() - 2, grid.size());
        final var maxX = grid.keySet().stream().mapToInt(Point::x).max().getAsInt();
        final var maxY = grid.keySet().stream().mapToInt(Point::y).max().getAsInt();
        final var answer1 = new LinkedList<NodeToNode>(); // pair of nodes A->B
        for (final var nodeA : grid.entrySet()) {
            for (final var nodeB : grid.entrySet()) {
                if (nodeA.getKey().equals(nodeB.getKey())) continue;
                final var na = nodeA.getValue();
                final var nb = nodeB.getValue();
                if (na.used() > 0 && na.used() <= available(nb)) answer1.add(new NodeToNode(na, nb));
            }
        }
        Assert.assertEquals("answer 1", 924, answer1.size());

        final var empty = new ArrayList<Point>(); // empty node(s)
        for (int y = 0; y <= maxY; y++) {
            for (int x = 0; x <= maxX; x++) {
                final var a = Point.with(x, y);
                final var n = grid.get(a);
                final boolean isEmpty = (n.used() / (double) n.size()) < 0.1;
                if (isEmpty) empty.add(a);
            }
        }
        // XXX solved analytically looking at the map
        final var iep = empty.getFirst();
        int distance = iep.x() + iep.y() + maxX; // to arrive to initial point
        distance += 5 * (maxX - 1);
        Assert.assertEquals("answer 2", 213, distance);
    }

    record Node(int size, int used) {
    }

    record NodeToNode(Node a, Node b) {
    }
}
