import javax.swing.JTree;
import javax.swing.tree.TreeModel;

// 使用树模型遍历树中的所有结点
public class TestTreeModel {
	public static void main(String[] args) {
		JTree jTree1 = new JTree();
		TreeModel model = jTree1.getModel();
		 traversal(model, model.getRoot());
//		Object root = model.getChild(model.getRoot(), 2);
//		System.out.println(model.getChildCount(root));
	}

	private static void traversal(TreeModel model, Object root) {
		System.out.print(root + " ");
		if (model.isLeaf(root))
			return;
		for (int i = 0; i < model.getChildCount(root); i++) {
			traversal(model, model.getChild(root, i));
		}
	}
}
