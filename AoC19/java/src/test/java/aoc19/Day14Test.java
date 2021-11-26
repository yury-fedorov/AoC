package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.regex.*;
import java.util.stream.*;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class Day14Test {
    final static String ORE = "ORE";
    final static String FUEL = "FUEL";

    final static Pattern LINE = Pattern.compile( "^(.+) => (\\d+) (\\w+)$" );
    final static Pattern INPUT = Pattern.compile( "(\\d+) (\\w+)" );

    record Portion( int quantity, String chemical ) {}
    record Receipt( List<Portion> input, Portion output ) {}

    static Receipt parse( String line ) {
        // 1 ORE => 1 B
        // 7 A, 1 E => 1 FUEL
        final var ml = LINE.matcher(line);
        if ( !ml.find() ) throw new RuntimeException( "bad line" );
        final var i = ml.group(1);
        final var oq = Integer.parseInt( ml.group(2) );
        final var oc = ml.group(3);
        
        final var mi = INPUT.matcher(i);
        final var input = new ArrayList<Portion>();
        while ( mi.find() ) {
            final var q = Integer.parseInt( mi.group(1) );
            final var c = mi.group(2);
            input.add( new Portion( q, c ) );
        }
        return new Receipt( input, new Portion(oq, oc) );
    }

    @Test
    public void solution() {
        final var reactions = IOUtil.input("day14");
        final var react = reactions.stream().map( Day14Test::parse ).collect(Collectors.toList());
        assertEquals( "answer 1", 0, 1);
        assertEquals( "answer 2", 0, 2 );
    }
}
