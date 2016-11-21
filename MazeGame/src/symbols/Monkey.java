package symbols;

import utils.Moveable;

public class Monkey extends Sprite implements Moveable {

	private int score;
	private int moves;
	private int nextRow, nextCol;

	public Monkey(char symbol, int row, int column, int score, int moves) {
		super(symbol, row, column);
		this.score = score;
		this.moves = moves;

		this.nextRow = -1;
		this.nextCol = -1;
	}

	public void eatBanana(int bananaScore) {
		this.score += bananaScore;
	}

	public int getScore() {
		return this.score;
	}

	public int getNumMoves() {
		return this.moves;
	}

	public int getNextRow() {
		return this.nextRow;
	}

	public int getNextCol() {
		return this.nextCol;
	}

	public void setNextMove(int nextRow, int nextCol) {
		this.nextRow = nextRow;
		this.nextCol = nextCol;
	}

	public void move() {
		this.row = this.nextRow;
		this.column = this.nextCol;
		this.moves++;
		this.nextRow = -1;
		this.nextCol = -1;
	}

	@Override
	public void move(int newRow, int newColumn) {
		this.row = newRow;
		this.column = newColumn;
		this.moves++;

		this.nextRow = -1;
		this.nextCol = -1;
	}

}
