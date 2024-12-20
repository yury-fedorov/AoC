import common as c
import unittest


class Computer:
    registers: {int: int}  # Register - integer
    instruction_pointer: int
    program: [int]
    output: [int]

    def __init__(self, registers, program):
        self.registers = registers
        self.program = program
        self.instruction_pointer = 0
        self.output = []

    def a(self) -> int:
        return self.registers[0]

    def b(self) -> int:
        return self.registers[1]

    def c(self) -> int:
        return self.registers[2]

    def set_a(self, value: int):
        self.registers[0] = value

    def set_b(self, value: int):
        self.registers[1] = value

    def set_c(self, value: int):
        self.registers[2] = value


def _init(data: [str]) -> Computer:
    registers = dict({})
    ri = 0
    for l in data[:3]:
        registers[ri] = int(l.split(": ")[1])
        ri += 1
    program = [int(i) for i in data[4].split(": ")[1].split(",")]
    return Computer(registers, program)


def _run(computer: Computer) -> [int]:
    code = computer.program
    while computer.instruction_pointer < len(code):
        is_not_jump = True
        operand = code[computer.instruction_pointer + 1]
        match code[computer.instruction_pointer]:
            case 0:
                computer.set_a(_adv(computer, operand))
            case 1:
                computer.set_b(_bxl(computer, operand))
            case 2:
                computer.set_b(_bst(computer, operand))
            case 3:
                jump = _jnz(computer, operand)
                if jump is not None:
                    is_not_jump = False
                    computer.instruction_pointer = jump
            case 4:
                computer.set_b(_bxc(computer, operand))
            case 5:
                _out(computer, operand)
            case 6:
                computer.set_b(_adv(computer, operand))
            case 7:
                computer.set_c(_adv(computer, operand))
        if is_not_jump:
            computer.instruction_pointer += 2
    return computer.output


def _combo_operand(computer: Computer, operand: int) -> int:
    if operand <= 3:
        return operand

    match operand:
        case 4:
            return computer.a()
        case 5:
            return computer.b()
        case 6:
            return computer.c()
    raise ValueError(f"Bad operand: {operand}")


def _adv(computer: Computer, operand: int) -> int:
    return int(computer.a() / (2 ** _combo_operand(computer, operand)))


def _bxl(computer: Computer, operand: int) -> int:
    # https://en.wikipedia.org/wiki/Bitwise_operation#XOR
    # https://www.geeksforgeeks.org/python-bitwise-operators/
    return computer.b() ^ operand


def _bst(computer: Computer, operand: int) -> int:
    return _combo_operand(computer, operand) % 8


def _jnz(computer: Computer, operand: int) -> int | None:
    return None if computer.a() == 0 else operand


def _bxc(computer: Computer, _: int) -> int | None:
    return computer.b() ^ computer.c()


def _out(computer: Computer, operand: int) -> int:
    result = _combo_operand(computer, operand) % 8
    computer.output.append(result)
    return result


def _output_to_str(output: [int]) -> str:
    return ",".join([str(i) for i in output])


def _answer1(computer: Computer) -> str:
    return _output_to_str(_run(computer))


def _run2(a: int, code: [int]) -> [int]:
    return _run(Computer({0: a, 1: 0, 2: 0}, code))


def _answer2(computer: Computer) -> int:
    def check(a: int) -> bool:
        r = _run2(a, computer.program)
        return r == computer.program

    def count_matching(result: [int]) -> int:
        if len(result) != len(computer.program):
            return 0
        matching = 0
        for i in range(len(result) - 1, -1, -1):
            if result[i] == computer.program[i]:
                matching += 1
            else:
                break
        return matching

    def a2(a_min: int, a_max: int, digits_matching: int) -> int:
        slices = 10000
        da = int((a_max - a_min) / slices)
        grid = {}
        if da > 1:
            for ai in range(a_min, (a_min + da * slices + 1), da):
                grid[ai] = _run2(ai, computer.program)

            next_matching = digits_matching + 1
            super_grid = dict([(ai, grid[ai]) for ai in grid if count_matching(grid[ai]) > digits_matching])
            if len(super_grid) == 0:
                next_matching = digits_matching
                super_grid = dict([(ai, grid[ai]) for ai in grid if count_matching(grid[ai]) >= digits_matching])

            new_a_min = min(super_grid.keys()) - da
            new_a_max = max(super_grid.keys()) + da
            return a2(new_a_min, new_a_max, next_matching)
        for ai in range(a_min, a_max + 1):
            if check(ai):
                return ai
        return -1

    a_min = 35100000000000  # 15 digits
    a_max = 561600000000000  # 17 digits
    return a2(a_min, a_max, 0)


class Day17(unittest.TestCase):

    def __solution(self, data: str, a1: str, a2: int):
        lines = c.read_lines(data)
        computer = _init(lines)
        self.assertEqual(a1, _answer1(computer), "answer 1")
        self.assertEqual(a2, _answer2(computer), "answer 2")

    def test_sample(self):
        lines = c.read_lines("17-1")
        computer = _init(lines)
        self.assertEqual("4,6,3,5,6,3,5,2,1,0", _answer1(computer), "answer 1")

    def test_day(self):
        self.__solution("17", "7,3,0,5,7,1,4,0,5", 202972175280682)  # part 2 in 16.45 seconds
