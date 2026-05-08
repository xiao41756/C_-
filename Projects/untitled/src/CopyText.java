import java.io.*;

class Copy {
    String  sourceName, destName;
    BufferedReader source;
    BufferedWriter dest;
    String line;
    //打开源文件和目的文件，如无异常则返回true
    private boolean openFile() {
        try {
            source = new BufferedReader(new FileReader(sourceName));
        }catch (IOException e) {
            System.out.println("无法打开源文件：" + sourceName);
            return false;
        }
        try {
            dest = new BufferedWriter(new FileWriter(destName));
        }catch (IOException e) {
            System.out.println("无法打开目的文件：" + destName);
            return false;
        }
        return true;
    }
    //复制文件
    private boolean copyFile() {
        try {
            while ((line = source.readLine()) != null) { // 逐行读取源文件内容
                dest.write(line);
                dest.newLine(); // 在此行数据末尾换行
                // 移除多余的读取操作
            }
        } catch (IOException e) {
            System.out.println("复制文件时出错：" + e.getMessage());
            return false;
        }
        return true;
    }
    //关闭文件
    private boolean closeFile(){
        boolean result = true;
        try {
            source.close();
        }catch (IOException e) {
            System.out.println("关闭源文件时出错：" + e.getMessage());
            result = false;
        }
        try {
            dest.close();
        }catch (IOException e) {
            System.out.println("关闭目的文件时出错：" + e.getMessage());
            result = false;
        }
        return result;
    }
    //调用三个私有方法
    public boolean copy(String sourceName, String destName) {
        this.sourceName = sourceName;
        this.destName = destName;
        return openFile() && copyFile() && closeFile();
    }
}

public class CopyText {
    public static void main(String[] args) {
        if (args.length == 2) {
            boolean success = new Copy().copy(args[0], args[1]);
            if (success) {
                System.out.println("文件复制成功");
            } else {
                System.out.println("文件复制失败");
            }
        } else {
            System.out.println("请提供源文件名和目标文件名作为命令行参数");
        }
    }


}
