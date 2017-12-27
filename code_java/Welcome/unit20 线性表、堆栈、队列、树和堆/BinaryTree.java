public class BinaryTree {
	private static class TreeNode {
		Object element;
		TreeNode left;
		TreeNode right;

		public TreeNode(Object o) {
			element = o;
		}
	}
	private TreeNode root;

	private int size = 0;

	public BinaryTree() {
	}

	public BinaryTree(Object[] objects) {
		for (int i = 0; i < objects.length; i++)
			insert(objects[i]);
	}

	public int getSize() {
		return size;
	}

	public void inorder() {
		inorder(root);
	}

	private void inorder(TreeNode root) {
		if (root == null)
			return;
		inorder(root.left);
		System.out.print(root.element + " ");
		inorder(root.right);
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public boolean insert(Object o) {
		if (root == null)
			root = new TreeNode(o);
		else {
			TreeNode parent = null;
			TreeNode current = root;
			while (current != null) {
				if (((Comparable) o).compareTo(current.element) < 0) {
					parent = current;
					current = current.left;
				} else if (((Comparable) o).compareTo(current.element) > 0) {
					parent = current;
					current = current.right;
				} else
					return false;
			}
			if (((Comparable) o).compareTo(parent.element) < 0)
				parent.left = new TreeNode(o);
			else
				parent.right = new TreeNode(o);
		}
		size++;
		return true;
	}

	public void postorder() {
		postorder(root);
	}

	private void postorder(TreeNode root) {
		if (root == null)
			return;
		postorder(root.left);
		postorder(root.right);
		System.out.print(root.element + " ");
	}

	public void preorder() {
		preorder(root);
	}

	private void preorder(TreeNode root) {
		if (root == null)
			return;
		System.out.print(root.element + " ");
		preorder(root.left);
		preorder(root.right);
	}
}
