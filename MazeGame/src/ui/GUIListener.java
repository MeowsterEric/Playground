package ui;

import java.awt.event.*;

public class GUIListener extends KeyAdapter {

	private GUI window;

	public GUIListener(GUI window) {
		this.window = window;
	}

	@Override
	public void keyPressed(KeyEvent event) {
	}

	@Override
	public void keyTyped(KeyEvent event) {
		char nextMove = event.getKeyChar();
		this.window.getGame().move(nextMove);
		this.window.updateLabels();
	}

	@Override
	public void keyReleased(KeyEvent event) {
	}

}
