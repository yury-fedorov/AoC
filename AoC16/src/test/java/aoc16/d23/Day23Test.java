package aoc16.d23;

import aoc16.common.IOUtil;
import aoc16.d12.Day12Test;
import org.javatuples.Pair;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Day23Test extends Day12Test {

    public static class TogglingInstruction implements Instruction {

        Instruction instruction;
        Instruction other;

        public TogglingInstruction( Instruction instruction, Instruction other ) {
            this.instruction = instruction;
            this.other = other;
        }

        public void toggle() {
            var temp = instruction;
            instruction = other;
            other = temp;
        }

        @Override
        public int apply(Matcher m, List<Pair<Matcher, Instruction>> code, int curIns, Map<Character, Integer> reg) {
            return instruction.apply(m, code, curIns, reg );
        }
    }

    public void test() {
        final var input = IOUtil.inputByPath("d23/sample.txt");
        Assert.assertEquals( "sample", 3, execute(input) );
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d23");
        Assert.assertEquals( "answer 1", -1, execute(input) );
    }

    private static int execute(List<String> input) {
        final var iset = new HashMap<>(initInstuctions());
        final var INC = compile(iset, "inc a").getValue1();
        final var DEC = compile(iset, "dec a").getValue1();
        final var CPY = compile(iset, "cpy 1 a").getValue1();
        final var JNZ = compile(iset, "jnz a 1").getValue1();
        final Instruction TGL = ( m, c, ci, r ) -> {
            final var reg = r(m.group(1));
            final TogglingInstruction ti = (TogglingInstruction)c.get( r.getOrDefault(reg, 0) + ci ).getValue1();
            ti.toggle();
            return 1; };
        final var opposite
                = Map.of( INC, DEC, DEC, INC, CPY, JNZ, JNZ, CPY, TGL, INC );
        iset.put( Pattern.compile("^tgl ([a-d])$"), TGL );
        final var code = compile( iset, input);
        for ( int i = 0; i < code.size(); i++ ) {
            final var cl = code.get(i);
            final var op = cl.getValue1();
            // we need to replace instruction
            code.set( i, Pair.with( cl.getValue0(), new TogglingInstruction( op, opposite.get(op) ) ) );
        }
        var registers = new HashMap<Character,Integer>();
        execute(code, registers);
        return registers.get('a').intValue();
    }
}
