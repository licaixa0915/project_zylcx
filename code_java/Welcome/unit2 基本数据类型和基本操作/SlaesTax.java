public class SlaesTax {
	public static void main(String[] args) {
		double purchaseAmount = 197.59;
		double tax = purchaseAmount * 0.06;
		System.out.println((int) Math.round(tax * 100) / 100.0); // Math.round(float
		// a)四舍五入函数
	}
}
