package aoc19.computer;

import aoc19.computer.operation.Factory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.stream.Collectors;

public class IntcodeComputer {

    public static List<Long> loadMemory( String code ) {
        return Arrays.stream(code.split("," )).map( Long::valueOf ).collect( Collectors.toList() );
    }

    public static boolean run(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, OnEvent onExecuted ) {
        int cur = 0;
        long relativeBase = 0;
        while ( cur >= 0 && cur < memory.size() )  {
            final var o = Factory.createOperation( memory, cur );
            if ( o.command == Command.End ) return true;
            final var jump = o.execute(memory, in, out, relativeBase);
            onExecuted.handler(o);
            relativeBase = jump.relativeBase();
            cur = ( jump.jump() == Jump.Absolute ) ? (int)jump.shift() : ( cur + o.length() );
        }
        return false;
    }
}
