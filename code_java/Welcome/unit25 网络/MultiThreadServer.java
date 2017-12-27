import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class MultiThreadServer extends JFrame {
	class HandleAClient implements Runnable {
		private int clientNo;
		private Socket socket;

		public HandleAClient(Socket socket, int clientNo) {
			this.socket = socket;
			this.clientNo = clientNo;
		}

		public void run() {
			try {
				DataInputStream inputFromClient = new DataInputStream(socket
						.getInputStream());
				DataOutputStream outputToClient = new DataOutputStream(socket
						.getOutputStream());
				while (true) {
					double radius = inputFromClient.readDouble();
					double area = radius * radius * Math.PI;
					outputToClient.writeDouble(area);
					jta.append("radius received from client" + clientNo+ ": " + radius + '\n');
					jta.append("Area found: " + area + '\n');
				}
			} catch (IOException e) {
				System.err.println(e);
			}
		}
	}

	public static void main(String[] args) {
		new MultiThreadServer();
	}

	private JTextArea jta = new JTextArea();

	@SuppressWarnings("resource")
	public MultiThreadServer() {
		setLayout(new BorderLayout());
		add(new JScrollPane(jta), BorderLayout.CENTER);
		jta.setEditable(false);
		setTitle("MultiThreadServer");
		setSize(500, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);

		try {
			ServerSocket serverSocket = new ServerSocket(8000);
			jta.append("MultiThreadServer started at " + new Date() + '\n');
			int clientNo = 1;
			while (true) {
				Socket socket = serverSocket.accept();
				jta.append("Starting thread for client " + clientNo + " at "
						+ new Date() + '\n');
				InetAddress inetAddress = socket.getInetAddress();
				jta.append("Client " + clientNo + "'s host name is "
						+ inetAddress.getHostName() + "\n");
				jta.append("Client " + clientNo + "'s IP Address is "
						+ inetAddress.getHostAddress() + "\n");
				HandleAClient task = new HandleAClient(socket, clientNo);
				new Thread(task).start();
				clientNo++;
			}
		} catch (IOException ex) {
			System.err.println(ex);
		}
	}
}
