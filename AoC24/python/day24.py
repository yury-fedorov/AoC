import common as c
import unittest
from itertools import combinations
from typing import NamedTuple


class Formula(NamedTuple):
    a: str
    b: str
    op: str
    r: str


def _bin2int(value: str) -> int: return int(value, 2)


def _is_z(reg_name: str) -> bool: return reg_name.startswith("z")


def _reg_name(group: str, index: int) -> str: return f"{group}{index:02d}"


def _z(registers: {str: bool}, mapping: {str: str}) -> int:
    mz = {mapping.get(ri, ri): ri for ri in registers.keys() if _is_z(mapping.get(ri, ri))}
    n = sum(1 for ri in registers.keys() if _is_z(mapping.get(ri, ri)))
    result = ""
    for i in range(n):
        result = ("1" if registers[mz[_reg_name("z", i)]] else "0") + result
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

    return _z(registers, mapping)


def _answer1(registers: {str: bool}, formulas: [Formula]) -> int:
    return _calc(registers, formulas, {})


def _set_registers(x, y, len_reg: int) -> {str: bool}:
    result = {}
    for i in range(len_reg):
        result[_reg_name("x", i)] = (x & (1 << i)) != 0
        result[_reg_name("y", i)] = (y & (1 << i)) != 0
    return result


def _set_registers_with_xy(x, y, index, len_reg: int) -> {str: bool}:
    return _set_registers(x << index, y << index, len_reg)


def _check_wrong_mode(formulas: [Formula], n: int, mapping: {str: str}, mode: str) -> [int]:
    wrong = []
    for i in range(n - 1):
        a, b = 0, 0
        match mode:
            case "a":
                a = 1
            case "b":
                b = 1
            case "ab":
                a, b = 1, 1
        regs = _set_registers_with_xy(a, b, i, n)
        res = _calc(regs, formulas, mapping)
        if res != (a << i) + (b << i):
            wrong.append(i)
            # print( f"{i} {a} + {b} -> {res}")
    return wrong


def _check_wrong(formulas: [Formula], n: int, mapping: {str: str}) -> [int]:
    wa = _check_wrong_mode(formulas, n, mapping, "a")
    if len(wa): return wa
    wb = _check_wrong_mode(formulas, n, mapping, "b")
    if len(wb): return wb
    return _check_wrong_mode(formulas, n, mapping, "ab")


def _pairs_to_mapping(pairs: [(str, str)]) -> {str: str}:
    m = {}
    for a, b in pairs:
        m[a] = b
        m[b] = a
    return m


def _check_pairs(formulas: [Formula], n: int, pairs: [(str, str)]) -> (bool, [int]):
    m = _pairs_to_mapping(pairs)
    # if len(m) != 8: return False, []
    return True, _check_wrong(formulas, n, m)


def _answer2(registers: {str: bool}, formulas: [Formula]) -> int:
    n = _size_z(registers, formulas)
    wrong = _check_wrong(formulas, n, {})
    print(wrong)
    all_regs = {ri for ri in registers.keys()}
    all_regs.update([fi.r for fi in formulas])
    if False:
        # searching for pairs
        for i, a in enumerate(all_regs):
            for j, b in enumerate(all_regs):
                if j > i:
                    wrong1 = _check_wrong(formulas, n, {a: b, b: a})
                    if len(wrong1) < len(wrong):
                        print(f"{a} - {b}")
        # 8,
        # 16 65536 + 0 -> 131072
        # 32 4294967296 + 0 -> 8589934592
        # 38 274877906944 + 0 -> 549755813888
    pairs = [("qmd", "dhm"), ("qmd", "bqf"), ("gnn", "dhm"), ("gnn", "bqf"), ("jbc", "pqv"), ("jbc", "gfm"),
             ("dhm", "kvn"), ("dhm", "qjd"), ("kvn", "bqf"), ("mnv", "pqv"), ("mnv", "gfm"), ("bqf", "qjd")]
    if False:
        options = list(combinations(pairs, 4))
        for oi in options:
            mi = {}
            for pi in oi:
                a, b = pi
                mi[a] = b
                mi[b] = a
            if len(mi) != 8: continue
            wi = _check_wrong(formulas, n, mi)
            if len(wrong) > len(wi):
                # print(wi)
                # print(oi)
                wrong = wi

        ok3 = [('jbc', 'pqv'), ('gnn', 'bqf'), ('qmd', 'dhm')]
        for i in range(len(ok3)):
            mi = {}
            for j, fai in enumerate(ok3):
                if j == i: continue
                a, b = fai
                mi[a] = b
                mi[b] = a
            print(f"{i} {ok3[i]} {_check_wrong(formulas, n, mi)}")

        cr = {a for a, _ in pairs}
        cr.update({b for _, b in pairs})
        for a, b in ok3:
            cr.remove(a)
            cr.remove(b)
        if True:
            options = list(combinations(all_regs, 2))
            for oi in options:
                mi = {}
                for pi in ok3:
                    a, b = pi
                    mi[a] = b
                    mi[b] = a
                a, b = oi
                mi[a] = b
                mi[b] = a
                if len(mi) != 8: continue
                w = _check_wrong(formulas, n, mi)
                if len(wrong) > len(w):
                    print(oi)
                    print(w)

    p = [("pqv", "z32")]
    wr, wp = _check_pairs(formulas, n, p)
    return 0  # TODO to be solved
    return wrong


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
        self.assertEqual(4, _z({"z00": 0, "z01": 0, "z02": 1}, {}), "z case 1")
        self.assertEqual(2024, _bin2int("0011111101000"), "_bin2int")

    def test_sample1(self):
        registers, formulas = _parse("24-1")
        self.assertEqual(4, _answer1(registers, formulas), "answer 1 - sample 1")

    def test_sample2(self):
        registers, formulas = _parse("24-2")
        self.assertEqual(2024, _answer1(registers, formulas), "answer 1 - sample 2")

    def test_broken(self):
        registers, formulas = _parse("24")
        n = _size_z(registers, formulas)
        rs = _set_registers_with_xy(1, 0, 8, n)
        res = _calc(rs, formulas, {})
        res1 = _calc(rs, formulas, _pairs_to_mapping([("dnc", "z08")]))
        self.assertEqual(1 << 8, res1 & (1 << 8), "check 8 res")

    def test_day(self):
        self.__solution("24", 55114892239566, 0)
