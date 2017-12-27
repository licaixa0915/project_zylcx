import java.io.*;
import java.net.*;
import javax.swing.*;
import java.awt.*;
import java.util.Date;

class HandleASession implements Runnable, TicTacToeConstants {
	private char[][] cell = new char[3][3];
//	private DataInputStream fromPlayer1;
//	private DataOutputStream toPlayer1;
//	private DataInputStream fromPlayer2;
//	private DataOutputStream toPlayer2;
//	private boolean continueToPlay = true;
	private Socket player1;
	private Socket player2;

	public HandleASession(Socket player1, Socket player2) {
		this.player1 = player1;
		this.player2 = player2;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cell[i][j] = ' ';
	}

	private boolean isFull() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (cell[i][j] == ' ')
					return false;
		return true;
	}

	private boolean isWon(char token) {
		for (int i = 0; i < 3; i++)
			if ((cell[i][0] == token) && (cell[i][1] == token)
					&& (cell[i][2] == token))
				return true;
		for (int j = 0; j < 3; j++)
			if ((cell[0][j] == token) && (cell[1][j] == token)
					&& (cell[2][j] == token))
				return true;
		if ((cell[0][0] == token) && (cell[1][1] == token)
				&& (cell[2][2] == token))
			return true;
		if ((cell[0][2] == token) && (cell[1][1] == token)
				&& (cell[2][0] == token))
			return true;
		return false;
	}

	public void run() {
		try {
			DataInputStream fromPlayer1 = new DataInputStream(player1
					.getInputStream());
			DataOutputStream toPlayer1 = new DataOutputStream(player1
					.getOutputStream());
			DataInputStream fromPlayer2 = new DataInputStream(player2
					.getInputStream());
			DataOutputStream toPlayer2 = new DataOutputStream(player2
					.getOutputStream());
			toPlayer1.writeInt(1);
			while (true) {
				int row = fromPlayer1.readInt();
				int column = fromPlayer1.readInt();
				cell[row][column] = 'X';
				if (isWon('X')) {
					toPlayer1.writeInt(PLAYER1_WON);
					toPlayer2.writeInt(PLAYER1_WON);
					sendMove(toPlayer2, row, column);
					break;
				} else if (isFull()) {
					toPlayer1.writeInt(DRAW);
					toPlayer2.writeInt(DRAW);
					sendMove(toPlayer2, row, column);
					break;
				} else {
					toPlayer2.writeInt(CONTIUNE);
					sendMove(toPlayer2, row, column);
				}
				row = fromPlayer2.readInt();
				column = fromPlayer2.readInt();
				cell[row][column] = 'O';
				if (isWon('O')) {
					toPlayer1.writeInt(PLAYER2_WON);
					toPlayer2.writeInt(PLAYER2_WON);
					sendMove(toPlayer1, row, column);
					break;
				} else {
					toPlayer1.writeInt(CONTIUNE);
					sendMove(toPlayer1, row, column);
				}
			}
		} catch (IOException ex) {
			System.err.println(ex);
		}
	}

	private void sendMove(DataOutputStream out, int row, int column)
			throws IOException {
		out.writeInt(row);
		out.writeInt(column);
	}
}

@SuppressWarnings("serial")
public class TicTacToeServer extends JFrame implements TicTacToeConstants {
	public static void main(String[] args) {
//		TicTacToeServer frame = new TicTacToeServer();
		new TicTacToeServer();
	}

	@SuppressWarnings("resource")
	public TicTacToeServer() {
		JTextArea jtaLog = new JTextArea();
		JScrollPane scrollPane = new JScrollPane(jtaLog);
		add(scrollPane, BorderLayout.CENTER);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(400, 300);
		setTitle("TicTacToeServer");
		setVisible(true);

		try {
			ServerSocket serverSocket = new ServerSocket(8000);
			jtaLog.append(new Date() + ": Server started at socket 8000\n");
			int sessionNo = 1;
			while (true) {
				jtaLog.append(new Date()
						+ ": Wait for players to join session " + sessionNo
						+ '\n');
				Socket player1 = serverSocket.accept();
				jtaLog.append(new Date() + ": Player 1 joined session "
						+ sessionNo + '\n');
				jtaLog.append("Player 1's IP address"
						+ player1.getInetAddress().getHostAddress() + '\n');
				new DataOutputStream(player1.getOutputStream())
						.writeInt(PLAYER1);
				Socket player2 = serverSocket.accept();
				jtaLog.append(new Date() + ": Player 2 joined session "
						+ sessionNo + '\n');
				jtaLog.append("Player 2's IP address"
						+ player2.getInetAddress().getHostAddress() + '\n');
				new DataOutputStream(player2.getOutputStream())
						.writeInt(PLAYER2);
				jtaLog.append(new Date() + ": Start a thread for session "
						+ (sessionNo++) + '\n');
				HandleASession task = new HandleASession(player1, player2);
				new Thread(task).start();
			}
		} catch (IOException ex) {
			System.err.println(ex);
		}
	}
}
