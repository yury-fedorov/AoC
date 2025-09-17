package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Jump;
import aoc19.computer.Mode;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

public class InOutRel extends Operation {
    public InOutRel(Command command, List<Mode> modes, List<Long> args) {
        super(command, modes, args);
    }

    @Override
    public ExecResult execute(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, long relativeBase) {
        final var a0 = args.getFirst();
        if (command == Command.In) {
            set(memory, a0, in.poll(), modes.getFirst(), relativeBase);
        } else {
            final var a = get(memory, a0, modes.getFirst(), relativeBase);
            switch (command) {
                case Out -> out.add(a);
                case AdjustRelativeBase -> relativeBase += a;
                default -> throw new IllegalStateException("Unexpected command: " + command);
            }
        }
        return new ExecResult(Jump.Next, 0, relativeBase);
    }
}
