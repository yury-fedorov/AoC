package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.LinkedBlockingQueue;

import static org.junit.Assert.assertEquals;

public class Day11Test {
    record Point( long x, long y ) {}
    enum Direction { UP, RIGHT, DOWN, LEFT }
    static final long BLACK = 0;
    static final long WHITE = 1;

    static Direction newDirection( Direction direction, long rotationCode) {
        final var directions = Direction.values();
        final var index = Arrays.binarySearch(directions, direction);
        final int dIndex = rotationCode == 0L ? directions.length -1 : rotationCode == 1L ? 1 : 0;
        if ( dIndex == 0 ) throw new IllegalStateException( "wrong rotationCode = " + rotationCode );
        return directions[ ( index + dIndex ) % directions.length ];
    }

    static Map<Point,Long> solution( long startColor ) {
        final var  code = IntcodeComputer.loadMemory(IOUtil.input("day11").get(0));
        var in = new LinkedBlockingQueue<Long>();
        var out = new LinkedBlockingQueue<Long>();
        var comp = new IntcodeComputer(code, in, out);
        var position = new Point( 0, 0 );
        var direction = Direction.UP;
        final var map = new HashMap<Point,Long>();
        in.add(startColor);
        while ( comp.run() != IntcodeComputer.RunPhase.HALT ) {
            final var newColor = out.poll();
            final var newDirection = out.poll();
            map.put(position, newColor);
            direction = newDirection(direction, newDirection);
            position = newPosition(position, direction);
            var color = map.getOrDefault(position, BLACK);
            in.add(color);
        }
        return map;
    }

    @Test
    public void solution() {
        assertEquals( "answer 1", 2392, solution(BLACK).size() );

        // answer 2
        final var a2 = solution(WHITE);
        final var white = a2.entrySet().stream().filter( e -> e.getValue() == WHITE )
                .map( e -> e.getKey() ).toList();
        final var minX = white.stream().mapToLong(p -> p.x).min().getAsLong();
        final var maxX = white.stream().mapToLong(p -> p.x).max().getAsLong();
        final var minY = white.stream().mapToLong(p -> p.y).min().getAsLong();
        final var maxY = white.stream().mapToLong(p -> p.y).max().getAsLong();
        final var text = new StringBuilder();
        for ( var y = minY; y <= maxY; y++ ) {
            for ( var x = minX; x <= maxX; x++ ) {
                final var isWhite = white.contains( new Point(x,y) );
                text.append( isWhite ? '*' : ' ' );
            }
            text.append( System.lineSeparator() );
        }
        System.out.println(text); // EGBHLEUE
    }

    static Point newPosition(Point position, Direction direction) {
        final var dx = direction == Direction.LEFT ? -1 : direction == Direction.RIGHT ? 1 : 0;
        final var dy = direction == Direction.UP ? -1 : direction == Direction.DOWN ? 1 : 0;
        return new Point(position.x + dx, position.y + dy );
    }
}
