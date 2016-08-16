package symbols;

import utils.Moveable;

public class MobileBanana extends Banana implements Moveable {

	public MobileBanana(char symbol, int row, int column, int bananaValue) {
		super(symbol, row, column, bananaValue);
	}

	@Override
	public void move(int row, int column) {
		this.row += row;
		this.column += column;
	}

}
