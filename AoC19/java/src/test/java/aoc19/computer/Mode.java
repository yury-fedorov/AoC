package aoc19.computer;

public enum Mode {
    Position(0),
    Immediate(1),
    Relative(2);
    public int code;

    private Mode(int code) {
        this.code = code;
    }
}