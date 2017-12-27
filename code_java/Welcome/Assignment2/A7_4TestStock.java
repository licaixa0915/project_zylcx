public class A7_4TestStock {
	public static void main(String[] args) {
		Stock stock;
		stock = new Stock("SUNW", "Sun Microsystem Inc.");
		stock.setPreviousClosingPrice(100);
		stock.setCurrentPrice(90);
		System.out.println(stock.toString());
	}
}

// π…∆±¿‡
class Stock {
	private double currentPrice;
	private String name;
	private double previousClosingPrice;
	private String symbol;

	public Stock() {
	}

	public Stock(String symbol, String name) {
		this.symbol = symbol;
		this.name = name;
	}

	public double changePercent() {
		return (currentPrice - previousClosingPrice) / previousClosingPrice
				* 100.0;
	}

	public double getCurrentPrice() {
		return currentPrice;
	}

	public String getName() {
		return name;
	}

	public double getPreviousClosingPrice() {
		return previousClosingPrice;
	}

	public String getSymbol() {
		return symbol;
	}

	public void setCurrentPrice(double currentPrice) {
		this.currentPrice = currentPrice;
	}

	public void setPreviousClosingPrice(double previousClosingPrice) {
		this.previousClosingPrice = previousClosingPrice;
	}

	public String toString() {
		return "The current price of stock " + getSymbol() + " is "
				+ getCurrentPrice() + "\n The changePercent is "
				+ changePercent() + "%";
	}
}