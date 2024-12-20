package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Optional;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day13Test {

    final static long BLOCK = 2L;
    final static long PADDLE = 3L;
    final static long BALL = 4L;

    static Optional<Command> readOut(IntcodeComputer comp, Queue<Long> out) {
        if (out.size() >= 3) {
            var x = comp.out();
            var y = comp.out();
            var v = comp.out();
            return Optional.of(new Command(new Point(x, y), v));
        }
        return Optional.empty();
    }

    @Test
    public void solution() {
        final var memory = IntcodeComputer.loadMemory(IOUtil.input("day13").get(0));
        final var memory2 = (ArrayList<Long>) memory.clone();
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        final var map = new HashMap<Point, Long>();
        Optional<Command> command;
        while ((command = readOut(comp, out)).isPresent()) {
            var c = command.get();
            map.put(c.point, c.tileId);
        }
        assertEquals("answer 1", 309L, map.values().stream().filter(t -> t == BLOCK).count());

        memory2.set(0, 2L);
        in.clear();
        out.clear();
        comp = new IntcodeComputer(memory2, in, out);
        var ballX = 0L; // ball x position
        var paddleX = 0L; // paddle x position
        while (comp.run() != IntcodeComputer.RunPhase.HALT) {
            while ((command = readOut(comp, out)).isPresent()) {
                var c = command.get();
                if (c.tileId == BALL) ballX = c.point.x;
                else if (c.tileId == PADDLE) paddleX = c.point.x;
            }
            var dj = ballX - paddleX;
            var joystick = dj < 0 ? -1L : dj > 0 ? 1L : 0L;
            comp.in(joystick);
        }
        // final score
        final var SCORE_POINT = new Point(-1, 0);
        long score = 0L;
        while ((command = readOut(comp, out)).isPresent()) {
            var c = command.get();
            if (c.point.equals(SCORE_POINT)) score = c.tileId;
        }
        assertEquals("answer 2", 15410, score);
    }

    record Point(long x, long y) {
    }

    record Command(Point point, long tileId) {
    }
}
