# Copyright 2014 Dustin Wehr
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
ConsoleController: User interface for manually solving Anne Hoy's problems
from the console.

move: Apply one move to the given model, and print any error message
to the console.
"""

from TOAHModel import TOAHModel, Cheese, IllegalMoveError, InvalidInputError
import tkinter as TI
import time

INSTRUCTIONS = "Game Instructions:\n  - To Move: move s t\n  - To Quit: quit"
DASH_SEPARATOR = "-------------------------------------------------------"
EQUAL_SEPARATOR = "======================================================="


def move(model: TOAHModel, origin: int, dest: int):
    '''
    Module method to apply one move to the given model, and print any
    error message to the console.

    model - the TOAHModel that you want to modify
    origin - the stool number (indexing from 0!) of the cheese you want
             to move
    dest - the stool number that you want to move the top cheese
            on stool origin onto.
    '''
    model.move(origin, dest)


class ConsoleController:

    def __init__(self: 'ConsoleController',
                 number_of_cheeses: int, number_of_stools: int):
        """
        Initialize a new 'ConsoleController'.

        number_of_cheeses - number of cheese to tower on the first stool,
                            not counting the bottom cheese acting as stool
        number_of_stools - number of stools, to be shown as large cheeses
        """
        self._model = TOAHModel(number_of_stools)
        self._model.fill_first_stool(number_of_cheeses)

    def get_model(self):
        """ (ConsoleController) -> TOAHModel
        Return the TOAHModel of this game.
        """
        return self._model

    def play_loop(self: 'ConsoleController'):
        '''
        Console-based game.
        TODO:
        -Start by giving instructions about how to enter moves (which is up to
        you). Be sure to provide some way of exiting the game, and indicate
        that in the instructions.
        -Use python's built-in function input() to read a potential move from
        the user/player. You should print an error message if the input does
        not meet the specifications given in your instruction or if it denotes
        an invalid move (e.g. moving a cheese onto a smaller cheese).
        You can print error messages from this method and/or from
        ConsoleController.move; it's up to you.
        -After each valid move, use the method TOAHModel.__str__ that we've
        provded to print a representation of the current state of the game.
        '''
        print(self.get_model())
        print(INSTRUCTIONS)

        print(DASH_SEPARATOR)
        cmd = input("Please enter your command: ")
        while(cmd != "quit"):
            # parse the user input to get src and dest stool index
            # re-enter if InvalidInputError exception caught
            try:
                src, dest = _instruction_filter(cmd)
            except InvalidInputError:
                print("Invalid Input!\nPlease follow the instruction!")
                print(INSTRUCTIONS + "\n" + DASH_SEPARATOR)
                cmd = input("Please enter your command: ")
                continue

            # perform move operation, re-enter command if exception caught
            try:
                move(self.get_model(), src, dest)
            except IllegalMoveError:
                print("Illegal Move!\nPlease follow the instruction!")
                print(INSTRUCTIONS + "\n" + DASH_SEPARATOR)
                cmd = input("Please enter your command: ")
                continue

            print(self.get_model())
            print(DASH_SEPARATOR)

            # check if the game is ended
            if (self._winning_check()):
                print("Congratulation!")
                cmd = "quit"
            else:
                cmd = input("Please enter your command: ")

        print("Thank you for playing.")

    def _winning_check(self):
        """ (ConsoleController) -> bool
        Return True if all cheeses are moved to the last stool.
        This represents the game is ended.
        """
        last_stool = self.get_model().get_stool(-1)
        return len(last_stool) == self.get_model().number_of_cheeses()


def _instruction_filter(cmd):
    """ (str) -> tuple of int or str
    Return a tuple of int (src, dest) stools which converted from user input;
    Return "quit" if the user input "quit";
    Raise exception if caught InvalidInputError exception

    >>> _instruction_filter("move 0 1")
    (0, 1)
    >>> _instruction_filter("quit")
    'quit'
    """
    cmd_list = cmd.lower().split()
    # parse command
    try:
        if (cmd_list[0] == "move"):
            return (int(cmd_list[1]), int(cmd_list[2]))
        elif (cmd_list[0] == "quit"):
            return cmd_list[0]
        else:
            raise InvalidInputError
    # raise InvalidInputError if any exception caught
    except:
        raise InvalidInputError


def _input_filter(num):
    """ (str) -> int
    Return an integer which converted from user input.

    >>> _input_filter("5")
    5
    """
    while(not num.isdigit() or int(num) < 3):
        num = input("Please re-enter an integer that is no less than 3: ")
    return int(num)

if __name__ == '__main__':
    # You should initiate game play here. Your game should be playable by
    # running this file.
    stools = _input_filter(input("Enter the number of stools: "))
    cheeses = _input_filter(input("Enter the number of cheeses: "))

    print("\n" + EQUAL_SEPARATOR)
    print("You have entered %s stools and %s cheeses for this game" %
          (stools, cheeses))
    print(EQUAL_SEPARATOR + "\n")
    print("Game Start!\n")

    game = ConsoleController(cheeses, stools)
    game.play_loop()
