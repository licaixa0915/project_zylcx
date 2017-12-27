import java.io.*;

public class Copy {
	/**
	 * Main method
	 * 
	 * @param args
	 *            [0] for source file
	 * @param args
	 *            [1] for target file
	 */
	public static void main(String[] args) throws IOException {
		if (args.length != 2) {
			System.out.println("Usage: Java CopyFile sourceFile targetFile");
			System.exit(0);
		}
		File sourceFile = new File(args[0]);
		if (!sourceFile.exists()) {
			System.out.println("Source file " + args[0] + " not exist");
			System.exit(0);
		}
		File targetFile = new File(args[1]);
		if (targetFile.exists()) {
			System.out.println("Target file " + args[1] + " already exists");
			System.exit(0);
		}
		BufferedInputStream input = new BufferedInputStream(
				new FileInputStream(sourceFile));
		BufferedOutputStream output = new BufferedOutputStream(
				new FileOutputStream(targetFile));
		System.out.println("The file " + args[0] + " has " + input.available()
				+ " bytes");
		int r;
		while ((r = input.read()) != -1)
			output.write((byte) r);
		input.close();
		output.close();
		System.out.println("Copy done!");
	}
}
