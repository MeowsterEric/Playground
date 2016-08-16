package mazeGame;

public interface Grid<T> {

	public void setCell(int row, int col, T item);

	public T getCell(int rwo, int col);

	public int getNumRows();

	public int getNumCols();

	public boolean equals(Grid<T> other);

	public String toString();

}
