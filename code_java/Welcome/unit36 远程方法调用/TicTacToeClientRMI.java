import java.rmi.*;
import java.rmi.registry.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

@SuppressWarnings("serial")
public class TicTacToeClientRMI extends JApplet {
	private char marker;
	private boolean myTurn = false;
	private Cell[][] cell;
	private TicTacToeInterface ticTacToe;
	private Border lineBorder = BorderFactory.createLineBorder(Color.yellow, 1);
	private JLabel jlblStatues = new JLabel("jLabel1");
	private JLabel jlblIdentification = new JLabel();
	boolean isStandalone = false;

	public void init() {
		JPanel jPanel1 = new JPanel();
		jPanel1.setBorder(lineBorder);
		jPanel1.setLayout(new GridLayout(3, 3, 1, 1));

		add(jlblStatues, BorderLayout.SOUTH);
		add(jPanel1, BorderLayout.CENTER);
		add(jlblIdentification, BorderLayout.NORTH);

		cell = new Cell[3][3];
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				jPanel1.add(cell[i][j] = new Cell(i, j));

		try {
			initializeRMI();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	protected boolean initializeRMI() throws Exception {
		String host = "";
		if (!isStandalone)
			host = getCodeBase().getHost();

		try {
			Registry registry = LocateRegistry.getRegistry(host);
			ticTacToe = (TicTacToeInterface) registry.lookup("TicTacToeImpl");
			System.out.println("Server object " + ticTacToe + " found");
		} catch (Exception ex) {
			System.out.println(ex);
		}

		CallBackImpl callBackControl = new CallBackImpl(this);
		if ((marker = ticTacToe.connect((CallBack) callBackControl)) != ' ') {
			System.out.println("connected ad " + marker + " player.");
			jlblIdentification.setText("You are player " + marker);
			return true;
		} else {
			System.out.println("already two players connected as ");
			return false;
		}
	}

	public void setMyTurn(boolean myTurn) {
		this.myTurn = myTurn;
	}

	public void setMessage(String message) {
		jlblStatues.setText(message);
	}

	public void mark(int row, int column, char token) {
		cell[row][column].setToken(token);
	}

	private class Cell extends JPanel {
		private boolean marked = false;
		@SuppressWarnings("unused")
		int row, column;
		private char token;

		public Cell(final int row, final int column) {
			this.row = row;
			this.column = column;
			addMouseListener(new MouseAdapter() {
				public void mouseClicked(MouseEvent e) {
					if (myTurn && !marked) {
						setToken(marker);
						try {
							ticTacToe.myMove(row, column, marker);
						} catch (RemoteException ex) {
							System.out.println(ex);
						}
					}
				}
			});
			setBorder(lineBorder);
		}

		public void setToken(char c) {
			token = c;
			marked = true;
			repaint();
		}

		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			g.drawRect(0, 0, getSize().width, getSize().height);
			if (token == 'X') {
				g.drawLine(10, 10, getSize().width - 10, getSize().height - 10);
				g.drawLine(getSize().width - 10, 10, 10, getSize().height - 10);
			} else if (token == 'O')
				g.drawOval(10, 10, getSize().width - 20, getSize().height - 20);
		}
	}

	public static void main(String[] args) {
		TicTacToeClientRMI applet = new TicTacToeClientRMI();
		applet.isStandalone = true;
		applet.init();
		applet.start();
		JFrame frame = new JFrame();
		frame.add(applet, BorderLayout.CENTER);
		frame.setSize(400, 320);
		frame.setTitle("TicTacToeClientRMI");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
