import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

@SuppressWarnings("serial")
// �ڲ����
public class ShowInternalFrame extends JApplet {
	private ImageIcon USIcon = new ImageIcon(getClass().getResource(
			"image/usIcon.gif"));
	private ImageIcon CanadaIcon = new ImageIcon(getClass().getResource(
			"image/caIcon.gif"));
	private JMenuBar jMenuBar1 = new JMenuBar();
	private JMenuItem jmiUS = new JMenuItem("US");
	private JMenuItem jmiCanada = new JMenuItem("Canada");
	private JLabel jlblImage = new JLabel(USIcon, JLabel.CENTER);

	// ���ڴ������ĵ����������������������û��ɴ���
	// JInternalFrame���󲢽�����ӵ� JDesktopPane
	// ͨ������ JInternalFrames �ĸ���
	private JDesktopPane desktop = new JDesktopPane();
	private JInternalFrame internalFrame = new JInternalFrame("US", true, true,
			true, true); // ��������ָ�����⡢�ɵ������ɹرա�����󻯺Ϳ�ͼ�껯�� JInternalFrame

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
