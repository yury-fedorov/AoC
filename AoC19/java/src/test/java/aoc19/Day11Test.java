package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.HashMap;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day11Test {
    record Point( long x, long y ) {}
    enum Direction { UP, LEFT, DOWN, RIGHT }
    static final long BLACK = 0;
    static final long WHITE = 1;

    static Direction newDirection( Direction direction, long rotationCode) {
        // TODO implement
        return direction;
    }
    @Test
    public void solution() {
        final var  code = IntcodeComputer.loadMemory(IOUtil.input("day11").get(0));
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        var comp = new IntcodeComputer(code, in, out);
        var position = new Point( 0, 0 );
        var direction = Direction.UP;
        final var map = new HashMap<Point,Long>();
        in.add(BLACK);
        while ( comp.run() != IntcodeComputer.RunPhase.HALT ) {
            final var newColor = out.poll();
            final var newDirection = out.poll();
            map.put(position, newColor);
            direction = newDirection(direction, newDirection);
            position = newPosition(position, direction);
            var color = map.getOrDefault(position, BLACK);
            in.add(color);
        }
        assertEquals( "answer 1", -1L, map.size() );
        assertEquals( "answer 2", -2L, 0 );
    }

    static Point newPosition(Point position, Direction direction) {
        // TODO to implement
        return position;
    }
}
