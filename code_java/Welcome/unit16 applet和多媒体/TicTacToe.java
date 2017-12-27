import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.LineBorder;

@SuppressWarnings("serial")
public class TicTacToe extends JApplet {
	public class Cell extends JPanel {
		private class MouseListener extends MouseAdapter {
			public void mouseClicked(MouseEvent e) {
				if (token == ' ' && whoseTurn != ' ') {
					setToken(whoseTurn);
					if (isWon(whoseTurn)) {
						jlblStatus
								.setText(whoseTurn + " won! The game is over");
						whoseTurn = ' ';
					} else if (isFull()) {
						jlblStatus.setText("Draw! The game is over");
						whoseTurn = ' ';
					} else {
						whoseTurn = (whoseTurn == 'X') ? 'O' : 'X';
						jlblStatus.setText(whoseTurn + "'s tuen");
					}
				}
			}
		}

		private char token = ' ';

		public Cell() {
			setBorder(new LineBorder(Color.BLACK, 1));
			addMouseListener(new MouseListener());
		}

		public char getToken() {
			return token;
		}

		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			if (token == 'X') {
				g.drawLine(10, 10, getWidth() - 10, getHeight() - 10);
				g.drawLine(getWidth() - 10, 10, 10, getHeight() - 10);
			} else if (token == 'O') {
				g.drawOval(10, 10, getWidth() - 20, getHeight() - 20);
			}
		}

		public void setToken(char c) {
			token = c;
			repaint();
		}
	}
	private Cell[][] cells = new Cell[3][3];
	private JButton jbtNew = new JButton("New Game");
	private JLabel jlblStatus = new JLabel("X's turn to play");

	private char whoseTurn = 'X';

	public TicTacToe() {
		JPanel p = new JPanel(new GridLayout(3, 3, 0, 0));
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				p.add(cells[i][j] = new Cell());
		p.setBorder(new LineBorder(Color.RED, 1));
		jlblStatus.setBorder(new LineBorder(Color.YELLOW, 1));
		add(jbtNew, BorderLayout.NORTH);
		add(p, BorderLayout.CENTER);
		add(jlblStatus, BorderLayout.SOUTH);

		// 刷新,重新开始游戏
		jbtNew.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (e.getSource() == jbtNew) {
					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++) {
							cells[i][j].setToken(' ');
							cells[i][j].repaint();
						}
					jlblStatus.setText("X's turn. Restart the game");
					whoseTurn = 'X';
				}
			}
		});
	}

	public boolean isFull() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (cells[i][j].getToken() == ' ')
					return false;
		return true;
	}

	public boolean isWon(char token) {
		for (int i = 0; i < 3; i++)
			if ((cells[i][0].getToken() == token)
					&& (cells[i][1].getToken() == token)
					&& (cells[i][2].getToken() == token))
				return true;
		for (int j = 0; j < 3; j++)
			if ((cells[0][j].getToken() == token)
					&& (cells[1][j].getToken() == token)
					&& (cells[2][j].getToken() == token))
				return true;
		if ((cells[0][0].getToken() == token)
				&& (cells[1][1].getToken() == token)
				&& (cells[2][2].getToken() == token))
			return true;
		if ((cells[0][2].getToken() == token)
				&& (cells[1][1].getToken() == token)
				&& (cells[2][0].getToken() == token))
			return true;
		return false;
	}
}
