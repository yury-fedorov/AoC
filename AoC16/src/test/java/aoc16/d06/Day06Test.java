package aoc16.d06;

import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import static org.junit.Assert.assertEquals;

public class Day06Test {
    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d06/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );
        final var stats = new ArrayList<Map<Character,Integer>>();
        final var n = input.get(0).length();
        for ( int i = 0; i < n; i++ ) stats.add( new HashMap<Character,Integer>() );
        for ( final var l : input ) {
            for ( int i = 0; i < l.length(); i++ ) {
                final var ch = l. charAt(i);
                final var isNew = i >= stats.size();
                final var m = stats.get(i);
                m.put( ch, 1 + m.getOrDefault( ch, 0 ) );
            }
        }
        final var answer1 = new StringBuilder();
        final var answer2 = new StringBuilder();
        for ( int i = 0; i < n; i++ ) {
            final var m = stats.get(i);
            final var max = m.values().stream().max(Integer::compare).get();
            final var min = m.values().stream().min(Integer::compare).get();
            for ( final var e : m.entrySet() ) {
                if ( max == e.getValue() ) {
                    answer1.append( e.getKey() );
                }
                if ( min == e.getValue() ) {
                    answer2.append( e.getKey() );
                }
            }
        }
        assertEquals( "answer 1", "umejzgdw", answer1.toString() );
        assertEquals( "answer 2", "aovueakv", answer2.toString() );
    }
}
