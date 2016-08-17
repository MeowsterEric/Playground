package game;

import symbols.Sprite;

public class ArrayGrid<T> implements Grid<Sprite> {

	private final int numRows;
	private final int numCols;
	private Sprite[][] cell;

	public ArrayGrid(int numRows, int numCols) {
		this.numRows = numRows;
		this.numCols = numCols;
		this.cell = new Sprite[numRows][numCols];
	}

	@Override
	public void setCell(int row, int col, Sprite item) {
		this.cell[row][col] = item;
	}

	@Override
	public Sprite getCell(int row, int col) {
		return this.cell[row][col];
	}

	@Override
	public int getNumRows() {
		return this.numRows;
	}

	@Override
	public int getNumCols() {
		return this.numCols;
	}

	@Override
	public boolean equals(Grid<Sprite> other) {
		return this.cell.equals(other);
	}

	@Override
	public String toString() {
		String strRep = "";
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++)
				strRep += cell[i][j];
			strRep += "\n";
		}

		return strRep;
	}

}
