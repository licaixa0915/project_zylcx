import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import javax.swing.*;

@SuppressWarnings("serial")
public class ViewRemoteFile extends JApplet {
	private JButton jbtView = new JButton("View");
	private JLabel jlblStatus = new JLabel();
	private JTextArea jtaFile = new JTextArea();
	private JTextField jtfURL = new JTextField(12);

	public void init() {
		JPanel p1 = new JPanel();
		p1.setLayout(new BorderLayout());
		p1.add(new JLabel("Filename"), BorderLayout.WEST);
		p1.add(jtfURL, BorderLayout.CENTER);
		p1.add(jbtView, BorderLayout.EAST);

		setLayout(new BorderLayout());
		add(new JScrollPane(jtaFile), BorderLayout.CENTER);
		add(p1, BorderLayout.NORTH);
		add(jlblStatus, BorderLayout.SOUTH);

		jbtView.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				showFile();
			}
		});
	}

	private void showFile() {
		BufferedReader infile = null;
		URL url = null;

		try {
			url = new URL(jtfURL.getText().trim());
			InputStream is = url.openStream(); // 打开到此 URL 的连接并返回一个用于从该连接读入的InputStream
			infile = new BufferedReader(new InputStreamReader(is));
			String inLine;
			while ((inLine = infile.readLine()) != null) {
				jtaFile.append(inLine + '\n');
			}
			jlblStatus.setText("File loaded successfully");
		} catch (FileNotFoundException e) {
			jlblStatus.setText("URL " + url + " not found.");
		} catch (IOException e) {
			jlblStatus.setText(e.getMessage());
		} finally {
			try {
				if (infile != null)
					infile.close();
			} catch (IOException e) {
			}
		}
	}
}
