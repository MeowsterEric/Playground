package game;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Random;

import symbols.Banana;
import symbols.MobileBanana;
import symbols.Monkey;
import symbols.Sprite;
import symbols.UnvisitedHallway;
import symbols.VisitedHallway;
import symbols.Wall;
import utils.MazeConstants;
import utils.MazeConstants.MazeSymbols;

/**
 * A class that represents the basic functionality of the maze game. This class is responsible for performing the
 * following operations:
 * 
 * 1. At creation, it initializes the instance variables used to store the current state of the game.
 * 
 * 2. When a move is specified, it checks if it is a legal move and makes the move if it is legal.
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
		this.bananas = new ArrayList<Banana>();
		setupMaze(layoutFileName);
	}

	private void setupMaze(String layoutFileName) throws IOException {
		// read the maze layout file
		BufferedReader br = new BufferedReader(new FileReader(layoutFileName));
		br.mark(0); // mark the starting point of the file

		String line = br.readLine();
		int numCols = line.length(); // get the number of columns
		int numRows = 0;
		while (line != null && line != "") { // find the number of rows
			numRows++;
			line = br.readLine();
		}
		br.reset(); // return to starting point

		this.maze = new ArrayGrid<Sprite>(numRows, numCols);
		for (int i = 0; i < numRows; i++) {
			line = br.readLine();
			for (int j = 0; j < line.length(); j++) {
				switch (line.charAt(j)) {
				case MazeSymbols.P1:
					maze.setCell(i, j, new Monkey(MazeSymbols.P1, i, j, 0, 0));
					break;
				case MazeSymbols.P2:
					maze.setCell(i, j, new Monkey(MazeSymbols.P2, i, j, 0, 0));
					break;
				case MazeSymbols.VACANT:
					maze.setCell(i, j, new UnvisitedHallway(MazeSymbols.VACANT, i, j));
					break;
				case MazeSymbols.VISITED:
					maze.setCell(i, j, new VisitedHallway(MazeSymbols.VISITED, i, j));
					break;
				case MazeSymbols.WALL:
					maze.setCell(i, j, new Wall(MazeSymbols.WALL, i, j));
					break;
				case MazeSymbols.BANANA:
					Banana b = new Banana(MazeSymbols.BANANA, i, j, MazeConstants.BANANA_SCORE);
					maze.setCell(i, j, b);
					this.bananas.add(b);
					break;
				case MazeSymbols.MOBILE_BANANA:
					MobileBanana mb = new MobileBanana(MazeSymbols.MOBILE_BANANA, i, j, MazeConstants.MOBILE_BANANA_SCORE);
					maze.setCell(i, j, mb);
					this.bananas.add(mb);
					break;
				default:
					throw new NoSuchElementException("No such symbol.");
				}
			}
		}

		br.close();
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
