package aoc19.computer.operation;

import aoc19.computer.Command;
import aoc19.computer.Jump;
import aoc19.computer.Mode;

import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

import static aoc19.computer.Mode.Relative;

public abstract class Operation {
    public record ExecResult (Jump jump, long shift, long relativeBase ) {}
    public final Command command;
    final List<Mode> modes;
    final List<Long> args;
    protected Operation( Command command, List<Mode> modes, List<Long> args ) {
        this.command =  command; this.modes = modes; this.args = args;
    }
    public abstract ExecResult execute(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out, long relativeBase);
    public int length() { return 1 + args.size(); }

    public static long get( List<Long> memory, long index, Mode mode, long relativeBase ) {
        switch (mode) {
            case Position:  return index >= memory.size() ? 0 : memory.get( (int)index );
            case Immediate: return index;
            case Relative:  {
                index = relativeBase + index;
                return index >= memory.size() ? 0 : memory.get( (int)index );
            }
            default: throw new IllegalStateException( "unmanaged mode" );
        }
    }

    public static void set(ArrayList<Long> memory, long index, long value, Mode mode, long relativeBase ) {
        if ( mode == Relative ) index += relativeBase;
        if ( memory.size() <= index ) {
            final int newSize = (int)index + 1;
            memory.ensureCapacity(newSize);
            while ( memory.size() < newSize ) memory.add(0L);
        }
        memory.set( (int)index, value );
    }
}
