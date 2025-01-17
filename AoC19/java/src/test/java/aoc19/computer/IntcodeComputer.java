package aoc19.computer;

import aoc19.computer.operation.Factory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.stream.Collectors;

public class IntcodeComputer {

    final ArrayList<Long> _memory;
    final Queue<Long> _in;
    final Queue<Long> _out;
    int _cur = 0;
    long _relativeBase = 0;
    public IntcodeComputer(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out) {
        _memory = new ArrayList<>(memory);
        _in = in;
        _out = out;
    }

    public static ArrayList<Long> loadMemory(String code) {
        return new ArrayList<>(Arrays.stream(code.split(",")).map(Long::valueOf).collect(Collectors.toList()));
    }

    public static RunPhase run(ArrayList<Long> memory, long in, Queue<Long> out) {
        var inQueue = new LinkedBlockingQueue<Long>();
        inQueue.add(in);
        return IntcodeComputer.run(memory, inQueue, out);
    }

    public static RunPhase run(ArrayList<Long> memory, Queue<Long> in, Queue<Long> out) {
        var c = new IntcodeComputer(memory, in, out);
        return c.run();
    }

    public void in(long data) {
        _in.add(data);
    }

    public long out() {
        return _out.poll();
    }

    public int inSize() {
        return _in.size();
    }

    public int outSize() {
        return _out.size();
    }

    public long get(int index) {
        return _memory.get(index);
    }

    public ArrayList<Long> getMemory() {
        return _memory;
    }

    public RunPhase run() {
        while (true) {
            final var o = Factory.createOperation(_memory, _cur);
            if (o.command == Command.End) return RunPhase.HALT;
            if (o.command == Command.In && _in.isEmpty()) return RunPhase.NEED_FOR_INPUT;
            final var jump = o.execute(_memory, _in, _out, _relativeBase);
            _relativeBase = jump.relativeBase();
            _cur = (jump.jump() == Jump.Absolute) ? (int) jump.shift() : (_cur + o.length());
        }
    }

    public enum RunPhase {HALT, NEED_FOR_INPUT}
}
