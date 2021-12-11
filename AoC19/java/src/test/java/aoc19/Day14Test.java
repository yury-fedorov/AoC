package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.regex.*;
import java.util.stream.*;

import static org.junit.Assert.*;

public class Day14Test {
    final static String ORE = "ORE";
    final static String FUEL = "FUEL";

    final static Pattern LINE = Pattern.compile( "^(.+) => (\\d+) (\\w+)$" );
    final static Pattern INPUT = Pattern.compile( "(\\d+) (\\w+)" );

    record Portion( double quantity, String chemical ) {}
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
        final var reactions = IOUtil.input("day14-sample");
        final var react = reactions.stream().map( Day14Test::parse ).collect(Collectors.toList());

        final var required = new ArrayList<Portion>();
        int oreQuantity = 0;
        required.add( new Portion( 1, FUEL ) );
        while ( !required.isEmpty() ) {
            var portion = required.remove(0);
            var options = react.stream().filter( r -> r.output.chemical.equals( portion.chemical ) ).toList();
            assertFalse( options.isEmpty() );
            if ( options.size() == 1 ) {
                var r = options.get(0);
                int k = (int)Math.ceil( portion.quantity / (double)r.output.quantity );  // what we need / what we have
                for (  var i : r.input ) {
                    if ( i.chemical.equals( ORE ) ) {
                        // the initial component
                        oreQuantity += k * i.quantity;
                    } else {
                        required.add( new Portion( k * i.quantity, i.chemical ) );
                    }
                }
            } else fail( "not implemented yet" );
        }
        assertEquals( "answer 1", oreQuantity, -1 ); // 331686 is too low, 421639 low, 1380560 - not right

        assertEquals( "answer 2", 0, 2 );
    }
}
