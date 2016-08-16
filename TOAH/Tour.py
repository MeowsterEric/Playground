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

from ConsoleController import ConsoleController
from GUIController import GUIController
from TOAHModel import TOAHModel

import time


def tower_of_hanoi(model, n, src, spare, dest,
                   delay_btw_moves, console_animate):
    """ (TOAHModel, int, int, int, float, bool) -> NoneType
    This is a helper function for rec_four_stools. This function applies
    the Tower of Hanoi algorithm.
    This algorithm will run 2^n-1 steps to complete

    Note: n is the number of cheeses
    """
    if (n > 0):
        # move n-1 Cheeses from src stool to spare stool, we use dest stool
        # as the temporary stool
        tower_of_hanoi(model, n - 1, src, dest, spare,
                       delay_btw_moves, console_animate)

        # move operation
        model.move(src, dest)
        # display to console if the animation is set to True
        if(console_animate):
            print(model)
            time.sleep(delay_btw_moves)

        # move n-1 Cheeses from spare stool to dest stool, we use src stool
        # as the temporary stool
        tower_of_hanoi(model, n - 1, spare, src, dest,
                       delay_btw_moves, console_animate)


def rec_four_stools(model, n, src, spare_1, spare_2, dest,
                    delay_btw_moves, console_animate):
    """ (TOAHModel, int, int, int, int, float, bool) -> NoneType
    This is the four stool recursive algorithm. Consider the following steps:

    1. move half of the cheeses from src to spare_1 stool, we use spare_2 and
       dest stool as temporary stools. This will run M(n-1) time.

    2. move rest of the cheeses from src to dest stool, we use spare_2 as the
       temporary stool. Apply Tower of Hanoi algorithm here. This will run
       (2^n-1) time to complete.

    3. move the cheese stack from step 1 from spare_1 to dest stool, we use
       src and spare_2 as temporary stools. This takes another M(n-1) time.
    """
    if (n > 0):
        # the step 1
        rec_four_stools(model, n // 2, src, spare_2, dest, spare_1,
                        delay_btw_moves, console_animate)

        # the step 2: Tower of Hanoi
        tower_of_hanoi(model, n - n // 2, src, spare_2, dest,
                       delay_btw_moves, console_animate)

        # the step 3
        rec_four_stools(model, n // 2, spare_1, src, spare_2, dest,
                        delay_btw_moves, console_animate)


def tour_of_four_stools(model: TOAHModel, delay_btw_moves: float=0.5,
                        console_animate: bool=False):
    """Move a tower of cheeses from the first stool in model to the fourth.

       model - a TOAHModel with a tower of cheese on the first stool
                and three other empty stools
       console_animate - whether to use ConsoleController to animate the tour
       delay_btw_moves - time delay between moves in seconds IF
                         console_animate == True
                         no effect if console_animate == False
    """
    src = 0
    spare_1 = 1
    spare_2 = 2
    dest = 3
    num_cheeses = model.number_of_cheeses()

    rec_four_stools(model, num_cheeses, src, spare_1, spare_2, dest,
                    delay_btw_moves, console_animate)

if __name__ == '__main__':
    # DO NOT MODIFY THE CODE BELOW.
    model = TOAHModel(4)
    model.fill_first_stool(number_of_cheeses=8)

    tour_of_four_stools(model, console_animate=True, delay_btw_moves=0.5)
    print(model.number_of_moves())

    # some debug code below, don't forget to comment it #
    # print(model.get_move_seq().get_moves())
