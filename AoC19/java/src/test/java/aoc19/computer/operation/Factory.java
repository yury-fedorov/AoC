package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Mode;

import java.util.Arrays;
import java.util.List;

public class Factory {
    public static Mode getMode(int mode) {
        final var option = Arrays.stream(Mode.values())
                .filter( m -> m.code == mode ).findFirst();
        if (option.isEmpty()) throw new IllegalArgumentException( "Wrong mode: " + mode );
        return option.get();
    }

    public static Operation createOperation( List<Long> code, int position ) {
        final long modesCode = code.get(position);
        final long mcq = modesCode / 100L; // quot - modes, rem - command
        final int commandCode = (int)( modesCode % 100 );
        final int m0 = (int) ( mcq % 10 );
        final int m1 = (int) ( ( mcq /  10 ) % 10 );
        final int m2 = (int) ( ( mcq / 100 ) % 10 );
        final var modes = List.of( getMode( m0 ), getMode( m1 ), getMode( m2 ) );
        final var optCommand = Arrays.stream(Command.values()).filter( c -> c.code == commandCode ).findFirst();
        if ( optCommand.isEmpty() )
            throw new IllegalStateException( "Unknown operation code: " + commandCode );
        final Command command = optCommand.get();
        List<Long> args;
        switch ( command ) {
            case End: return new End();
            case Add:
            case Mul:
            case Equals:
            case LessThan:
                args = List.of( code.get(position +1 ), code.get(position + 2), code.get(position + 3) );
                break;
            case JumpIfTrue:
            case JumpIfFalse:
                args = List.of( code.get(position + 1), code.get(position + 2) );
                break;
            case In:
            case Out:
            case AdjustRelativeBase:
                args = List.of( code.get(position + 1) );
                return new InOutRel(command, modes, args);
            default: throw new IllegalStateException();
        }
        return new Common( command, modes, args );
    }
}
