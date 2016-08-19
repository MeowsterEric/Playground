package game;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
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
import utils.MazeConstants.PlayerCommands;

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
	private ArrayGrid<Sprite> maze;
	private ArrayList<Banana> bananas;
	private Monkey player1;
	private Monkey player2;

	public MazeGame(String layoutFileName) throws IOException {
		this.random = new Random();
		this.bananas = new ArrayList<Banana>();
		this.maze = buildMaze(layoutFileName);
	}

	private ArrayGrid<Sprite> buildMaze(String layoutFileName) throws IOException {
		// read the maze layout file
		BufferedReader br = new BufferedReader(new FileReader(layoutFileName));
		ArrayList<String> layoutContent = new ArrayList<String>();
		String line;

		while ((line = br.readLine()) != null && line != "") {
			layoutContent.add(line);
		}
		br.close();

		return buildMazeLayout(layoutContent);
	}

	private ArrayGrid<Sprite> buildMazeLayout(ArrayList<String> layoutContent) {
		ArrayGrid<Sprite> mazeMap = new ArrayGrid<Sprite>(layoutContent.size(), layoutContent.get(0).length());

		for (int i = 0; i < mazeMap.getNumRows(); i++) {
			for (int j = 0; j < mazeMap.getNumColumns(); j++) {
				switch (layoutContent.get(i).charAt(j)) {
				case MazeSymbols.P1:
					mazeMap.setCell(i, j, (this.player1 = new Monkey(MazeSymbols.P1, i, j, 0, 0)));
					break;
				case MazeSymbols.P2:
					mazeMap.setCell(i, j, (this.player2 = new Monkey(MazeSymbols.P2, i, j, 0, 0)));
					break;
				case MazeSymbols.VACANT:
					mazeMap.setCell(i, j, new UnvisitedHallway(MazeSymbols.VACANT, i, j));
					break;
				case MazeSymbols.VISITED:
					mazeMap.setCell(i, j, new VisitedHallway(MazeSymbols.VISITED, i, j));
					break;
				case MazeSymbols.WALL:
					mazeMap.setCell(i, j, new Wall(MazeSymbols.WALL, i, j));
					break;
				case MazeSymbols.BANANA:
					Banana b = new Banana(MazeSymbols.BANANA, i, j, MazeConstants.BANANA_SCORE);
					mazeMap.setCell(i, j, b);
					this.bananas.add(b);
					break;
				case MazeSymbols.MOBILE_BANANA:
					MobileBanana mb = new MobileBanana(MazeSymbols.MOBILE_BANANA, i, j, MazeConstants.MOBILE_BANANA_SCORE);
					mazeMap.setCell(i, j, mb);
					this.bananas.add(mb);
					break;
				// case: //more symbols goes here in the future.
				// break;
				default:
					return null;
				}
			}
		}

		return mazeMap;
	}

	public ArrayGrid<Sprite> getMaze() {
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

	public int getNumColumns() {
		return this.maze.getNumColumns();
	}

	public Sprite get(int i, int j) {
		return this.maze.getCell(i, j);
	}

	public void move(char nextMove) {

		if (isNextMoveValid(nextMove) && isNextMoveVacant(nextMove)) {
			switch (nextMove) {
			case PlayerCommands.P1_UP:
			case PlayerCommands.P1_DOWN:
			case PlayerCommands.P1_LEFT:
			case PlayerCommands.P1_RIGHT:
				movePlayer(this.player1, nextMove);
				break;
			case PlayerCommands.P2_UP:
			case PlayerCommands.P2_DOWN:
			case PlayerCommands.P2_LEFT:
			case PlayerCommands.P2_RIGHT:
				movePlayer(this.player2, nextMove);
				break;
			default:
				return;
			}
		}

	}

	private boolean isNextMoveValid(char nextMove) {
		// TODO Auto-generated method stub
		return false;
	}

	private boolean isNextMoveVacant(char nextMove) {

		return false;
	}

	private void movePlayer(Monkey player, char nextMove) {
		int row = player.getRow();
		int col = player.getColumn();

		switch (nextMove) {
		case PlayerCommands.P1_UP:
		case PlayerCommands.P2_UP:
			row += MazeConstants.UP;
			break;
		case PlayerCommands.P1_DOWN:
		case PlayerCommands.P2_DOWN:
			row += MazeConstants.DOWN;
			break;
		case PlayerCommands.P1_LEFT:
		case PlayerCommands.P2_LEFT:
			col += MazeConstants.LEFT;
			break;
		case PlayerCommands.P1_RIGHT:
		case PlayerCommands.P2_RIGHT:
			col += MazeConstants.RIGHT;
			break;
		default:
			return; // no move
		}
		moveAndSetPlayerOnGrid(player, row, col);
	}

	private void moveAndSetPlayerOnGrid(Monkey player, int nextRow, int nextColumn) {
		Sprite banana;
		int row, col;

		if ((banana = get(nextRow, nextColumn)) instanceof Banana) {
			player.eatBanana(((Banana) banana).getValue());
			this.bananas.remove(banana);
		}

		maze.setCell(nextRow, nextColumn, player);
		maze.setCell((row = player.getRow()), (col = player.getColumn()), new VisitedHallway(MazeConstants.MazeSymbols.VISITED, row, col));
		player.move(nextRow, nextColumn);
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
