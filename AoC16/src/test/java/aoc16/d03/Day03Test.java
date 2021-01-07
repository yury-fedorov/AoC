package aoc16.d03;

import aoc16.common.IOUtil;
import org.junit.Test;

import java.util.ArrayList;

import static org.junit.Assert.assertEquals;

public class Day03Test {

    public static boolean isTriangle( int a, int b, int c ) {
        return ( ( a + b ) > c ) && ( ( a + c ) > b ) && ( ( b + c ) > a );
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d03");
        final var list1 = new ArrayList<Integer>();
        final var list2 = new ArrayList<Integer>();
        final var list3 = new ArrayList<Integer>();
        int answer1 = 0;
        for ( final var l : input ) {
            final var a = l.split( "\\s+" );
            final var a1 = Integer.parseInt(a[1].trim() );
            final var a2 = Integer.parseInt(a[2].trim() );
            final var a3 = Integer.parseInt(a[3].trim() );
            answer1 += isTriangle( a1, a2, a3 ) ? 1 : 0;
            list1.add(a1);
            list2.add(a2);
            list3.add(a3);
        }
        assertEquals("answer 1", 1050, answer1 );
        int answer2 = 0;
        for ( int i = 0; i < list1.size(); i += 3 ) {
            answer2 += isTriangle( list1.get(i), list1.get(i+1), list1.get(i+2) ) ? 1 : 0;
            answer2 += isTriangle( list2.get(i), list2.get(i+1), list2.get(i+2) ) ? 1 : 0;
            answer2 += isTriangle( list3.get(i), list3.get(i+1), list3.get(i+2) ) ? 1 : 0;
        }
        assertEquals("answer 2", 1921, answer2 );
    }
}
