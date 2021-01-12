package aoc16.d23;

import aoc16.common.IOUtil;
import aoc16.d12.Day12Test;
import org.junit.Assert;
import org.junit.Test;

import java.util.HashMap;

public class Day23Test extends Day12Test {

    @Test
    public void solution() {
        final var input = IOUtil.input("d23");
        final var code = compile( initInstuctions(), input );
        var registers = new HashMap<Character,Integer>();
        execute(code, registers);
        Assert.assertEquals( "answer 1", -1, registers.get('a').intValue() );
    }
}
