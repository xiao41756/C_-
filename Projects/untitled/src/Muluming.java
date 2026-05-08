import java.io.File;
public class Muluming {
    public static void main(String[] args) {
        // 指定目录路径
        String directoryPath = "D:\\IdeaProjects\\Projects\\untitled";

        // 创建File对象
        File directory = new File(directoryPath);

        // 检查路径是否存在且是一个目录
        if (directory.exists() && directory.isDirectory()) {
            // 获取目录中的所有文件和子目录
            File[] contents = directory.listFiles();

            if (contents != null) {
                System.out.println("子目录:");
                for (File content : contents) {
                    if (content.isDirectory()) {
                        System.out.println(content.getName());
                    }
                }

                System.out.println("子文件:");
                for (File content : contents) {
                    if (content.isFile()) {
                        System.out.println(content.getName());
                    }
                }
            } else {
                System.out.println("目录为空或无法读取。");
            }
        } else {
            System.out.println("指定的路径不是一个有效的目录。");
        }
    }
}

