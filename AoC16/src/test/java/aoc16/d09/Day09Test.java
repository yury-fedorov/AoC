package aoc16.d09;

import aoc16.common.IOUtil;
import org.junit.Test;

import java.util.regex.Pattern;

import static org.junit.Assert.*;

public class Day09Test {

    final static Pattern patternInstruction = Pattern.compile("^(\\d+)x(\\d+)$");

    static long decodedLength( CharSequence s ) {
        long result = 0;
        boolean isInstruction = false;
        final var instruction = new StringBuilder();
        for ( int i = 0; i < s.length(); ) {
            final char ch = s.charAt(i);
            if ( ch == '(' ) {
                // starts instruction
                assertFalse("instruction == false", isInstruction);
                isInstruction = true;
                instruction.setLength(0);
                i++;
            } else if ( ch == ')' ) {
                // ends instruction
                isInstruction = false;
                final var matcher = patternInstruction.matcher( instruction );
                assertTrue( "matcher.find()", matcher.find() );
                final var length = Integer.parseInt( matcher.group(1) );
                var repeated = Integer.parseInt( matcher.group(2) );
                final var start = i + 1;
                final var end = start + length;
                final var ss = s.subSequence( start, end );
                final long sl = decodedLength( ss );
                assertEquals( "ss.length()", length, ss.length() );
                result += ( repeated * sl );
                instruction.setLength(0);
                i = end;
            } else {
                if ( isInstruction ) instruction.append(ch);
                else result++;
                i++;
            }
        }
        return result;
    }

    static CharSequence decode( CharSequence s ) {
        final var out = new StringBuilder();
        boolean isInstruction = false;
        final var instruction = new StringBuilder();
        for ( int i = 0; i < s.length(); ) {
            final char ch = s.charAt(i);
            if ( ch == '(' ) {
                // starts instruction
                assertFalse("instruction == false", isInstruction);
                isInstruction = true;
                instruction.setLength(0);
                i++;
            } else if ( ch == ')' ) {
                // ends instruction
                isInstruction = false;
                final var matcher = patternInstruction.matcher( instruction );
                assertTrue( "matcher.find()", matcher.find() );
                final var length = Integer.parseInt( matcher.group(1) );
                var repeated = Integer.parseInt( matcher.group(2) );
                final var start = i + 1;
                final var end = start + length;
                final var ss = s.subSequence( start, end );
                assertEquals( "ss.length()", length, ss.length() );
                for ( ; repeated > 0; repeated-- ) {
                    out.append( ss );
                }
                instruction.setLength(0);
                i = end;
            } else {
                ( isInstruction ? instruction : out ).append(ch);
                i++;
            }
        }
        return out;
    }

    @Test
    public void test() {
        assertEquals( "ADVENT", decode("ADVENT").toString() );
        assertEquals( "ABBBBBC", decode("A(1x5)BC").toString() );
        assertEquals( "XYZXYZXYZ", decode("(3x3)XYZ").toString() );
        assertEquals( 9, decodedLength( "(3x3)XYZ" ) );
        assertEquals( "XABCABCABCABCABCABCY".length(), decodedLength("X(8x2)(3x3)ABCY" ) );
    }

    @Test
    public void solution()  {
        final var input = IOUtil.input("d09");
        int answer1 = 0;
        long answer2 = 0;
        for ( final var l : input ) {
            answer1 += decode( l ).length();
            answer2 += decodedLength(l);
            assertTrue( "answer2 > 0", answer2 > 0 );
        }
        assertEquals("answer 1", 150914, answer1 );
        assertEquals("answer 2", 11052855125L, answer2 ); // too low - 501035
    }
}