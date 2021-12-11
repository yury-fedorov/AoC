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

    // list of chemicals used for output
    static Set<String> to( List<Receipt> react, String output ) {
        var options = react.stream().filter( r -> r.output.chemical.equals( output ) ).toList();
        if ( options.isEmpty() ) return new HashSet<String>();
        var set = options.get(0).input.stream().map( p -> p.chemical ).toList();
        var result = new HashSet<String>();
        result.addAll(set);
        for ( var c : set ) {
            result.addAll( to( react, c ) );
        }
        return result;
    }

    static boolean isPart( List<Receipt> react, String output, String part ) {
        return to( react, output ).contains( part );
    }

    static List<String> latest( List<Receipt> react, Collection<String> set ) {
        // any which does not contain any other
        // var map = new HashMap< String, Set<String> > ();
        var mapCount = new HashMap< String, Integer > ();
        for ( var c : set ) {
            var s = to( react, c );
            // map.put( c, s );
            var count = mapCount.getOrDefault( c, 0 ) + 1;
            mapCount.put( c, count );
        }
        var max = mapCount.values().stream().mapToInt( v -> v ).max().getAsInt();
        return mapCount.keySet().stream().filter( k -> mapCount.get(k) == max ).toList();
    }

    @Test
    public void solution() {
        final var reactions = IOUtil.input("day14-sample");
        final var react = reactions.stream().map( Day14Test::parse ).toList();

        final var required = new HashMap<String,Integer>();
        int oreQuantity = 0;
        required.put( FUEL, 1 );
        while ( !required.isEmpty() ) {
            var latest = latest(react, required.keySet() );
            var chemical = latest.get(0);
            int portion = required.remove( chemical );
            var options = react.stream().filter( r -> r.output.chemical.equals( chemical ) ).toList();
            assertFalse( options.isEmpty() );
            if ( options.size() == 1 ) {
                var r = options.get(0);
                int k = (int)Math.ceil( portion / (double)r.output.quantity );  // what we need / what we have
                for (  var i : r.input ) {
                    if ( i.chemical.equals( ORE ) ) {
                        // the initial component
                        oreQuantity += ( k * i.quantity );
                    } else {
                        var qty = required.getOrDefault( i.chemical, 0 ) + ( k * i.quantity );
                        required.put( i.chemical, qty );
                    }
                }
            } else fail( "not implemented yet" );
        }
        assertEquals( "answer 1", oreQuantity, -1 ); // 331686 is too low, 421639 low, 1380560 - not right

        assertEquals( "answer 2", 0, 2 );
    }
}
