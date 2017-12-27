import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
public class ComboBoxDemo extends JFrame {
	public static void main(String[] args) {
		ComboBoxDemo frame = new ComboBoxDemo();
		frame.pack();
		frame.setTitle("ComboBoxDemo");
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
	private DescriptionPanel descriptionPanel = new DescriptionPanel();
	private String[] flagDescriptioon = new String[9];
	private ImageIcon[] flagImage = { new ImageIcon("image/ca.gif"),
			new ImageIcon("image/china.gif"),
			new ImageIcon("image/denmark.gif"), new ImageIcon("image/fr.gif"),
			new ImageIcon("image/germany.gif"),
			new ImageIcon("image/india.gif"),
			new ImageIcon("image/norway.gif"), new ImageIcon("image/uk.gif"),
			new ImageIcon("image/us.gif") };
	private String[] flagTitles = { "Canada", "China", "Denmark", "France",
			"Germany", "India", "Norway", "United Kingdom",
			"United State of America" };

	@SuppressWarnings({ "rawtypes", "unchecked" })
	private JComboBox jcbo = new JComboBox(flagTitles);

	public ComboBoxDemo() {
		flagDescriptioon[0] = "The Maple Leaf flag \n\n"
				+ "The Canadian National Flag was adopted by the Candian "
				+ "Parliament on October 22, 1964 and was proclaimed into law "
				+ "by Her Majesty Queen Elizabeth II (the Queen of Canada) on "
				+ "February 15, 1965. The Canadian Flag (colloquially known "
				+ "as The Maple Leaf Flag) is a red flag of the proportions "
				+ "two by length and one by width, containing in its center a "
				+ "mapleleaf centered in the white square.";
		flagDescriptioon[1] = "Description for China ... ";
		flagDescriptioon[2] = "Description for Denmark ... ";
		flagDescriptioon[3] = "Description for France ... ";
		flagDescriptioon[4] = "Description for Germany ... ";
		flagDescriptioon[5] = "Description for India ... ";
		flagDescriptioon[6] = "Description for Norway ... ";
		flagDescriptioon[7] = "Description for UK ... ";
		flagDescriptioon[8] = "Description for US ... ";

		setDisplay(0);
		add(jcbo, BorderLayout.NORTH);
		add(descriptionPanel, BorderLayout.CENTER);

		jcbo.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				setDisplay(jcbo.getSelectedIndex());
			}
		});
		// jcbo.addActionListener(new ActionListener() {
		// public void actionPerformed(ActionEvent e) {
		// setDisplay(jcbo.getSelectedIndex());
		// }
		// });
	}

	public void setDisplay(int index) {
		descriptionPanel.setTitle(flagTitles[index]);
		descriptionPanel.setImageIcon(flagImage[index]);
		descriptionPanel.setDescription(flagDescriptioon[index]);
	}
}
