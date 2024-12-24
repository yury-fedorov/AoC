from typing import NamedTuple

import common as c
import unittest


class Formula(NamedTuple):
    a: str
    b: str
    op: str
    r: str


def _bin2int(value: str) -> int: return int(value, 2)


def _is_z(reg_name: str) -> bool: return reg_name.startswith("z")


def _z(registers: {str: bool}) -> int:
    n = sum(1 for ri in registers.keys() if _is_z(ri))
    result = ""
    for i in range(n):
        result = ("1" if registers[f"z{i:02d}"] else "0") + result
    return _bin2int(result)


def _is_result_defined(registers: {str: bool}, n: int) -> bool:
    return sum([1 for ri in registers.keys() if _is_z(ri)]) >= n


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(registers: {str: bool}, formulas: [Formula]) -> int:
    # detect all z
    z_set = {ri for ri in registers.keys() if _is_z(ri)}
    z_set.update([fi.r for fi in formulas if _is_z(fi.r)])
    n = len(z_set)
    while not _is_result_defined(registers, n):
        for fi in formulas:
            if fi.r in registers.keys(): continue
            result = None
            a = registers.get(fi.a, None)
            b = registers.get(fi.b, None)
            is_a = a is not None
            is_b = b is not None
            match fi.op:
                case "AND":
                    # enough single False to know result
                    if (is_a and a == False) or (is_b and b == False):
                        result = False
                    elif is_a and is_b:
                        result = a and b
                case "OR":
                    # enough single True to know result
                    if (is_a and a == True) or (is_b and b == True):
                        result = True
                    elif is_a and is_b:
                        result = a or b
                case "XOR":
                    if is_a and is_b:
                        result = a ^ b
                case _:
                    raise ValueError("bad operator")
            if result is not None:
                registers[fi.r] = result

    return _z(registers)


def _answer2(lines: []) -> int:
    return 0


class Day24(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        registers = {}
        formulas = []
        is_registers = True
        for l in lines:
            if len(l) < 2:
                is_registers = False
            elif is_registers:
                name, value = l.split(": ")
                registers[name] = int(value) == 1
            else:
                exp, r = l.split(" -> ")
                a, op, b = exp.split(" ")
                formulas.append(Formula(a, b, op, r))
        self.assertEqual(a1, _answer1(registers, formulas), "answer 1")
        self.assertEqual(a2, _answer2(lines), "answer 2")

    def test_z(self):
        self.assertEqual(4, _z({"z00": 0, "z01": 0, "z02": 1}), "z case 1")
        self.assertEqual(2024, _bin2int("0011111101000"), "_bin2int")

    def test_sample1(self):
        self.__solution("24-1", 4, 0)

    def test_sample2(self):
        self.__solution("24-2", 2024, 0)

    def test_day(self):
        self.__solution("24", 55114892239566, 0)
