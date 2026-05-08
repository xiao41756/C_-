import java.io.File;
import java.util.ArrayList;
import java.util.List;
public class Javatet {
    public static void main(String[] args) {
        // 指定目录路径
        String directoryPath = "D:\\IdeaProjects\\Projects\\untitled";
        // 获取目录对象
        File directory = new File(directoryPath);
        // 检查目录是否存在
        if (!directory.exists() || !directory.isDirectory()) {
            System.out.println("指定的路径不是一个有效的目录。");
            return;
        }
        // 获取所有.java文件
        List<File> javaFiles = findJavaFiles(directory);
        // 打印文件列表
        for (File file : javaFiles) {
            System.out.println(file.getAbsolutePath());
        }
    }
    private static List<File> findJavaFiles(File directory) {
        List<File> javaFiles = new ArrayList<>();
        // 获取目录中的所有文件和子目录
        File[] files = directory.listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.isFile() && file.getName().endsWith(".java")) {
                    // 如果是.java文件，添加到列表中
                    javaFiles.add(file);
                } else if (file.isDirectory()) {
                    // 如果是目录，递归查找
                    javaFiles.addAll(findJavaFiles(file));
                }
            }
        }
        return javaFiles;
    }
}
