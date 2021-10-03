package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Jump;
import aoc19.computer.Mode;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

public class InOutRel extends Operation {
    public InOutRel(Command command, List<Mode> modes, List<Long> args ) { super(command, modes, args ); }

    @Override
    public ExecResult execute(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, long relativeBase) {
        final var a = get(memory, args.get(0), modes.get(0), relativeBase);
        switch ( command ) {
            case In: set(memory, a, in.poll() ); break;
            case Out: out.add(a); break;
            case AdjustRelativeBase: relativeBase += a;
            default: throw new IllegalStateException("Unexpected command: " + command);
        }
        return new ExecResult( Jump.Next, 0, relativeBase );
    }
}
