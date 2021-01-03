package aoc16.d10;

import org.javatuples.Pair;
import org.junit.Test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.function.Function;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Day10Test {

    enum Destination { BOT, OUTPUT };
    enum Source { BOT, VALUE };

    public static class BotMap extends HashMap<Integer,Pair<Pair<Destination, Integer>, Pair<Destination,Integer>>> {}
    public static class ValueMap extends HashMap<Integer,Integer> {}

    static Pair<Destination,Integer> out( String d, String n ) {
        return Pair.with( d.equals( "bot" ) ? Destination.BOT : Destination.OUTPUT, Integer.parseInt(n) );
    }

    static Collection< Pair<Source, Integer> > sources( ValueMap vmap, BotMap bmap, final int bot ) {
        final var result = new ArrayList<Pair<Source,Integer>>();
        result.addAll( vmap.entrySet().stream().filter( e -> e.getValue() == bot )
                .map( e -> Pair.with( Source.VALUE, e.getKey() )).collect(Collectors.toList()) );
        result.addAll( bmap.entrySet().stream()
                .filter( e -> ( e.getValue().getValue0().getValue0() == Destination.BOT && e.getValue().getValue0().getValue1() == bot )
                           || ( e.getValue().getValue1().getValue0() == Destination.BOT && e.getValue().getValue1().getValue1() == bot ) )
                .map( e -> Pair.with( Source.BOT, e.getKey() )).collect(Collectors.toList()) );
        return result;
    }

    final HashMap<Integer, Pair<Integer,Integer> > botResultMap = new HashMap<>();

    Pair<Integer, Integer> botOut( ValueMap vmap, BotMap bmap, final int botId ) {
        if ( botResultMap.containsKey( botId ) ) return botResultMap.get(botId);
        final var sources = sources( vmap, bmap, botId );
        assertEquals( "sources.size() == 2", 2, sources.size() );
        final var outValues = new ArrayList<Integer>();
        for ( final var s : sources ) {
            if ( s.getValue0() == Source.VALUE ) outValues.add( s.getValue1() );
            else {
                final var sourceBotId = s.getValue1();
                final var sourceBotOut = botOut( vmap, bmap, sourceBotId );
                final var setup = bmap.get( sourceBotId );
                if ( setup.getValue0().getValue0() == Destination.BOT && setup.getValue0().getValue1() == botId ) {
                    outValues.add( sourceBotOut.getValue0() );
                } else { // eventual check
                    outValues.add( sourceBotOut.getValue1() );
                }
            }
        }
        var result = ( outValues.get(0) < outValues.get(1) )
                ? Pair.with( outValues.get(0), outValues.get(1) ) : Pair.with( outValues.get(1), outValues.get(0) );
        botResultMap.put( botId, result );
        return result;
    }

    @Test
    public void solution() throws IOException {
        final var url = getClass().getClassLoader().getResource( "d10/input.txt" );
        final var input = Files.readAllLines( Path.of(url.getPath()) );

        final var patternValue = Pattern.compile("^value (\\d+) goes to bot (\\d+)$");
        final var patternBot = Pattern.compile("^bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)$");

        final var valueBotMap = new ValueMap();
        final var botOutMap = new BotMap();
        for ( final var l : input ) {
            var matcher = patternValue.matcher(l);
            if ( matcher.find() ) {
                valueBotMap.put( Integer.parseInt( matcher.group(1) ), Integer.parseInt( matcher.group(2) ) );
            } else {
                matcher = patternBot.matcher(l);
                assertTrue( "last pattern", matcher.find() );
                botOutMap.put( Integer.parseInt( matcher.group(1) ),
                        Pair.with( out( matcher.group(2), matcher.group(3) ),
                                   out( matcher.group(4), matcher.group(5) ) ) );
            }
        }
        final var ba = valueBotMap.get( 61 );
        final var bb = valueBotMap.get( 17 );
        final var bl = new HashSet<Integer>();
        bl.add(ba);
        bl.add(bb);
        Optional<Integer> answer1 = Optional.empty();
        final var processed = new HashSet<Integer>();
        while ( !bl.isEmpty() ) {
            final var curBot = bl.iterator().next();
            final var bo = botOut( valueBotMap, botOutMap, curBot );
            if ( bo.getValue0() == 17 && bo.getValue1() == 61 ) {
                answer1 = Optional.of( curBot );
                break;
            }
            bl.remove(curBot);
            processed.add(curBot); // works also without it but in this way it is faster
            final var conf = botOutMap.get( curBot );
            if ( conf.getValue0().getValue0() == Destination.BOT ) {
                final var id = conf.getValue0().getValue1();
                if ( !processed.contains( id )) bl.add( id );
            }
            if ( conf.getValue1().getValue0() == Destination.BOT ) {
                final var id = conf.getValue1().getValue1();
                if ( !processed.contains( id )) bl.add( id );
            }
        }
        assertEquals( "answer 1", 118, answer1.get().intValue() );

        long answer2 = 1;
        final Function< Pair<Destination,Integer>, Boolean> isOutput = ( p ) -> {
            return p.getValue0() == Destination.OUTPUT && p.getValue1() >= 0 && p.getValue1() <= 2;
        };
        for ( final var e : botOutMap.entrySet() ) {
            final var pp = e.getValue();
            if ( isOutput.apply( pp.getValue0() )  ) {
                answer2 *= botOut( valueBotMap, botOutMap, e.getKey() ).getValue0();
            }
            if ( isOutput.apply( pp.getValue1() )  ) {
                answer2 *= botOut( valueBotMap, botOutMap, e.getKey() ).getValue1();
            }
        }
         assertEquals( "answer 2", 143153, answer2 );
    }
}
