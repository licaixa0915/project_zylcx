// compute sum of (1/2 + 2/3 + 3/4 + ... + 98/99 + 99/100)
// ��������������,ʹ��ϵͳ���������޷���������������,���Ը���ʵ��������

public class A11_1UseRationalClass {
	public static void main(String[] args) {
		Rational r = new Rational();
		double sum = 0;
		for (int i = 1; i <= 99; i++) {
			r.setNumerator(i);
			r.setDenominator(i + 1);
			sum += (r.doubleValue());
		}
		System.out.println("The sum is " + sum);

		// Rational sum2 = new Rational();
		// for (int i = 1; i <= 99; i++) {
		// r.setNumerator(i);
		// r.setDenominator(i + 1);
		// sum2 = sum2.add(r);
		// System.out.println("The sum is " + sum2);
		// }
		// System.out.println("The sum is " + sum2);
	}
}
