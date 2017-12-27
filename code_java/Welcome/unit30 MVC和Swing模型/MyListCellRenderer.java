import java.awt.*;

import javax.swing.*;
import javax.swing.border.*;

public class MyListCellRenderer implements ListCellRenderer<Object> {
	private JLabel jlblCell = new JLabel(" ", JLabel.LEFT);
	private Border lineBorder = BorderFactory.createLineBorder(Color.black, 1);
	private Border emptyBorder = BorderFactory.createEmptyBorder(2, 2, 2, 2);

	public Component getListCellRendererComponent(JList<?> list, Object value,
			int index, boolean isSelected, boolean cellHasFocus) {
		Object[] pair = (Object[]) value;
		jlblCell.setOpaque(true); // 如果为 true，则该组件绘制其边界内的所有像素。
		jlblCell.setIcon((ImageIcon) pair[0]);
		jlblCell.setText(pair[1].toString());
		if (isSelected) {
			jlblCell.setForeground(list.getSelectionForeground());
			jlblCell.setBackground(list.getSelectionBackground());
		} else {
			jlblCell.setForeground(list.getForeground());
			jlblCell.setBackground(list.getBackground());
		}
		jlblCell.setBorder(cellHasFocus ? lineBorder : emptyBorder);
		return jlblCell;
	}

}
