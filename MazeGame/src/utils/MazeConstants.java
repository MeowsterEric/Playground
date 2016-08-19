package utils;

/** Constants used by the MazeGame. */
public class MazeConstants {

	public static final String FILENAME = "/Users/Eric/Github Workspace/Playground/MazeGame/src/maps/maze1.txt";

	public static final String UI_TYPE = "gui"; // must be "text" or "gui"

	public static final class MazeSymbols {

		/** The symbol for Player 1. */
		public static final char P1 = '1';

		/** The symbol for Player 2. */
		public static final char P2 = '2';

		/** The symbol for a hallway. */
		public static final char VACANT = ' ';

		/** The symbol for a visited location. */
		public static final char VISITED = '.';

		/** The symbol for a wall. */
		public static final char WALL = 'X';

		/** The symbol for a banana. */
		public static final char BANANA = 'B';

		/** The symbol for a mobile banana. */
		public static final char MOBILE_BANANA = 'M';

	}

	public static final class PlayerCommands {

		/** The symbol for Player 1 moving left. */
		public static final char P1_LEFT = 'a';

		/** The symbol for Player 1 moving down. */
		public static final char P1_DOWN = 's';

		/** The symbol for Player 1 moving right. */
		public static final char P1_RIGHT = 'd';

		/** The symbol for Player 1 moving up. */
		public static final char P1_UP = 'w';

		/** The symbol for Player 2 moving left. */
		public static final char P2_LEFT = 'j';

		/** The symbol for Player 2 moving down. */
		public static final char P2_DOWN = 'k';

		/** The symbol for Player 2 moving right. */
		public static final char P2_RIGHT = 'l';

		/** The symbol for Player 2 moving up. */
		public static final char P2_UP = 'i';

	}

	/** The score for a banana. */
	public static final int BANANA_SCORE = 1;

	/** The score for a mobile banana. */
	public static final int MOBILE_BANANA_SCORE = 2;

	/** The adjustment for moving left. */
	public static final int LEFT = -1;

	/** The adjustment for moving down. */
	public static final int DOWN = 1;

	/** The adjustment for moving right. */
	public static final int RIGHT = 1;

	/** The adjustment for moving up. */
	public static final int UP = -1;

}
