import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.border.LineBorder;

@SuppressWarnings("serial")
// 卡片布局管理器
public class ShowCardLayout extends JApplet {
	private CardLayout cardLayout = new CardLayout(20, 10);
	private JPanel cardPanel = new JPanel(cardLayout);
	private JButton jbtFirst, jbtNext, jbtPrevious, jbtLast;
	private JComboBox<String> jcboImage;
	private final int NUM_OF_FLAGS = 6;

	public ShowCardLayout() {
		cardPanel.setBorder(new LineBorder(Color.red));

		// Add 9 labels for displaying images into cardPanel
		for (int i = 1; i <= NUM_OF_FLAGS; i++) {
			JLabel label = new JLabel(new ImageIcon("image/flag" + i + ".gif"));
			cardPanel.add(label, String.valueOf(i));
		}

		JPanel p = new JPanel();
		p.add(jbtFirst = new JButton("First"));
		p.add(jbtNext = new JButton("Next"));
		p.add(jbtPrevious = new JButton("Previous"));
		p.add(jbtLast = new JButton("Last"));
		p.add(new JLabel("Image"));
		p.add(jcboImage = new JComboBox<String>());

		for (int i = 1; i <= NUM_OF_FLAGS; i++)
			jcboImage.addItem(String.valueOf(i));

		add(cardPanel, BorderLayout.CENTER);
		add(p, BorderLayout.SOUTH);

		jbtFirst.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				cardLayout.first(cardPanel);
			}
		});
		
		jbtNext.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				cardLayout.next(cardPanel);
			}
		});

		jbtPrevious.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				cardLayout.previous(cardPanel);
			}
		});
		
		jbtLast.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				cardLayout.last(cardPanel);
			}
		});
		
		jcboImage.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				cardLayout.show(cardPanel, (String)e.getItem());
			}
		});
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame("ShowCardLayout");
		ShowCardLayout applet = new ShowCardLayout();
		frame.add(applet);
		
		applet.init();
		applet.start();
		
		frame.pack();
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);
	}
}
