import java.io.*;

public class Cpan {
    public static void main(String[] args) throws IOException {
        //创建C盘下的Hello.txt文件
        String fileName = "C:\\Hello.txt";
        FileWriter fw = new FileWriter(fileName);
        // 写入字符串到文件
        fw.write("Hello, World!\n");
        fw.write("This is a test.\n");
        // 关闭文件
        fw.close();

    }
}
