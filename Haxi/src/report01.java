import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class report01 {
    //统计一篇英文文档中各单词出现的次数
    //并按单词出现的频率由高到低进行输出
    public static void main(String[] args) {

        //统计一篇英文文档中各单词出现的次数
        //1.输入文档并分单词存储 -> 字符串数组
        System.out.println("请输入一篇英文文档");
        Scanner input = new Scanner(System.in);
        String abb = input.nextLine();
        String[] tryarr;
        tryarr = abb.split(" ");
        //调用方法遇到空格分隔
        Map<String, Integer> map = new HashMap<String, Integer>();
        //增强for循环，for each element in arrayName do {...}
        //遍历数组
        for(String res:tryarr) {
            if (!map.containsKey(res)) {//自动计算出 res 的 hashCode, 判定其是否存在于 map 中.
                map.put(res,1);//hashCode 用于判断, key 中存储单词,  value 存储出现次数;
            } else {
                int number = map.get(res);
                number++;
                map.put(res, number);
            }
        }

        int i = 0;
        //对象数组
        Arr[] arrs = new Arr[map.size()];
        //对象数组实例化
        for(int exe = 0; exe < map.size(); exe++) {
            arrs[exe] = new Arr();
        }
        //遍历 hashTable, 用对象存储 String 类型的 key 和 int 类型的 value.
        //用数组存储对象
        for (Map.Entry<String, Integer> tra: map.entrySet()) {
            Arr temp1 = new Arr(tra.getKey(), tra.getValue());
            arrs[i] = temp1;
            i++;
        }
        //冒泡排序
        Arr temp;
        for (int j = 0; j < arrs.length-1; j++) {
            for (int k = j+1; k < arrs.length; k++) {
                if(arrs[j].getNumber() < (arrs[k].getNumber())) {
                    temp = arrs[j];
                    arrs[j]= arrs[k];
                    arrs[k] = temp;
                }
            }
        }
        //输出结果
        for (int i1 = 0; i1 < map.size() ; i1++) {
            //调用 show 方法
            arrs[i1].show();
        }



    }
}



class Arr {

    String jian;
    int number;

    public Arr() {

    }

    public Arr(String jian, int number) {
        this.jian = jian;
        this.number = number;
    }

    public String getJian() {
        return jian;
    }

    public void setJian(String jian) {
        this.jian = jian;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    //输出方法
    public void show() {

        System.out.println(jian + "：" + number + "次");


    }
}





