import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
// 内部框架
public class ShowInternalFrame extends JApplet {
	private ImageIcon USIcon = new ImageIcon(getClass().getResource(
			"image/usIcon.gif"));
	private ImageIcon CanadaIcon = new ImageIcon(getClass().getResource(
			"image/caIcon.gif"));
	private JMenuBar jMenuBar1 = new JMenuBar();
	private JMenuItem jmiUS = new JMenuItem("US");
	private JMenuItem jmiCanada = new JMenuItem("Canada");
	private JLabel jlblImage = new JLabel(USIcon, JLabel.CENTER);

	// 用于创建多文档界面或虚拟桌面的容器。用户可创建
	// JInternalFrame对象并将其添加到 JDesktopPane
	// 通常用作 JInternalFrames 的父类
	private JDesktopPane desktop = new JDesktopPane();
	private JInternalFrame internalFrame = new JInternalFrame("US", true, true,
			true, true); // 创建具有指定标题、可调整、可关闭、可最大化和可图标化的 JInternalFrame

	public ShowInternalFrame() {
		desktop.add(internalFrame);
		this.setSize(new Dimension(400, 300));
		this.getContentPane().add(desktop, BorderLayout.CENTER);

		jlblImage.setIcon(USIcon);
		internalFrame.setFrameIcon(USIcon);
		internalFrame.add(jlblImage);
		internalFrame.setLocation(20, 20);
		internalFrame.setSize(100, 100);
		internalFrame.setVisible(true);
		
		JMenu jMenu1 = new JMenu("Flags");
		jMenuBar1.add(jMenu1);
		jMenu1.add(jmiUS);
		jMenu1.add(jmiCanada);

		this.setJMenuBar(jMenuBar1);
		
		jmiUS.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblImage.setIcon(USIcon);
				internalFrame.setFrameIcon(USIcon);
				internalFrame.setTitle("US");
			}
		});

		jmiCanada.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				jlblImage.setIcon(CanadaIcon);
				internalFrame.setFrameIcon(CanadaIcon);
				internalFrame.setTitle("Canada");
			}
		});
	}
}
