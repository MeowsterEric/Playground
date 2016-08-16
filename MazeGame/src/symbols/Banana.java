package symbols;

public class Banana extends Sprite {

	private int value;

	public Banana(char symbol, int row, int column, int bananaValue) {
		super(symbol, row, column);
		this.value = bananaValue;
	}

	public int getValue() {
		return this.value;
	}

}
