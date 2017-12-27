import java.awt.*;
import javax.swing.*;

@SuppressWarnings("serial")
class DescriptionPanel extends JPanel {
	private JLabel jlblImageTitle = new JLabel();
	private JTextArea jtaDescription = new JTextArea();

	public DescriptionPanel() {
		jlblImageTitle.setHorizontalAlignment(JLabel.CENTER);
		jlblImageTitle.setHorizontalTextPosition(JLabel.CENTER);
		jlblImageTitle.setVerticalTextPosition(JLabel.BOTTOM); // BOTTOM:下方
		jlblImageTitle.setFont(new Font("ScansSerif", Font.BOLD, 16));
		jtaDescription.setFont(new Font("Serif", Font.PLAIN, 14));
		jtaDescription.setLineWrap(true);
		jtaDescription.setWrapStyleWord(true);
		jtaDescription.setEditable(false);
		JScrollPane scrollPane = new JScrollPane(jtaDescription); // 滚动条
		setLayout(new BorderLayout(5, 5));
		add(scrollPane, BorderLayout.CENTER);
		add(jlblImageTitle, BorderLayout.WEST);
	}

	public void setDescription(String text) {
		jtaDescription.setText(text);
	}

	public void setImageIcon(ImageIcon icon) {
		jlblImageTitle.setIcon(icon);
	}

	public void setTitle(String title) {
		jlblImageTitle.setText(title);
	}
}

@SuppressWarnings("serial")
public class TextAreaDemo extends JFrame {
	public static void main(String[] args) {
		TextAreaDemo frame = new TextAreaDemo();
		frame.pack();
		frame.setTitle("TextAreaDemo");
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}

	private DescriptionPanel descriptionPanel = new DescriptionPanel();

	public TextAreaDemo() {
		descriptionPanel.setTitle("Canada");
		String description = "The Maple Leaf flag \n\n"
				+ "The Canadian National Flag was adopted by the Candian "
				+ "Parliament on October 22, 1964 and was proclaimed into law "
				+ "by Her Majesty Queen Elizabeth II (the Queen of Canada) on "
				+ "February 15, 1965. The Canadian Flag (colloquially known "
				+ "as The Maple Leaf Flag) is a red flag of the proportions "
				+ "two by length and one by width, containing in its center a "
				+ "mapleleaf centered in the white square.";
		descriptionPanel.setDescription(description);
		descriptionPanel.setImageIcon(new ImageIcon("image/ca.gif"));
		setLayout(new BorderLayout());
		add(descriptionPanel, BorderLayout.CENTER);
	}
}
