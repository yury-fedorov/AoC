from typing import NamedTuple

import common as c
import unittest


class Item(NamedTuple):
    is_door: bool  # door / lock
    pins: [int]


def _match(a, b: Item, height: int) -> bool:
    if not a.is_door ^ b.is_door: return False
    for i, ap in enumerate(a.pins):
        if (ap + b.pins[i]) > height: return False
    return True


def _answer1(height: int, items: [Item]) -> int:
    doors = [i for i in items if i.is_door]
    locks = [i for i in items if not i.is_door]
    result = 0
    for d in doors:
        for l in locks:
            result += 1 if _match(d, l, height) else 0
    return result


def _parse_item(height: int, lines: [str]) -> Item:
    first_line = lines[0]
    is_door = False if first_line.startswith("###") else True
    pins = []
    for pin_index, _ in enumerate(first_line):
        pin_length = 0
        for h in range(height):
            pin_length += 1 if lines[h][pin_index] == "#" else 0
        pins.append(pin_length)
    return Item(is_door, pins)


def _parse(lines: [str]) -> (int, [Item]):
    height = None
    for li, l in enumerate(lines):
        if len(l) < 2:
            height = li
            break

    items = []
    for i in range(0, len(lines), height + 1):
        items.append(_parse_item(height, lines[i:]))
    return height, items


class Day25(unittest.TestCase):

    def __solution(self, data: str, a1: int):
        lines = c.read_lines(data)
        height, items = _parse(lines)
        self.assertEqual(a1, _answer1(height, items), "answer 1")

    def test_sample(self):
        self.__solution("25-1", 3)

    def test_day(self):
        self.__solution("25", 3663)
