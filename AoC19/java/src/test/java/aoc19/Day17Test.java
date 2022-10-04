package aoc19;

import aoc19.computer.IntcodeComputer;
import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

import static aoc19.computer.AsciiUtil.readOut;
import static org.junit.Assert.assertEquals;

public class Day17Test {
    
    @Test
    public void printDecomposition() {
      // calculated and written down by hand  
      final String path = "L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,10,R,12,R,8,L,10,R,12,R,8,R,8,R,10,R,12,L,12,L,10,R,8,L,12,R,8,R,10,R,12,L,10,R,12,R,8";
      final var pattern = Pattern.compile("^(.{1,20})\\1*(.{1,20})(?:\\1|\\2)*(.{1,20})(?:\\1|\\2|\\3)*$");
      final var matcher = pattern.matcher(path.replaceAll(",", ""));
      assertTrue( "a solution is found", matcher.find() );
      System.out.println("Found value: " + matcher.group(1)); // too long but the tail is also group 3, so it is ok
      System.out.println("Found value: " + matcher.group(2));
      System.out.println("Found value: " + matcher.group(3));
    }

    @Test
    public void solution() {
        final var  memory = IntcodeComputer.loadMemory(IOUtil.input("day17").get(0));
        final ArrayList<Long> memory2 = (ArrayList<Long>) memory.clone();
        final var in = new LinkedBlockingQueue<Long>();
        final var out = new LinkedBlockingQueue<Long>();
        final var comp = new IntcodeComputer(memory, in, out);
        comp.run();
        final var mapAsString = readOut(out).toString();
        final var map = Arrays.stream(mapAsString.split( "\n" )).map( s -> s.trim() ).toList();
        final var maxX = map.get(0).length();
        final var maxY = map.size();
        long answer1 = 0;
        for ( var y = 0; y < maxY; y++ ) {
            for ( var x = 0; x < maxX; x++ ) {
                answer1 += isIntersection(map, x, y) ? x * y : 0;
            }
        }
        assertEquals( "answer 1", 4600, answer1);

        memory2.set(0, 2l);
        in.clear();
        out.clear();
        
        //       [ A C             ][ A C             ][ B    ][ B    ][ C    ][ A C             ][ B    ]
        // Path: L12L10R8L12R8R10R12L12L10R8L12R8R10R12L10R12R8L10R12R8R8R10R12L12L10R8L12R8R10R12L10R12R8
        final var instructions = "A,C,A,C,B,B,C,A,C,B\n" +
                "L,12,L,10,R,8,L,12\n" +
                "L,10,R,12,R,8\n" +
                "R,8,R,10,R,12\n" +
                "n\n";
        
        instructions.chars().forEach( c -> in.add((long)c ));
        final var comp2 = new IntcodeComputer(memory2, in, out );
        final var phase = comp2.run();
        System.out.println(phase);
        System.out.println( readOut(out) );
        final var answer2 = 0; // TODO: last from out queue
        assertEquals( "answer 2", -2, answer2 );
    }

    static char get( List<String> map, int x, int y ) {
        if ( y < 0 || y >= map.size() ) return OPEN_SPACE;
        final var line = map.get(y);
        if ( x < 0 || x >= line.length() ) return OPEN_SPACE;
        return line.charAt(x);
    }
    static final char SCAFFOLD = '#';
    static final char OPEN_SPACE = '.';
    static boolean isIntersection( List<String> map, int x, int y ) {
        if ( get( map, x, y ) != SCAFFOLD ) return false;
        if ( get( map, x + 1, y ) != SCAFFOLD ) return false;
        if ( get( map, x - 1, y ) != SCAFFOLD ) return false;
        if ( get( map, x, y + 1 ) != SCAFFOLD ) return false;
        return get( map, x, y - 1 ) == SCAFFOLD;
    }
}
