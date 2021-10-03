package aoc19.computer;

import aoc19.computer.operation.Operation;

public interface OnEvent {
    void handler(Operation operation);
}
