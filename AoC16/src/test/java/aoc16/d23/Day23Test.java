package aoc16.d23;

import aoc16.common.Config;
import aoc16.common.IOUtil;
import aoc16.d12.Day12Test;
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
        public int apply(Matcher m, List<MatcherInstruction> code, int curIns, Map<Character, Integer> reg) {
            return instruction.apply(m, code, curIns, reg );
        }
    }

    @Test
    public void test() {
        final var input = IOUtil.inputByPath("d23/sample.txt");
        Assert.assertEquals( "sample", 3, execute(input, 0) );
    }

    @Test
    public void solution() {
        final var input = IOUtil.input("d23");
        Assert.assertEquals( "answer 1", 12516, execute(input, 7) );
        if (Config.isFast()) return; // 3 mins 2 seconds
        Assert.assertEquals( "answer 2", 479009076, execute(input, 12) );
    }

    private static int execute(List<String> input, int regA) {
        final var iset = new HashMap<>(initInstuctions());
        final var INC = compile(iset, "inc a").instruction();
        final var DEC = compile(iset, "dec a").instruction();
        final var CPY = compile(iset, "cpy 1 a").instruction();
        final var JNZ = compile(iset, "jnz a 1").instruction();
        final Instruction TGL = ( m, c, ci, r ) -> {
            final var reg = r(m.group(1));
            final var ii = r.getOrDefault(reg, 0) + ci;
            if ( ii < c.size() ) {
                final TogglingInstruction ti = (TogglingInstruction)c.get( ii ).instruction();
                ti.toggle();
            }
            return 1; };
        final var opposite
                = Map.of( INC, DEC, DEC, INC, CPY, JNZ, JNZ, CPY, TGL, INC );
        iset.put( Pattern.compile("^tgl ([a-d])$"), TGL );
        final var code = compile( iset, input);
        for ( int i = 0; i < code.size(); i++ ) {
            final var cl = code.get(i);
            final var op = cl.instruction();
            // we need to replace instruction
            code.set( i, new MatcherInstruction( cl.matcher(), new TogglingInstruction( op, opposite.get(op) ) ) );
        }
        var registers = new HashMap<Character,Integer>();
        registers.put('a',regA);
        execute(code, registers);
        return registers.get('a');
    }
}
