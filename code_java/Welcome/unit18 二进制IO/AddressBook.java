import java.io.*;
import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.*;

@SuppressWarnings("serial")
public class AddressBook extends JFrame {
	final static int STATE_SIZE = 2;
	final static int STREET_SIZE = 32;
	final static int ZIP_SIZE = 5;
	final static int CITY_SIZE = 20;
	final static int NAME_SIZE = 32;
	final static int RECORD_SIZE = (NAME_SIZE + STREET_SIZE + CITY_SIZE
			+ STATE_SIZE + ZIP_SIZE);

	public static void main(String[] args) {
		AddressBook frame = new AddressBook();
		frame.pack();
		frame.setTitle("AddressBook");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	private JButton jbtAdd = new JButton("Add");
	private JButton jbtFirst = new JButton("First");
	private JButton jbtLast = new JButton("Last");
	private JButton jbtNext = new JButton("Next");
	private JButton jbtPrevious = new JButton("Previous");
	private JTextField jtfCity = new JTextField(CITY_SIZE);
	private JTextField jtfName = new JTextField(NAME_SIZE);
	private JTextField jtfState = new JTextField(STATE_SIZE);
	private JTextField jtfStreet = new JTextField(STREET_SIZE);
	private JTextField jtfZip = new JTextField(ZIP_SIZE);

	private RandomAccessFile raf;

	public AddressBook() {
		try {
			raf = new RandomAccessFile("address.dat", "rw");
		} catch (IOException ex) {
			System.out.print("Error: " + ex);
			System.exit(0);
		}
		// Panel p1 for holding labels Name, Street, and City
		JPanel p1 = new JPanel();
		p1.setLayout(new GridLayout(3, 1));
		p1.add(new JLabel("Name"));
		p1.add(new JLabel("Street"));
		p1.add(new JLabel("City"));
		// Panel jpState for holding state
		JPanel jpState = new JPanel();
		jpState.setLayout(new BorderLayout());
		jpState.add(new JLabel("State"), BorderLayout.WEST);
		jpState.add(jtfState, BorderLayout.CENTER);
		// Panel jpZip for holding zip
		JPanel jpZip = new JPanel();
		jpZip.setLayout(new BorderLayout());
		jpZip.add(new JLabel("Zip"), BorderLayout.WEST);
		jpZip.add(jtfZip, BorderLayout.CENTER);
		// Panel p2 for hold jpState and jpZip
		JPanel p2 = new JPanel();
		p2.setLayout(new BorderLayout());
		p2.add(jpState, BorderLayout.WEST);
		p2.add(jpZip, BorderLayout.CENTER);
		// Panel p3 for holding jtfCity and p2
		JPanel p3 = new JPanel();
		p3.setLayout(new BorderLayout());
		p3.add(jtfCity, BorderLayout.CENTER);
		p3.add(p2, BorderLayout.EAST);
		// Panel p4 for holding jftName, jtfStreet, and p3
		JPanel p4 = new JPanel();
		p4.setLayout(new GridLayout(3, 1));
		p4.add(jtfName);
		p4.add(jtfStreet);
		p4.add(p3);
		// P;ace p1 and p4 into jpAddress
		JPanel jpAddress = new JPanel(new BorderLayout());
		jpAddress.add(p1, BorderLayout.WEST);
		jpAddress.add(p4, BorderLayout.CENTER);
		// Set the panel with line border
		jpAddress.setBorder(new BevelBorder(BevelBorder.RAISED));
		// Add buttons to a panel
		JPanel jpButton = new JPanel();
		jpButton.add(jbtAdd);
		jpButton.add(jbtFirst);
		jpButton.add(jbtNext);
		jpButton.add(jbtPrevious);
		jpButton.add(jbtLast);
		// add jpAddress and jpButton to the frame
		add(jpAddress, BorderLayout.CENTER);
		add(jpButton, BorderLayout.SOUTH);

		jbtAdd.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				writeAddress();
			}
		});

		jbtFirst.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					if (raf.length() > 0)
						readAddress(0);
				} catch (IOException ex) {
					ex.printStackTrace();
				}
			}
		});

		jbtNext.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					long currentPosition = raf.getFilePointer();
					if (currentPosition < raf.length())
						readAddress(currentPosition);
				} catch (IOException ex) {
					ex.printStackTrace();
				}
			}
		});

		jbtPrevious.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					long currentPosition = raf.getFilePointer();
					if (currentPosition - 2 * RECORD_SIZE > 0)
						readAddress(currentPosition - 2 * 2 * RECORD_SIZE);
					else
						readAddress(0);
				} catch (IOException ex) {
					ex.printStackTrace();
				}
			}
		});

		jbtLast.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					long lastPosition = raf.getFilePointer();
					if (lastPosition > 0)
						readAddress(lastPosition - 2 * RECORD_SIZE);
				} catch (IOException ex) {
					ex.printStackTrace();
				}
			}
		});
		// Display the first record if exists
		try {
			if (raf.length() > 0)
				readAddress(0);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public void readAddress(long position) throws IOException {
		raf.seek(position);
		String name = FixedLengthStringIO.readFixedLengthString(NAME_SIZE, raf);
		String street = FixedLengthStringIO.readFixedLengthString(STREET_SIZE,
				raf);
		String city = FixedLengthStringIO.readFixedLengthString(CITY_SIZE, raf);
		String state = FixedLengthStringIO.readFixedLengthString(STATE_SIZE,
				raf);
		String zip = FixedLengthStringIO.readFixedLengthString(ZIP_SIZE, raf);
		jtfName.setText(name);
		jtfStreet.setText(street);
		jtfCity.setText(city);
		jtfState.setText(state);
		jtfZip.setText(zip);
	}

	public void writeAddress() {
		try {
			raf.seek(raf.length()); // 设置到此文件开头测量到的文件指针偏移量，在该位置发生下一个读取或写入操作
			FixedLengthStringIO.writeFixedLengthString(jtfName.getText(),
					NAME_SIZE, raf);
			FixedLengthStringIO.writeFixedLengthString(jtfStreet.getText(),
					STREET_SIZE, raf);
			FixedLengthStringIO.writeFixedLengthString(jtfCity.getText(),
					CITY_SIZE, raf);
			FixedLengthStringIO.writeFixedLengthString(jtfState.getText(),
					STATE_SIZE, raf);
			FixedLengthStringIO.writeFixedLengthString(jtfZip.getText(),
					ZIP_SIZE, raf);
		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}
}
