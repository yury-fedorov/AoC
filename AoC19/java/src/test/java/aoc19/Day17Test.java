package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Pattern;

import static aoc19.computer.AsciiUtil.readOut;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day17Test {

    static final char SCAFFOLD = '#';
    static final char OPEN_SPACE = '.';

    static char get(List<String> map, int x, int y) {
        if (y < 0 || y >= map.size()) return OPEN_SPACE;
        final var line = map.get(y);
        if (x < 0 || x >= line.length()) return OPEN_SPACE;
        return line.charAt(x);
    }

    static boolean isIntersection(List<String> map, int x, int y) {
        if (get(map, x, y) != SCAFFOLD) return false;
        if (get(map, x + 1, y) != SCAFFOLD) return false;
        if (get(map, x - 1, y) != SCAFFOLD) return false;
        if (get(map, x, y + 1) != SCAFFOLD) return false;
        return get(map, x, y - 1) == SCAFFOLD;
    }

    @Test
    public void printDecomposition() {
        // calculated and written down by hand
        final String path = "L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,10,R,12,R,8,L,10,R,12,R,8,R,8,R,10,R,12,L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,10,R,12,R,8";
        final var pattern = Pattern.compile("^(.{1,20})\\1*(.{1,20})(?:\\1|\\2)*(.{1,20})(?:\\1|\\2|\\3)*$");
        final var matcher = pattern.matcher(path.replaceAll(",", ""));
        assertTrue("a solution is found", matcher.find());
        System.out.println("Found value: " + matcher.group(1)); // too long but the tail is also group 3, so it is ok
        System.out.println("Found value: " + matcher.group(2));
        System.out.println("Found value: " + matcher.group(3));
    }

    @Test
    public void solution() {
        final var memory = IntcodeComputer.loadMemory(IOUtil.input("day17").getFirst());
        final var memory2 = new ArrayList<>(memory);
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        final var mapAsString = readOut(out).toString();
        final var map = Arrays.stream(mapAsString.split("\n")).map(String::trim).toList();
        final var maxX = map.getFirst().length();
        final var maxY = map.size();
        long answer1 = 0;
        for (var y = 0; y < maxY; y++) {
            for (var x = 0; x < maxX; x++) {
                answer1 += isIntersection(map, x, y) ? x * y : 0;
            }
        }
        assertEquals("answer 1", 4600, answer1);

        memory2.set(0, 2L);
        in.clear();
        out.clear();

        //       [ A C             ][ A C             ][ B    ][ B    ][ C    ][ A C             ][ B    ]
        // Path: L12L10R8L12R8R10R12L12L10R8L12R8R10R12L10R12R8L10R12R8R8R10R12L12L10R8L12R8R10R12L10R12R8
        final var instructions = """
                A,C,A,C,B,B,C,A,C,B
                L,12,L,10,R,8,L,12
                L,10,R,12,R,8
                R,8,R,10,R,12
                n
                """;

        instructions.chars().forEach(c -> in.add((long) c));
        final var comp2 = new IntcodeComputer(memory2, in, out);
        comp2.run();
        final var outDeque = new ArrayDeque<>(out);
        final var answer2 = outDeque.pollLast();
        assertEquals("answer 2", 1113411L, answer2.longValue());
    }
}
