package aoc19.computer;

public enum Command {
    Add(1),
    Mul(2),
    In(3),
    Out(4),
    JumpIfTrue(5),
    JumpIfFalse(6),
    LessThan(7),
    Equals( 8),
    AdjustRelativeBase(9),
    End(99);

    public final int code;
    private Command(int code) { this.code = code; }
}
