package aoc16.d12;

import aoc16.common.Config;
import aoc16.common.IOUtil;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

public class Day12Test {

    static Character r( String mg ) { return mg.charAt(0); }

    static Integer refValue(Map<Character,Integer> registers, String ref ) {
        final boolean isReg = ref.length() == 1 && ref.charAt(0) >= 'a' && ref.charAt(0) <= 'd';
        return isReg ? registers.getOrDefault( r(ref), 0 ) : Integer.parseInt( ref );
    }

    @Test
    public void solution() {
        if (Config.isFast()) return; // takes 8 seconds
        final var input = IOUtil.input("d12");

        var registers = new HashMap<Character,Integer>();
        execute(input, registers);
        Assert.assertEquals( "answer 1", 318117, registers.get('a').intValue() );

        registers = new HashMap<>();
        registers.put('c', 1);
        execute(input, registers);
        Assert.assertEquals( "answer 2", 9227771, registers.get('a').intValue() );
    }

    // TODO - faster is to compile into instructions instead of parse them every time
    // TODO - faster registers is simple array with index (register - 'a')
    private void execute(java.util.List<String> input, HashMap<Character, Integer> registers) {
        final var patternCpy = Pattern.compile("^cpy ([a-d0-9]+) ([a-d])$");
        final var patternInc = Pattern.compile("^inc ([a-d])$");
        final var patternDec = Pattern.compile("^dec ([a-d])$");
        final var patternJnz = Pattern.compile("^jnz ([a-d0-9]+) ([-0-9]+)$");

        for ( int i = 0; i < input.size(); ) {
            final var l = input.get(i);
            var m = patternCpy.matcher(l);
            if ( m.find() ) {
                registers.put( r( m.group(2) ), refValue(registers, m.group(1) ) );
                i++;
            } else {
                m = patternInc.matcher(l);
                if ( m.find() ) {
                    final var reg = r(m.group(1));
                    registers.put( reg, registers.getOrDefault(reg, 0) + 1 );
                    i++;
                } else {
                    m = patternDec.matcher(l);
                    if ( m.find() ) {
                        final var reg = r(m.group(1));
                        registers.put( reg, registers.getOrDefault(reg, 0) - 1 );
                        i++;
                    } else {
                        m = patternJnz.matcher(l);
                        if ( m.find() ) {
                            i += ( refValue(registers, m.group(1) ) != 0 ) ? Integer.parseInt( m.group(2) ) : 1;
                        } else Assert.fail( "Unexpected syntax: " + l );
                    }
                }
            }
        } // for
    }
}
