package aoc19.computer;

import aoc19.computer.operation.Factory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.stream.Collectors;

public class IntcodeComputer {

    public static ArrayList<Long> loadMemory( String code ) {
        return new ArrayList<>( Arrays.stream(code.split("," )).map( Long::valueOf ).collect( Collectors.toList() ) );
    }

    public static boolean run(ArrayList<Long> memory, long in, Queue<Long> out ) {
        var inQueue = new LinkedBlockingQueue<Long>();
        inQueue.add(in);
        return IntcodeComputer.run( memory, inQueue, out );
    }

    public static boolean run(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out ) {
        int cur = 0;
        long relativeBase = 0;
        while ( cur >= 0 && cur < memory.size() )  {
            final var o = Factory.createOperation( memory, cur );
            if ( o.command == Command.End ) return true;
            final var jump = o.execute(memory, in, out, relativeBase);
            relativeBase = jump.relativeBase();
            cur = ( jump.jump() == Jump.Absolute ) ? (int)jump.shift() : ( cur + o.length() );
        }
        return false;
    }

    public IntcodeComputer(  ) {
    }

}
