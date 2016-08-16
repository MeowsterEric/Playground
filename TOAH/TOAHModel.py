# Copyright 2013, 2014 Gary Baumgartner, Danny Heap, Dustin Wehr
# Distributed under the terms of the GNU General Public License.
#
# This file is part of Assignment 2, CSCA48, Summer 2016.
#
# This is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/>.
"""
TOAHModel:  Model a game of Towers of Anne Hoy
Cheese:   Model a cheese with a given (relative) size
IllegalMoveError: Type of exceptions thrown when an illegal move is attempted
MoveSequence: Record of a sequence of (not necessarily legal) moves. You will
need to return MoveSequence object after solving an instance of the 4-stool
Towers of Anne Hoy game, and we will use that to check the correctness of your
algorithm.
"""


class TOAHModel:
    """Model a game of Towers Of Anne Hoy.

    Model stools holding stacks of cheese, enforcing the constraint
    that a larger cheese may not be placed on a smaller one.  Note that
    large, aged, cheeses at the bottom of each pile serve as stools, and
    these may not be moved!

    fill_first_stool - put an existing model in the standard starting config
    move - move cheese from one stool to another
    add - add a cheese to a stool
    cheese_location - index of the stool that the given cheese is on
    number_of_cheeses - number of cheeses in this game
    number_of_moves - number of moves so far
    number_of_stools - number of stools in this game
    get_move_seq - MoveSequence object that records the moves used so far

    """

    def __init__(self, number_of_stools):
        """ (TOAHModel, int) -> NoneType
        Initialize model attributes, setup number of stool lists
        """
        self._num_stools = number_of_stools
        self._stool_list = []
        # initialize a stool list to store cheeses
        for stool in range(self._num_stools):
            self._stool_list.append([])

        self._num_cheeses = -1
        self._move_seq = MoveSequence([])

    def fill_first_stool(self: 'TOAHModel', number_of_cheeses: int):
        """
        Put number_of_cheeses cheeses on the first (i.e. 0-th) stool, in order
        of size, with a cheese of size == number_of_cheeses on bottom and
        a cheese of size == 1 on top.

        >>> M = TOAHModel(3)
        >>> M.fill_first_stool(5)
        >>> len(M.get_stool(0))
        5
        >>> M._cheese_at(0, 0).size
        5
        >>> M._cheese_at(0, 2).size
        3
        >>> M._cheese_at(0, 4).size
        1
        """
        self._num_cheeses = number_of_cheeses
        for size in range(number_of_cheeses, 0, -1):
            self.get_stool(0).append(Cheese(size))

    def _cheese_at(self, stool_index, stool_height):
        """ (TOAHModel, int, int) -> Cheese
        If there are at least stool_height+1 cheeses
        on stool stool_index then return the (stool_height)-th one.
        Otherwise return None.

        >>> M = TOAHModel(4)
        >>> M.fill_first_stool(5)
        >>> M._cheese_at(0,3).size
        2
        >>> M._cheese_at(0,0).size
        5
        """
        if (stool_index >= self.number_of_stools() or
                stool_height >= self.get_num_cheese_on_stool(stool_index)):
            cheese = None
        else:
            cheese = self.get_stool(stool_index)[stool_height]
        return cheese

    def get_move_seq(self: 'TOAHModel') -> 'MoveSequence':
        """
        Return the move sequence in the current game.
        """
        return self._move_seq

    def get_stool(self, stool_index):
        """ (TOAHModel, int) -> list of Cheese
        Return a stool list on a specific index, otherwise return -1.

        REQ: 0 <= index <= number of stools
        """
        return self._stool_list[stool_index]

    def get_num_cheese_on_stool(self, stool_index):
        """ (TOAHModel, int) -> int
        Return the number of cheeses on a specific stool.

        REQ: 0 <= index <= number of stools
        """
        return len(self._stool_list[stool_index])

    def top_cheese(self, stool_index):
        """ (TOAHModel, int) -> Cheese
        Return the top cheese (last cheese in the list) from a specific stool.
        This method does NOT remove the cheese from the list.
        """
        if (self.get_num_cheese_on_stool(stool_index) == 0):
            cheese = None
        else:
            cheese = self.get_stool(stool_index)[-1]
        return cheese

    def remove_top_cheese(self, stool_index):
        """ (TOAHModel, int) -> Cheese
        Return the top cheese (last cheese in the list) from a specific stool.
        This method remove the cheese from the list.
        """
        return self.get_stool(stool_index).pop()

    def number_of_cheeses(self):
        """ (TOAHModel) -> int
        Return the number of cheeses in this game.
        """
        return self._num_cheeses

    def number_of_stools(self):
        """ (TOAHModel) -> int
        Return the number of stools in this game.
        """
        return self._num_stools

    def number_of_moves(self):
        """ (TOAHModel) -> int
        Return the total number of moves in this game.
        """
        return self.get_move_seq().length()

    def move(self, src, dest):
        """ (TOAHModel, int, int) -> NoneType
        Move the top cheese from the src stool to dest stool.

        >>> M = TOAHModel(4)
        >>> M.fill_first_stool(3)
        >>> M.move(0, 1)
        >>> M.top_cheese(0)
        Cheese(2)
        >>> M.top_cheese(1)
        Cheese(1)
        """
        # Error Checking: input src or dest index is out of range
        if (src < 0 or src > self.number_of_stools()):
            raise IllegalMoveError
        elif (dest < 0 or dest > self.number_of_stools()):
            raise IllegalMoveError
        # Error Checking: src and dest index cannot be the same
        elif (src == dest):
            raise IllegalMoveError
        # Error Checking: src stool is empty
        elif (self.get_num_cheese_on_stool(src) == 0):
            raise IllegalMoveError
        # Error Checking: dest stool is not empty, moving cheese size
        # must not be greater than the top cheese size
        elif (self.get_num_cheese_on_stool(dest) > 0 and
              self.top_cheese(src).size > self.top_cheese(dest).size):
            raise IllegalMoveError
        else:
            cheese = self.remove_top_cheese(src)
            self.add(dest, cheese)
            self.get_move_seq().add_move(src, dest)

    def add(self, stool_index, cheese):
        """ (TOAHModel, int, Cheese) -> NoneType
        Add a Cheese to a specific stool.

        >>> M = TOAHModel(4)
        >>> M.add(1, Cheese(4))
        >>> M.cheese_location(Cheese(4))
        1
        """
        self.get_stool(stool_index).append(cheese)

    def cheese_location(self, cheese):
        """ (TOAHModel, Cheese) -> int

        >>> M = TOAHModel(4)
        >>> M.fill_first_stool(5)
        >>> M.cheese_location(Cheese(1))
        0
        >>> M.add(3, Cheese(6))
        >>> M.cheese_location(Cheese(6))
        3
        """
        for stool_index in range(self.number_of_stools()):
            for c in self.get_stool(stool_index):
                if (c == cheese):
                    return stool_index
        return None

    def __eq__(self: 'TOAHModel', other: 'TOAHModel') -> bool:
        """
        We're saying two TOAHModels are equivalent if their current
        configurations of cheeses on stools look the same.
        More precisely, for all h,s, the h-th cheese on the s-th
        stool of self should be equivalent the h-th cheese on the s-th
        stool of other

        >>> m1 = TOAHModel(4)
        >>> m1.fill_first_stool(7)
        >>> m1.move(0,1)
        >>> m1.move(0,2)
        >>> m1.move(1,2)
        >>> m2 = TOAHModel(4)
        >>> m2.fill_first_stool(7)
        >>> m2.move(0,3)
        >>> m2.move(0,2)
        >>> m2.move(3,2)
        >>> m1 == m2
        True
        """
        # checking if both model have the same number of stools
        s_num_stools = self.number_of_stools()
        o_num_stools = other.number_of_stools()
        if s_num_stools != o_num_stools:
            return False

        # checking if the stool on a specific index has same height
        for idx in range(s_num_stools):
            s_stool_height = self.get_num_cheese_on_stool(idx)
            o_stool_height = other.get_num_cheese_on_stool(idx)
            if (s_stool_height != o_stool_height):
                return False

            # checking if the Cheese has the same size
            for h in range(s_stool_height):
                if (self._cheese_at(idx, h) != other._cheese_at(idx, h)):
                    return False
        return True

    def __str__(self: 'TOAHModel') -> str:
        """
        Depicts only the current state of the stools and cheese.
        """
        stool_str = "=" * (2 * (self.number_of_cheeses()) + 1)
        stool_spacing = "  "
        stools_str = (stool_str + stool_spacing) * self.number_of_stools()

        def cheese_str(size: int):
            if size == 0:
                return " " * len(stool_str)
            cheese_part = "-" + "--" * (size - 1)
            space_filler = " " * int((len(stool_str) - len(cheese_part)) / 2)
            return space_filler + cheese_part + space_filler

        lines = ""
        for height in range(self.number_of_cheeses() - 1, -1, -1):
            line = ""
            for stool in range(self.number_of_stools()):
                c = self._cheese_at(stool, height)
                if isinstance(c, Cheese):
                    s = cheese_str(int(c.size))
                else:
                    s = cheese_str(0)
                line += s + stool_spacing
            lines += line + "\n"
        lines += stools_str

        return lines


class Cheese:

    def __init__(self: 'Cheese', size: int):
        """
        Initialize a Cheese to diameter size.

        >>> c = Cheese(3)
        >>> isinstance(c, Cheese)
        True
        >>> c.size
        3
        """
        self.size = size

    def __repr__(self: 'Cheese') -> str:
        """
        Representation of this Cheese
        """
        return "Cheese(" + str(self.size) + ")"

    def __eq__(self: 'Cheese', other: 'Cheese') -> bool:
        """
        Is self equivalent to other? We say they are if they're the same size.
        """
        return isinstance(other, Cheese) and self.size == other.size


class IllegalMoveError(Exception):
    pass


class InvalidInputError(Exception):
    pass


class MoveSequence(object):

    def __init__(self: 'MoveSequence', moves: list):
        # moves - a list of integer pairs, e.g. [(0,1),(0,2),(1,2)]
        self._moves = moves

    def get_move(self: 'MoveSequence', i: int):
        # Exception if not (0 <= i < self.length)
        return self._moves[i]

    def get_moves(self: "MoveSequence"):
        return self._moves

    def add_move(self: 'MoveSequence', src_stool: int, dest_stool: int):
        self._moves.append((src_stool, dest_stool))

    def length(self: 'MoveSequence') -> int:
        return len(self._moves)

    def generate_TOAHModel(self: 'MoveSequence', number_of_stools: int,
                           number_of_cheeses: int) -> 'TOAHModel':
        """
        An alternate constructor for a TOAHModel. Takes the two parameters for
        the game (number_of_cheeses, number_of_stools), initializes the game
        in the standard way with TOAHModel.fill_first_stool(number_of_cheeses),
        and then applies each of the moves in move_seq.
        """
        model = TOAHModel(number_of_stools)
        model.fill_first_stool(number_of_cheeses)
        for move in self._moves:
            model.move(move[0], move[1])
        return model

    def __repr__(self: 'MoveSequence') -> str:
        return "MoveSequence(" + repr(self._moves) + ")"


if __name__ == '__main__':
    import doctest
    doctest.testmod(verbose=True)
