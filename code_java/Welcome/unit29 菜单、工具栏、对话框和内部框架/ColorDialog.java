import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

@SuppressWarnings("serial")
// 自定义对话框
public class ColorDialog extends JDialog {
	private int redValue, greenValue, blueValue;
	private Color color = null;
	private JSlider jslRed = new JSlider(0, 255); // 滑动条
	private JSlider jslGreen = new JSlider(0, 255);
	private JSlider jslBlue = new JSlider(0, 255);
	private JButton jbtOK = new JButton("Ok");
	private JButton jbtCancel = new JButton("Cancel");
	private JPanel jpSelectedColor = new JPanel();

	public ColorDialog() {
		this(null, true);
	}

	public ColorDialog(Frame parent, Boolean modal) {
		super(parent, modal);
		setTitle("Choose Color");

		JPanel jpButtons = new JPanel();
		jpButtons.add(jbtOK);
		jpButtons.add(jbtCancel);

		JPanel jpLabels = new JPanel();
		jpLabels.setLayout(new GridLayout(3, 0));
		jpLabels.add(new JLabel("Red"));
		jpLabels.add(new JLabel("Green"));
		jpLabels.add(new JLabel("Blue"));

		JPanel jpSliders = new JPanel();
		jpSliders.setLayout(new GridLayout(3, 0));
		jpSliders.add(jslRed);
		jpSliders.add(jslGreen);
		jpSliders.add(jslBlue);

		JPanel jpSelectColor = new JPanel();
		jpSelectColor.setLayout(new BorderLayout());
		jpSelectColor.setBorder(BorderFactory
				.createTitledBorder("Select Color"));
		jpSelectColor.add(jpLabels, BorderLayout.WEST);
		jpSelectColor.add(jpSliders, BorderLayout.CENTER);

		JPanel jpColor = new JPanel();
		jpColor.setLayout(new BorderLayout());
		jpColor.add(jpSelectColor, BorderLayout.SOUTH);
		jpColor.add(jpSelectedColor, BorderLayout.CENTER);

		add(jpButtons, BorderLayout.SOUTH);
		add(jpColor, BorderLayout.CENTER);
		pack();

		jbtOK.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				setVisible(true);
			}
		});

		jbtCancel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				color = null;
				setVisible(false);
			}
		});

		jslRed.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				redValue = jslRed.getValue();
				color = new Color(redValue, greenValue, blueValue);
				jpSelectedColor.setBackground(color);
			}
		});

		jslGreen.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				greenValue = jslGreen.getValue();
				color = new Color(redValue, greenValue, blueValue);
				jpSelectedColor.setBackground(color);
			}
		});

		jslBlue.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				blueValue = jslBlue.getValue();
				color = new Color(redValue, greenValue, blueValue);
				jpSelectedColor.setBackground(color);
			}
		});
	}

	public Dimension getPreferredSize() {
		return new Dimension(200, 200);
	}

	public Color getColor() {
		return color;
	}
}
