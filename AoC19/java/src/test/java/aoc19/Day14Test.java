package aoc19;

import org.junit.Test;

import java.util.*;
import java.util.regex.*;

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
        if ( options.isEmpty() ) return new HashSet<>();
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
        solution1("day14-sample", 31); // sample
        solution1("day14-sample2", 165); // sample
        solution1("day14", 741927);
    }

    // reduce required using rest
    static void useRest( Map<String, Long> rest, Map<String, Long> required) {
        var scope = new HashSet<>(rest.keySet());
        scope.retainAll( required.keySet() );
        for ( var chemical : scope ) {
            var restQty = rest.remove(chemical);
            var requiredQty = required.remove(chemical);
            var newRequired = requiredQty - restQty;
            if ( newRequired > 0 ) {
                // all rest is consumed to reduce required
                required.put(chemical, newRequired);
            } else if ( newRequired < 0 ) {
                // rest was big enough, no more required but some rest
                rest.put(chemical, -newRequired);
            }
        }
    }

    public void solution1(String input, int expectedOreQuantity) {
        final var reactions = IOUtil.input(input);
        final var react = reactions.stream().map( Day14Test::parse ).toList();

        final var rest = new HashMap<String,Long>();
        final var required = new HashMap<String,Long>();
        int oreQuantity = 0;
        required.put( FUEL, 1L );
        while ( !required.isEmpty() ) {
            var latest = latest(react, required.keySet() );
            var chemical = latest.get(0);
            var requiredQty = required.remove( chemical ); // how much we need
            var options = react.stream().filter( r -> r.output.chemical.equals( chemical ) ).toList();
            assertFalse( options.isEmpty() );
            if ( options.size() == 1 ) {
                var r = options.get(0);
                int k = (int)Math.ceil( requiredQty / (double)r.output.quantity );  // what we need / what we have
                int producedQty = k * r.output.quantity; // could be more then needed
                var diffQty = producedQty - requiredQty;
                if ( diffQty > 0 ) {
                    // what we do not really need from output
                    var qty = rest.getOrDefault( r.output.chemical, 0L ) + diffQty;
                    rest.put( r.output.chemical, qty );
                }
                for (  var i : r.input ) {
                    var qtyInput = ( k * i.quantity );
                    if ( i.chemical.equals( ORE ) ) {
                        // the initial component
                        oreQuantity += qtyInput;
                    } else {
                        var qty = required.getOrDefault( i.chemical, 0L ) + qtyInput;
                        required.put( i.chemical, qty );
                        useRest(rest, required);
                    }
                }
            } else fail( "not implemented yet" );
        }
        assertEquals( "answer 1", expectedOreQuantity, oreQuantity );
        // TODO assertEquals( "answer 2", 0, 2 );
    }
}
