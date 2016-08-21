package game;

import java.io.IOException;

import ui.GUI;
import ui.TextUI;
import ui.UI;
import utils.MazeConstants;

public class Play {

	/**
	 * The main method.
	 *
	 * @param args
	 *            the arguments
	 * @throws IOException
	 *             Signals that an I/O exception has occurred.
	 */
	public static void main(String[] args) throws IOException {
		MazeGame game = new MazeGame(MazeConstants.FILENAME);
		System.out.println(game.getMaze().toString());

		UI gameUI;
		if (MazeConstants.UI_TYPE.equals("text"))
			gameUI = new TextUI(game);
		else
			gameUI = new GUI(game);

		gameUI.launchGame();
		gameUI.displayWinner();
	}

}
