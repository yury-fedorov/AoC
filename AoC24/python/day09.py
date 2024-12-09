import common as c
import unittest

EMPTY_SPACE = -1


def _unpack_disk(disk: str) -> {}:
    '''Returns map where key is global position and value is the file id or empty space marker.'''
    unpacked = {}  # common vector will supper on performance during the degfragmentation, alternative is linked list
    file_id = 0
    global_index = 0
    for i, length in enumerate(disk):
        length = int(length)
        is_file = (i % 2 == 0)  # zero based index, first goes a file
        content = file_id if is_file else EMPTY_SPACE  # file id or empty space
        for _ in range(length):
            unpacked[global_index] = content
            global_index += 1
        file_id += 1 if is_file else 0  # next file id
    return unpacked


def _defrag(disk_map: {}):
    '''Defragmentation for the answer 1 (fragmented files, empty space at the end is continuous).'''
    disk_end = max(disk_map.keys())
    position = disk_end
    low_bound_empty = 0  # before it there is no free space
    while position >= 0:
        content = disk_map[position]
        is_file = content != EMPTY_SPACE
        if is_file:
            # let's find empty space
            while low_bound_empty < position:
                if disk_map[low_bound_empty] == EMPTY_SPACE:
                    # found!
                    disk_map[low_bound_empty] = content
                    disk_map[position] = EMPTY_SPACE
                    break
                else:
                    # not found, let's move bound ahead
                    low_bound_empty += 1
        position -= 1
    return disk_map


# TODO may rewrite in shorter form
def _checksum(disk_map: {}) -> int:
    result = 0
    for position, file_id in disk_map.items():
        if file_id != EMPTY_SPACE:
            result += (position * file_id)
    return result


def _defrag2(disk_map: {}):
    '''Defragmentation for the answer 2 (all files continues and moved only once).'''
    disk_end = max(disk_map.keys())
    position = disk_end
    while position >= 0:
        content = disk_map[position]
        is_file = content != EMPTY_SPACE
        if not is_file:
            position -= 1
        else:
            # let's find sufficient empty space
            file_size = 0
            cur_position = position
            while cur_position >= 0 and disk_map[cur_position] == content:
                file_size += 1
                cur_position -= 1

            # find sufficient empty space
            # empty_head = find_empty_space(disk_map, file_size)
            empty_head = 0  # XXX here could be improved
            while empty_head <= position:
                if disk_map[empty_head] != EMPTY_SPACE:
                    empty_head += 1
                else:
                    is_good = True
                    for i in range(file_size):
                        if disk_map[empty_head + i] != EMPTY_SPACE:
                            # not big enough
                            empty_head += i
                            is_good = False
                            break  # out of internal continues empty space loop
                    if is_good:
                        break  # out of searching loop, we have found the empty space

            if empty_head < position:
                # reallocate the file
                for i in range(file_size):
                    c = disk_map[position - i]
                    if c != content:
                        # shouldn't happen
                        raise ValueError()
                    e = disk_map[empty_head + i]
                    if e != EMPTY_SPACE:
                        # shouldn't happen
                        raise ValueError()
                    disk_map[empty_head + i] = content
                    disk_map[position - i] = EMPTY_SPACE

            # we move in front of ex file in both cases: when it was moved and when not
            position -= file_size

    return disk_map


def _answer1(disk: str) -> {}:
    return _checksum(_defrag(_unpack_disk(disk)))


def _answer2(disk: str) -> {}:
    return _checksum(_defrag2(_unpack_disk(disk)))


class Day09(unittest.TestCase):

    def __solution(self, data: str, a1: int, a2: int):
        disk = c.read_lines(data)[0]
        self.assertEqual(a1, _answer1(disk), "answer 1")
        if not c.is_fast_only():  # takes 34 seconds
            self.assertEqual(a2, _answer2(disk), "answer 2")

    def test_sample(self):
        self.__solution("09-1", 1928, 2858)

    def test_day(self):
        self.__solution("09", 6370402949053, 6398096697992)
