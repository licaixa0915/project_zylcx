public class A6_24TwoMatrixAddition {
	// 两个数组相加
	public static int[][] addMatrix(int[][] a, int[][] b) {
		int[][] result = new int[a.length][a[0].length];
		for (int i = 0; i < result.length; i++)
			for (int j = 0; j < result[0].length; j++)
				result[i][j] = a[i][j] + b[i][j];
		return result;
	}

	public static void main(String[] args) {
		// 初始化
		int[][] matrix1 = new int[5][5];
		int[][] matrix2 = new int[5][5];
		for (int i = 0; i < matrix1.length; i++)
			for (int j = 0; j < matrix1[i].length; j++) {
				matrix1[i][j] = (int) (Math.random() * 10);
				matrix2[i][j] = (int) (Math.random() * 10);
			}
		int[][] resultmatrix = addMatrix(matrix1, matrix2);
		System.out.println("The result is: ");
		printResult(matrix1, matrix2, resultmatrix);
	}

	// 打印
	public static void printResult(int[][] a, int[][] b, int[][] result) {
		for (int i = 0; i < a.length; i++) {
			for (int j = 0; j < a[0].length; j++)
				System.out.print(" " + a[i][j]);
			if (i == a.length / 2)
				System.out.print("   +   ");
			else
				System.out.print("       ");
			for (int j = 0; j < b[0].length; j++)
				System.out.print(" " + b[i][j]);
			if (i == a.length / 2)
				System.out.print("   =   ");
			else
				System.out.print("       ");
			for (int j = 0; j < result[0].length; j++)
				if (result[i][j] < 10)
					System.out.print("  " + result[i][j]);
				else
					System.out.print(" " + result[i][j]);
			System.out.println();
		}
	}
}
