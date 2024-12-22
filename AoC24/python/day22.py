from math import floor

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


# TODO - template to use for daily solutions, don't forget to add the solution to aoc24.py
def _answer1(secrets: [int]) -> int:
    return sum(_next_n(secret, 2000) for secret in secrets)


def _answer2(secrets: [int]) -> int:
    return 0


class Day22(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        lines = c.read_lines(data)
        secrets = [int(l) for l in lines]
        self.assertEqual(a1, _answer1(secrets), "answer 1")
        self.assertEqual(a2, _answer2(secrets), "answer 2")

    def test_sample(self):
        self.__solution("22-1", 37327623, 0)

    def test_day(self):
        self.__solution("22", 14691757043, 0)
