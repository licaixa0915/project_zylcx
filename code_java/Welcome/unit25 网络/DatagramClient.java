import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class DatagramClient extends JFrame {
	private class ButtonListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			try {
				Arrays.fill(buf, (byte) 0);
				sendPacket.setData(jtf.getText().trim().getBytes());
				socket.send(sendPacket);
				socket.receive(receivePacket);
				jta.append("Radius is " + jtf.getText().trim() + "\n");
				jta.append("Area received from the server is "
						+ Double.parseDouble(new String(buf).trim()) + '\n');
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}
	}
	public static void main(String[] args) {
		new DatagramClient();
	}
	private InetAddress address;
	private byte[] buf = new byte[256];
	private JTextArea jta = new JTextArea();
	private JTextField jtf = new JTextField();
	private DatagramPacket receivePacket;

	private DatagramPacket sendPacket;

	private DatagramSocket socket;

	public DatagramClient() {
		JPanel p = new JPanel();
		p.setLayout(new BorderLayout());
		p.add(new JLabel("Enter radius"), BorderLayout.WEST);
		p.add(jtf, BorderLayout.CENTER);
		jtf.setHorizontalAlignment(JTextField.RIGHT);

		setLayout(new BorderLayout());
		add(p, BorderLayout.NORTH);
		add(new JScrollPane(jta), BorderLayout.CENTER);
		jta.setEditable(false);
		jtf.addActionListener(new ButtonListener());
		setTitle("DatagramClient");
		setSize(500, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);

		try {
			socket = new DatagramSocket();
			address = InetAddress.getByName("localhost");
			sendPacket = new DatagramPacket(buf, buf.length, address, 8000);
			receivePacket = new DatagramPacket(buf, buf.length);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
