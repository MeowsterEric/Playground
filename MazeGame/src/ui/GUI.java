package ui;

import java.awt.*;
import javax.swing.*;

import game.MazeGame;

/** A simple GUI for the Maze Game. */
public class GUI extends JFrame implements UI {

	private static final long serialVersionUID = 1L;
	private MazeGame game;
	private JLabel[][] tile;

	/**
	 * Initializes a GUI for the given MazeGame.
	 * 
	 * @param game
	 *            The MazeGame of this GUI
	 */
	public GUI(MazeGame game) {
		this.game = game;
	}

	/**
	 * Returns the MazeGame of this GUI.
	 * 
	 * @return the MazeGame of this GUI
	 */
	public MazeGame getGame() {
		return game;
	}

	@Override
	public void launchGame() {
		int numRows = game.getNumRows();
		int numCols = game.getNumColumns();

		Container c = this.getContentPane();
		c.setLayout(new GridLayout(numRows, numCols));
		tile = new JLabel[numRows][numCols];

		GUIListener listener = new GUIListener(this);
		this.addKeyListener(listener);

		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				tile[i][j] = new JLabel();
				tile[i][j].setText("" + game.get(i, j).toString());
				tile[i][j].setFont(new Font(null, Font.BOLD, 18));
				tile[i][j].setHorizontalAlignment(SwingConstants.CENTER);
				tile[i][j].setVerticalAlignment(SwingConstants.CENTER);
				c.add(tile[i][j]);
			}
		}
		setVisible(true);
		pack();
	}

	@Override
	public void displayWinner() {

		int won = game.hasWon();
		int moves = 0;
		String message;

		if (game.isBlocked()) { // no winners
			message = "Game over! Both players are stuck.";
		} else {
			if (won == 0)
				return; // game is still on
			else if (won == 1)
				message = "Congratulations Player 1! You won the maze in " + game.getPlayerOne().getNumMoves()
						+ " moves.";
			else if (won == 2)
				message = "Congratulations Player 2! You won the maze in " + game.getPlayerTwo().getNumMoves()
						+ " moves.";
			else // it's a tie
				message = "It's a tie!";
		}
		JOptionPane.showMessageDialog(null, message);
		setVisible(false);
	}

	/**
	 * Update the grid display.
	 */
	public void updateLabels() {
		int numRows = game.getNumRows();
		int numCols = game.getNumColumns();

		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				tile[i][j].setText(game.get(i, j).toString());
			}
		}
		displayWinner();
	}

}