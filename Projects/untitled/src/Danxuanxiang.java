import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Danxuanxiang {
    public static void main(String[] args) {
        // 创建主窗体
        JFrame frame = new JFrame("关于单选按钮");
        frame.setSize(300, 250);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new GridLayout(5, 1));

        // 添加标题
        JLabel titleLabel = new JLabel("请选择您的志向:");
        titleLabel.setHorizontalAlignment(SwingConstants.CENTER);
        frame.add(titleLabel);

        // 创建单选按钮组
        ButtonGroup group = new ButtonGroup();
        JRadioButton managerRadio = new JRadioButton("经理");
        JRadioButton engineerRadio = new JRadioButton("工程师");
        JRadioButton teacherRadio = new JRadioButton("教师");

        // 将单选按钮添加到组中
        group.add(managerRadio);
        group.add(engineerRadio);
        group.add(teacherRadio);

        // 添加单选按钮到窗体
        frame.add(managerRadio);
        frame.add(engineerRadio);
        frame.add(teacherRadio);

        // 添加结果显示标签
        JLabel resultLabel = new JLabel("当前选择: 无");
        resultLabel.setHorizontalAlignment(SwingConstants.CENTER);
        frame.add(resultLabel);

        // 单选按钮事件监听
        ItemListener listener = new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (managerRadio.isSelected()) {
                    resultLabel.setText("当前选择: 经理");
                } else if (engineerRadio.isSelected()) {
                    resultLabel.setText("当前选择: 工程师");
                } else if (teacherRadio.isSelected()) {
                    resultLabel.setText("当前选择: 教师");
                }
            }
        };

        managerRadio.addItemListener(listener);
        engineerRadio.addItemListener(listener);
        teacherRadio.addItemListener(listener);

        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
