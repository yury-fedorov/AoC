package aoc16.d12;

import aoc16.common.IOUtil;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Day12Test {

    public static Character r( String mg ) { return mg.charAt(0); }

    public static Integer refValue(Map<Character,Integer> registers, String ref ) {
        final boolean isReg = ref.length() == 1 && ref.charAt(0) >= 'a' && ref.charAt(0) <= 'd';
        return isReg ? registers.getOrDefault( r(ref), 0 ) : Integer.parseInt( ref );
    }

    @FunctionalInterface
    public interface Instruction {
        int apply(Matcher m, List<Pair<Matcher,Instruction>> code, int curIns, Map<Character,Integer> registers );
    }

    public static Map<Pattern, Instruction> initInstuctions() {
        return Map.of(
            Pattern.compile("^cpy ([-a-d0-9]+) ([a-d])$"),
            ( m, c, ci, r ) -> { r.put( r( m.group(2) ), refValue(r, m.group(1) ) ); return 1; },
            Pattern.compile("^inc ([a-d])$"),
            ( m, c, ci, r ) -> { final var reg = r(m.group(1));
                r.put( reg, r.getOrDefault(reg, 0) + 1 ); return 1; },
            Pattern.compile("^dec ([a-d])$"),
            ( m, c, ci, r ) -> { final var reg = r(m.group(1));
                r.put( reg, r.getOrDefault(reg, 0) -1 ); return 1; },
            Pattern.compile("^jnz ([a-d0-9]+) ([-a-d0-9]+)$"),
            ( m, c, ci, r ) -> ( ( refValue(r, m.group(1) ) != 0 ) ? refValue(r, m.group(2) ) : 1 )
        );
    }

    public static Pair<Matcher,Instruction> compile( Map<Pattern, Instruction> instuctionSet, String line ) {
        final var o = instuctionSet.entrySet().stream()
                .map( (p) -> Pair.with( p.getKey().matcher( line ), p.getValue() ) )
                .filter( (p) -> p.getValue0().find() )
                .findAny();
        Assert.assertTrue( "Bad syntax: " + line, o.isPresent() );
        return o.get();
    }

    public static List<Pair<Matcher,Instruction>> compile( Map<Pattern, Instruction> instuctionSet, List<String> input ) {
        return input.stream().map( (l) -> compile( instuctionSet, l ) ).collect(Collectors.toList());
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d12");
        final var code = compile( initInstuctions(), input );

        var registers = new HashMap<Character,Integer>();
        Assert.assertEquals( "answer 1", 318117, execute(code, registers) );

        registers = new HashMap<>();
        registers.put('c', 1);
        Assert.assertEquals( "answer 2", 9227771, execute(code, registers) );
    }

    // TODO - faster registers is simple array with index (register - 'a')
    public static int  execute(List<Pair<Matcher,Instruction>> code, HashMap<Character, Integer> registers) {
        for ( int i = 0; i < code.size(); ) {
            final var ci = code.get(i);
            i += ci.getValue1().apply( ci.getValue0(), code, i, registers );
        }
        return registers.get('a');
    }
}
