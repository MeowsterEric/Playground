package symbols;

import utils.Moveable;

public class Monkey extends Sprite implements Moveable {

	private int score;
	private int moves;

	public Monkey(char symbol, int row, int column, int score, int moves) {
		super(symbol, row, column);
		this.score = score;
		this.moves = moves;
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

	@Override
	public void move(int newRow, int newColumn) {
		this.row = newRow;
		this.column = newColumn;
		this.moves++;
	}

}
