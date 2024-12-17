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


def _init(data: [str]) -> Computer:
    registers = dict({})
    ri = 0
    for l in data[:3]:
        registers[ri] = int(l.split(": ")[1])
        ri += 1
    program = [int(i) for i in data[4].split(": ")[1].split(",")]
    return Computer(registers, program)


def _run(computer: Computer) -> [int]:
    # TODO - implement
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
    b = computer.b()
    o = operand
    result = 0
    # TODO - https://en.wikipedia.org/wiki/Bitwise_operation#XOR
    return result


def _output_to_str(output: [int]) -> str:
    return ",".join([str(i) for i in output])


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(computer: Computer) -> str:
    return _output_to_str(_run(computer))


def _answer2(computer: Computer) -> str:
    return ""


class Day17(unittest.TestCase):

    def __solution(self, data: str, a1: str, a2: str):
        lines = c.read_lines(data)
        computer = _init(lines)
        self.assertEqual(a1, _answer1(computer), "answer 1")
        self.assertEqual(a2, _answer2(computer), "answer 2")

    def test_sample(self):
        self.__solution("17-1", "4,6,3,5,6,3,5,2,1,0", "")

    def test_day(self):
        self.__solution("17", "", "")
