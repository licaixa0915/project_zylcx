import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;
import java.util.*;

@SuppressWarnings("serial")
public class TreeNodeDemo extends JApplet {
	@SuppressWarnings({ "rawtypes" })
	public TreeNodeDemo() {
		DefaultMutableTreeNode root, europe, northAmerica, us;

		europe = new DefaultMutableTreeNode("Europe");
		europe.add(new DefaultMutableTreeNode("UK"));
		europe.add(new DefaultMutableTreeNode("Germany"));
		europe.add(new DefaultMutableTreeNode("France"));
		europe.add(new DefaultMutableTreeNode("Norway"));

		us = new DefaultMutableTreeNode("US");
		us.add(new DefaultMutableTreeNode("California"));
		us.add(new DefaultMutableTreeNode("Texas"));
		us.add(new DefaultMutableTreeNode("New York"));
		us.add(new DefaultMutableTreeNode("Florida"));
		us.add(new DefaultMutableTreeNode("Illinois"));

		northAmerica = new DefaultMutableTreeNode("North America");
		northAmerica.add(us);
		northAmerica.add(new DefaultMutableTreeNode("Canada"));

		root = new DefaultMutableTreeNode("World");
		root.add(europe);
		root.add(northAmerica);

		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout(1, 2));
		panel.add(new JScrollPane(new JTree(root)));
		panel.add(new JScrollPane(new JTree(new DefaultTreeModel(root))));

		JTextArea jtaMessage = new JTextArea();
		jtaMessage.setWrapStyleWord(true); // true:在单词边界（空白）处换行;false:在字符边界处换行
		jtaMessage.setLineWrap(true); // true:换行;false:不换行
		add(new JSplitPane(JSplitPane.VERTICAL_SPLIT, panel, new JScrollPane(
				jtaMessage)), BorderLayout.CENTER);

		jtaMessage.append("Depth of the node US is " + us.getDepth());
		jtaMessage.append("\nLevel of the node US is " + us.getLevel());
		jtaMessage.append("\nFirst child of the root is "
				+ root.getFirstChild());
		jtaMessage.append("\nFirst leaf of the root is " + root.getFirstLeaf());
		jtaMessage.append("\nNumber of the children of the root is "
				+ root.getChildCount());
		jtaMessage.append("\nNumber of leaves in the tree is "
				+ root.getLeafCount());

		String breadthFirstSearchResult = "";
		Enumeration bf = root.breadthFirstEnumeration();
		while (bf.hasMoreElements())
			breadthFirstSearchResult += bf.nextElement().toString() + " ";
		jtaMessage.append("\nBreath-first traversal from the root is "
				+ breadthFirstSearchResult);
	}
}
