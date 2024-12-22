from math import floor
from typing import NamedTuple

import common as c
import unittest


def _mix(secret, value: int) -> int: return secret ^ value


def _prune(secret: int) -> int: return secret % 16777216


def _next(secret: int) -> int:
    secret = _prune(_mix(secret, secret * 64))
    secret = _prune(_mix(secret, floor(secret / 32)))
    return _prune(_mix(secret, secret * 2048))


def _next_n(secret: int, n: int) -> int:
    for _ in range(n):
        secret = _next(secret)
    return secret


def _price(secret: int) -> int: return secret % 10


class PriceChange(NamedTuple):
    price: int
    change: int


def _price_change(secret, n: int) -> [PriceChange]:
    previous = _price(secret)
    result = []
    for _ in range(n):
        secret = _next(secret)
        price = _price(secret)
        result.append(PriceChange(price, price - previous))
        previous = price
    return result


def _answer1(secrets: [int]) -> int:
    return sum(_next_n(secret, 2000) for secret in secrets)


def _answer2(secrets: [int]) -> int:
    price_change_list = [_price_change(secret, 2000) for secret in secrets]
    return 0


class Day22(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        secrets = [int(l) for l in lines]
        self.assertEqual(a1, _answer1(secrets), "answer 1")
        self.assertEqual(a2, _answer2(secrets), "answer 2")

    def test_sample(self):
        self.__solution("22-1", 37327623, 0)

    def test_prices(self):
        test = _price_change(123, 9)
        prices = [pc.price for pc in test]
        changes = [pc.change for pc in test]
        self.assertEqual([0, 6, 5, 4, 4, 6, 4, 4, 2], prices, "prices")
        self.assertEqual([-3, 6, -1, -1, 0, 2, -2, 0, -2], changes, "changes")

    def test_day(self):
        self.__solution("22", 14691757043, 0)
