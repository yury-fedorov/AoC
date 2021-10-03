package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Jump;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Queue;

public class End extends Operation {

    public End() { super(Command.End, Collections.emptyList(), Collections.emptyList() );}

    @Override
    public ExecResult execute(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, long relativeBase) {
        return new ExecResult( Jump.Absolute, 1000000, relativeBase );
    }
}
