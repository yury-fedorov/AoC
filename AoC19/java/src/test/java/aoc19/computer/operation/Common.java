package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Jump;
import aoc19.computer.Mode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Queue;

public class Common extends Operation{
    public Common(Command command, List<Mode> modes, List<Long> args ) { super(command, modes, args ); }

    @Override
    public ExecResult execute(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, long relativeBase) {
        final var a = get(memory, args.get(0), modes.get(0), relativeBase);
        final var b = get(memory, args.get(1), modes.get(1), relativeBase);
        Optional<Long> r = Optional.empty();
        Optional<Boolean> isJump = Optional.empty();
        switch( command ) {
            case Add: r = Optional.of( a + b ); break;
            case Mul: r = Optional.of( a * b ); break;
            case Equals: r = Optional.of( a == b ? 1l : 0 ); break;
            case LessThan: r = Optional.of( a < b ? 1l : 0 ); break;
            case JumpIfFalse: isJump = Optional.of( a == 0 ); break;
            case JumpIfTrue: isJump = Optional.of( a != 0 ); break;
            default: assert(false); // not expected
        }
        if ( r.isPresent() ) {
            final var ra = args.get(2);
            set(memory, ra, r.get(), modes.get(2), relativeBase);
            return new ExecResult( Jump.Next, 0, relativeBase );
        } else if (  isJump.isPresent() ) {
            return new ExecResult( isJump.get() ? Jump.Absolute : Jump.Next, b, relativeBase );
        }
        assert(false);
        return null;
    }
}
