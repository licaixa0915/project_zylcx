public class TestFileClass {
	public static void main(String[] args) {
		java.io.File file = new java.io.File("image/us.gif"); // 需要手动在改目录下建立文件
		System.out.println("Does it exist? " + file.exists());
		System.out.println("Can it bu read? " + file.canRead());
		System.out.println("Can it be written? " + file.canWrite());
		System.out.println("Is it a directory? " + file.isDirectory());
		System.out.println("Is it a file? " + file.isFile());
		System.out.println("Is it absolute? " + file.isAbsolute());
		System.out.println("Is it hidden? " + file.isHidden());
		System.out.println("Absolute path is " + file.getAbsolutePath()); // 存在于Welcome文件夹下
		System.out.println("Last modified on "
				+ new java.util.Date(file.lastModified()));
	}
}
