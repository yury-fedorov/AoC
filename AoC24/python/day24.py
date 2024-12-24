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


def _reg_name(group: str, index: int) -> str: return f"{group}{index:02d}"


def _z(registers: {str: bool}) -> int:
    n = sum(1 for ri in registers.keys() if _is_z(ri))
    result = ""
    for i in range(n):
        result = ("1" if registers[_reg_name("z", i)] else "0") + result
    return _bin2int(result)


def _is_result_defined(registers: {str: bool}, n: int, mapping: {str: str}) -> bool:
    return sum([1 for ri in registers.keys() if _is_z(mapping.get(ri, ri))]) >= n


def _size_z(registers: {str: bool}, formulas: [Formula]) -> int:
    # detect all z
    z_set = {ri for ri in registers.keys() if _is_z(ri)}
    z_set.update([fi.r for fi in formulas if _is_z(fi.r)])
    return len(z_set)


def _calc(registers: {str: bool}, formulas: [Formula], mapping: {str: str}) -> int | None:
    n = _size_z(registers, formulas)
    while not _is_result_defined(registers, n, mapping):
        is_any_progress = False
        for fi in formulas:
            fir = mapping.get(fi.r, fi.r)
            if fir in registers.keys(): continue
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
                is_any_progress = True
                registers[fir] = result
        if not is_any_progress:
            # no new register was set in a whole iteration, we are stuck
            return None

    return _z(registers)


def _answer1(registers: {str: bool}, formulas: [Formula]) -> int:
    return _calc(registers, formulas, {})


def _set_registers(x, y, len_reg: int) -> {str: bool}:
    result = {}
    for i in range(len_reg):
        result[_reg_name("x", i)] = (x & (1 << i)) != 0
        result[_reg_name("y", i)] = (y & (1 << i)) != 0
    return result


def _answer2(registers: {str: bool}, formulas: [Formula]) -> int:
    n = _size_z(registers, formulas)
    r = _set_registers(1, 3, n)
    return 0
    return _calc(r, formulas, {})


def _parse(data: str) -> ({str: bool}, [Formula]):
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
    return registers, formulas


class Day24(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        registers, formulas = _parse(data)
        self.assertEqual(a1, _answer1(registers, formulas), "answer 1")
        self.assertEqual(a2, _answer2(registers, formulas), "answer 2")

    def test_z(self):
        self.assertEqual(4, _z({"z00": 0, "z01": 0, "z02": 1}), "z case 1")
        self.assertEqual(2024, _bin2int("0011111101000"), "_bin2int")

    def test_sample1(self):
        registers, formulas = _parse("24-1")
        self.assertEqual(4, _answer1(registers, formulas), "answer 1 - sample 1")

    def test_sample2(self):
        registers, formulas = _parse("24-2")
        self.assertEqual(2024, _answer1(registers, formulas), "answer 1 - sample 2")

    def test_day(self):
        self.__solution("24", 55114892239566, 0)
