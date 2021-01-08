package aoc16.d17;

import aoc16.common.Md5Util;
import org.junit.Assert;
import org.junit.Test;

public class Day17Test {

    enum Direction { UP, DOWN, LEFT, RIGHT }

    static boolean isOpen( char code ) { return code >= 'b' && code <= 'f'; }

    static String md5( String text ) { return new Md5Util().md5(text).toLowerCase(); }

    static String shortestPath( String passcode ) { return ""; }

    @Test
    public void test() {
        Assert.assertEquals( "DDRRRD", shortestPath("ihgpwlah") );
        Assert.assertEquals( "DDUDRLRRUDRD", shortestPath("kglvqrro") );
        Assert.assertEquals( "DRURDRUDDLLDLUURRDULRLDUUDDDRR", shortestPath("ulqzkmiv") );
    }

    @Test
    public void solution() {
        final var INPUT = "gdjjyniy";
        Assert.assertEquals( "answer 1", null, shortestPath( INPUT ) );
    }
}
