import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;

@SuppressWarnings("serial")
public class MyImageCellRenderer extends DefaultTableCellRenderer {
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean isFocused, int row, int column) {
		Image image = ((ImageIcon) value).getImage();
		ImageViewer imageViewer = new ImageViewer(image);
		return imageViewer;
	}
}
