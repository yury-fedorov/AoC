import common as c
import unittest


def _read_input(data: str) -> ([], []):
    rules = []
    updates = []
    is_rule = True
    for line in c.read_lines(data):
        if len(line) <= 1:
            is_rule = False
        elif is_rule:
            rules.append(line.split('|'))
        else:
            updates.append(line.split(','))
    return rules, updates


def _middle_page(order: str) -> int: return int(order[int(len(order) / 2)])


def _is_correct(rules: [], update: []) -> bool:
    if len(update) <= 1:
        return True
    later = update[1:]
    page = update[0]
    for before, after in rules:
        if after == page:
            if any(x == before for x in later):
                return False
    return _is_correct(rules, later)


def _answer1(rules, updates) -> int:
    return sum([_middle_page(u) for u in updates if _is_correct(rules, u)])


def _correct(rules: [], update: []) -> []:
    def sub_correct(rules: [], corrected: [], to_correct: str) -> []:
        n = len(corrected)
        for i in range(n + 1):
            version = corrected[:i] + [to_correct] + corrected[i:]
            if _is_correct(rules, version):
                return version
        raise RuntimeError(f"Bad sequence {corrected} for {to_correct}")

    corrected = [update[0]]
    to_correct = update[1:]
    while len(to_correct) > 0:
        next_to_correct = to_correct[0]
        to_correct = to_correct[1:]
        corrected = sub_correct(rules, corrected, next_to_correct)
    return corrected


def _answer2(rules, updates) -> int:
    incorrect = [u for u in updates if not _is_correct(rules, u)]
    return sum([_middle_page(_correct(rules, u)) for u in incorrect])


class Day05(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        rules, updates = _read_input(data)
        self.assertEqual(_answer1(rules, updates), a1, "answer 1")
        self.assertEqual(_answer2(rules, updates), a2, "answer 2")

    def test_middle_page(self):
        self.assertEqual(_middle_page([75, 47, 61, 53, 29]), 61, "of five")
        self.assertEqual(_middle_page([97, 61, 53, 29, 13]), 53, "of five for 53")
        self.assertEqual(_middle_page([75, 29, 13]), 29, "of three")

    def test_sample(self):
        self.__solution("05-1", 143, 123)

    def test_day(self):
        self.__solution("05", 7307, 4713)  # 4672 -- too low
