public class A6_16ComputeExecutionTime {
	public static int binarySearch(int[] list, int key) {
		int low = 0;
		int high = list.length - 1;
		while (high >= low) {
			int mid = (high + low) / 2;
			if (key < list[mid])
				high = mid - 1;
			else if (key == list[mid])
				return mid;
			else
				low = mid + 1;
		}
		return -low - 1;
	}

	public static int linearSearch(int[] list, int key) {
		for (int i = 0; i < list.length; i++)
			if (key == list[i])
				return i;
		return -1;
	}

	public static void main(String[] args) {
		// 初始化
		final int TOTAL_NUMBER = 100000;
		int[] numbers = new int[TOTAL_NUMBER];
		int key = (int) (Math.random() * TOTAL_NUMBER);
		for (int i = 0; i < numbers.length; i++)
			numbers[i] = (int) (Math.random() * (65535 + 1));
		// linearSearch线性查找执行时间
		long startTime = System.currentTimeMillis();
		int k1 = linearSearch(numbers, numbers[key]);
		System.out.println("The key is " + key + " and the k1 is " + k1);
		System.out.println("The numbers[key] is " + numbers[key]
				+ " and the numbers[k1] is " + numbers[k1]);
		long endTime = System.currentTimeMillis();
		long executionTime1 = endTime - startTime;
		System.out.println("The time of linearSearch method is "
				+ executionTime1);
		// 数组排序
		java.util.Arrays.sort(numbers);
		// bianrySearch二分查找执行时间
		startTime = System.currentTimeMillis();
		int k2 = binarySearch(numbers, numbers[key]);
		System.out.println("The key is " + key + " and the k2 is " + k2);
		System.out.println("The numbers[key] is " + numbers[key]
				+ " and the numbers[k2] is " + numbers[k2]);
		endTime = System.currentTimeMillis();
		long executionTime2 = endTime - startTime;
		System.out.println("The time of bianrySearch method is "
				+ executionTime2);

		if (executionTime1 < executionTime2)
			System.out.println("the linearSearch method is faster!");
		else if (executionTime1 > executionTime2)
			System.out.println("the bianrySearch method is faster!");
		else
			System.out.println("they are euqals!");
	}
}
