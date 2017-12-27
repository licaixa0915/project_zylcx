import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class Client extends JFrame {
	private class ButtonListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			try {
				double radius = Double.parseDouble(jtf.getText().trim());
				toServer.writeDouble(radius);
				toServer.flush();
				double area = fromServer.readDouble();
				jta.append("Radius is " + radius + "\n");
				jta.append("Area received from the server is " + area + "\n");
			} catch (IOException ex) {
				System.err.println(ex);
			}
		}
	}
	public static void main(String[] args) {
		new Client();
	}
	private DataInputStream fromServer;
	private JTextArea jta = new JTextArea();

	private JTextField jtf = new JTextField();

	private DataOutputStream toServer;

	@SuppressWarnings("resource")
	public Client() {
		JPanel p = new JPanel();
		p.setLayout(new BorderLayout());
		p.add(new JLabel("Enter radius"), BorderLayout.WEST);
		p.add(jtf, BorderLayout.CENTER);
		jtf.setHorizontalAlignment(JTextField.RIGHT);
		jta.setEditable(false);
		setLayout(new BorderLayout());
		add(p, BorderLayout.NORTH);
		add(new JScrollPane(jta), BorderLayout.CENTER);
		jtf.addActionListener(new ButtonListener());
		setTitle("Client");
		setSize(500, 300);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);

		try {
			Socket socket = new Socket("localhost", 8000);
			fromServer = new DataInputStream(socket.getInputStream());
			toServer = new DataOutputStream(socket.getOutputStream());
		} catch (IOException ex) {
			jta.append(ex.toString() + '\n');
		}
	}
}
