package aoc16.d14;

import aoc16.common.Config;
import aoc16.common.Md5Util;
import org.junit.Assert;
import org.junit.Test;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

public class Day14Test {

    final static Map<String,String> cache = new ConcurrentHashMap<>();

    static String key( String salt, int index ) { return salt + index; }

    private static String hash( String key) {
        var result = cache.getOrDefault(key, null );
        if ( result == null ) {
            result = new Md5Util().md5(key).toLowerCase(); // it is slow
            cache.put(key, result);
        }
        return result;
    }

    static String hash2( String key ) {
        var result = cache.getOrDefault(key, null );
        if ( result == null ) {
            var h = key;
            for ( int i = 0; i < 2017; i++ ) {
                h = new Md5Util().md5(h).toLowerCase();
            }
            result = h;
            cache.put(key, result);
            System.out.println( key + " -> " + result );
        }
        return result;
    }

    static Optional<Character> matchInRow( String text, int n ) {
        final int l = text.length();
        final int k = l - n;
        for ( int i = 0; i <= k; i++ ) {
            final char ch = text.charAt(i);
            var isMatching = true;
            for ( int j = 1; j < n; j++ ) {
                if ( ch != text.charAt(i+j) ) {
                    isMatching = false;
                    break;
                }
            }
            if ( isMatching ) return Optional.of(ch);
        }
        return Optional.empty();
    }

    @Test
    public void testMatchInRow() {
        final var ONE = Optional.of('1');
        Assert.assertEquals( ONE, matchInRow( "112", 2 ) );
        Assert.assertEquals( Optional.empty(), matchInRow( "112", 3 ) );
        Assert.assertEquals( ONE, matchInRow( "111", 3 ) );
        Assert.assertEquals( ONE, matchInRow( "1112", 3 ) );
        Assert.assertEquals( ONE, matchInRow( "2111", 3 ) );
        Assert.assertEquals( ONE, matchInRow( "21112", 3 ) );
        Assert.assertEquals( ONE, matchInRow( "2111222333", 3 ) );
    }

    @Test // very slow (almost 12 minutes)
    public void solution() throws InterruptedException {
        if (Config.isFast() ) return;
        final Function<String, String> fh1 = (s) -> hash(s);
        final Function<String, String> fh2 =  (s) -> hash2(s);

        Assert.assertEquals( "test 1",  22728, solution( "abc", fh1 ) );

        final var SALT = "zpqevtbw";
        Assert.assertEquals( "answer 1",  16106, solution( SALT, fh1 ) );
        Assert.assertEquals( "answer 2",  22423, solution( SALT, fh2 ) );
    }

    int solution( String salt, Function<String,String> fh ) throws InterruptedException {
        cache.clear();
        final var executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors() );
        for ( int i = 0; i < 30000; i++ ) {
            final var key = key(salt, i);
            executor.execute( () -> fh.apply( key ) );
        }
        final var result = generate(salt, fh).keySet().stream().max( Integer::compare ).get().intValue();
        executor.shutdownNow();
        executor.awaitTermination(10, TimeUnit.SECONDS );
        return result;
    }

    private HashMap<Integer, String> generate(String salt, Function<String,String> fh ) {
        final var map = new HashMap<Integer,String>();
        for ( int i = 0; map.size() < 64; i++ ) {
            final var h = fh.apply( key(salt, i ) );
            var ch = matchInRow( h, 3 );
            if ( ch.isPresent() ) {
                final String pattern = String.join("", Collections.nCopies( 5, ch.get().toString() ) );
                for ( int j = 1; j <= 1000; j++ ) {
                    if ( fh.apply( key(salt, i + j ) ).indexOf( pattern ) >= 0 ) {
                        map.put( i, h );
                        break;
                    }
                }
            }
        }
        return map;
    }
}
