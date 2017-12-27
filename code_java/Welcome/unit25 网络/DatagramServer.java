import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class DatagramServer extends JFrame {
	public static void main(String[] args) {
		new DatagramServer();
	}
	private byte[] buf = new byte[256];

	private JTextArea jta = new JTextArea();

	@SuppressWarnings("resource")
	public DatagramServer() {
		setLayout(new BorderLayout());
		add(new JScrollPane(jta), BorderLayout.CENTER);
		jta.setEditable(false);
		setTitle("DatagramServer");
		setSize(300, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);
		
		try {
			DatagramSocket socket = new DatagramSocket(8000);
			jta.append("Server started at " + new Date() + '\n');
			DatagramPacket receivePacket = new DatagramPacket(buf, buf.length);
			DatagramPacket sendPacket = new DatagramPacket(buf, buf.length);

			while (true) {
				Arrays.fill(buf, (byte) 0);
				socket.receive(receivePacket);
				jta.append("The client host name is "
						+ receivePacket.getAddress().getHostName()
						+ " and port number is " + receivePacket.getPort()
						+ '\n');
				jta.append("Radius receive from client is "
						+ new String(buf).trim() + '\n');
				double radius = Double.parseDouble(new String(buf).trim());
				double area = radius * radius * Math.PI;
				jta.append("Area is " + area + '\n');
				sendPacket.setAddress(receivePacket.getAddress());
				sendPacket.setPort(receivePacket.getPort());
				sendPacket.setData(new Double(area).toString().getBytes());
				socket.send(sendPacket);
			}
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
