package game;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import symbols.Banana;
import symbols.MobileBanana;
import symbols.Monkey;
import symbols.Sprite;
import symbols.UnvisitedHallway;
import symbols.VisitedHallway;
import symbols.Wall;
import utils.MazeConstants;

/**
 * A class that represents the basic functionality of the maze game. This class
 * is responsible for performing the following operations:
 * 
 * 1. At creation, it initializes the instance variables used to store the
 * current state of the game.
 * 
 * 2. When a move is specified, it checks if it is a legal move and makes the
 * move if it is legal.
 * 
 * 3. It reports information about the current state of the game when asked.
 */
public class MazeGame {

	/** A random number generator to move the MobileBananas. */
	private Random random;

	private Grid<Sprite> maze;
	private List<Banana> bananas;
	private Monkey player1;
	private Monkey player2;

	public MazeGame(String layoutFileName) throws IOException {
		this.random = new Random();
		int[] dimension = getDimensions(layoutFileName);
		this.maze = new ArrayGrid<Sprite>(dimension[0], dimension[1]);
		this.bananas = new ArrayList<Banana>();
	}

	private int[] getDimensions(String layoutFileName) throws IOException {
		// read the maze layout file
		BufferedReader br = new BufferedReader(new FileReader(layoutFileName));

		String line = br.readLine();
		int numCols = line.length(); // get the number of columns
		int numRows = 0;
		while (line != null && line != "") { // find the number of rows
			numRows++;
			line = br.readLine();
		}

		br.close();
		return new int[] { numRows, numCols };
	}

	private void setupMaze() {

	}

	public Grid<Sprite> getMaze() {
		return this.maze;
	}

	public Monkey getPlayerOne() {
		return this.player1;
	}

	public Monkey getPlayerTwo() {
		return this.player2;
	}

	public int getNumRows() {
		return this.maze.getNumRows();
	}

	public int getNumCols() {
		return this.maze.getNumCols();
	}

	public Sprite get(int i, int j) {
		return this.maze.getCell(i, j);
	}

	public void move(char nextMove) {
		// TODO
	}

	public int hasWon() {
		// TODO
		return 0;
	}

	public boolean isBlocked() {
		// TODO
		return false;
	}

}
