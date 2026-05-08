import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class CheckboxDemo {
    public static void main(String[] args) {
        // 创建主窗口
        JFrame frame = new JFrame("关于复选框");
        frame.setSize(400, 200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());

        // 创建顶部标签
        JLabel titleLabel = new JLabel("爱好选择：");
        titleLabel.setFont(new Font("宋体", Font.PLAIN, 16));
        titleLabel.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 0));

        // 创建复选框面板
        JPanel checkboxPanel = new JPanel();
        JCheckBox musicBox = new JCheckBox("音乐");
        JCheckBox sportBox = new JCheckBox("运动");
        JCheckBox webBox = new JCheckBox("上网");

        // 添加复选框到面板
        checkboxPanel.add(musicBox);
        checkboxPanel.add(sportBox);
        checkboxPanel.add(webBox);
        checkboxPanel.setBorder(BorderFactory.createEmptyBorder(10, 20, 10, 0));

        // 创建显示结果的文本框
        JTextField resultField = new JTextField("您选择了：");
        resultField.setEditable(false); // 设置为只读
        resultField.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("选择结果"),
                BorderFactory.createEmptyBorder(5, 10, 5, 10)
        ));

        // 添加事件监听器
        ItemListener listener = e -> {
            StringBuilder selection = new StringBuilder("您选择了：");
            if (musicBox.isSelected()) selection.append("音乐 ");
            if (sportBox.isSelected()) selection.append("运动 ");
            if (webBox.isSelected()) selection.append("上网 ");
            resultField.setText(selection.toString());
        };

        musicBox.addItemListener(listener);
        sportBox.addItemListener(listener);
        webBox.addItemListener(listener);

        // 组装界面
        frame.add(titleLabel, BorderLayout.NORTH);
        frame.add(checkboxPanel, BorderLayout.CENTER);
        frame.add(resultField, BorderLayout.SOUTH);

        // 显示窗口
        frame.setLocationRelativeTo(null); // 居中显示
        frame.setVisible(true);
    }
}