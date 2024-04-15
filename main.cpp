// 单播Mac地址生成器
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QClipboard>
#include <QMouseEvent>
#include <QString>
#include <QRandomGenerator>
#include <QTimer>

class MACGenerator : public QWidget
{
    Q_OBJECT

public:
    MACGenerator(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("MAC地址生成");
        setWindowIcon(QIcon(":/new/prefix1/ICO"));

        QVBoxLayout *layout = new QVBoxLayout(this);

        macDisplay = new QLineEdit(this);
        macDisplay->setReadOnly(true);
        macDisplay->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
        QFont font("Arial", 16);                   // 设置字体为Arial，大小为12像素
        macDisplay->setFont(font);                 // 应用设置的字体
        layout->addWidget(macDisplay);

        generateButton = new QPushButton("生成MAC地址", this);
        QFont buttonFont("Microsoft YaHei UI", 12); // 设置字体为Arial
        generateButton->setFont(buttonFont);        // 应用设置的字体
        connect(generateButton, &QPushButton::clicked, this, &MACGenerator::generateMAC);
        layout->addWidget(generateButton);
        setLayout(layout);

        setFixedSize(250, 100); // 设置窗口大小

        generateMAC(); // 生成初始的MAC地址

        macDisplay->installEventFilter(this); // 安装事件过滤器
    }

private slots:
    void generateMAC()
    // 生成单播MAC地址
    {
        QString macAddress;
        // 生成第一个字节，单播地址最低位为0
        uint firstByte = QRandomGenerator::global()->bounded(0x00, 0x7F) & 0xFE;
        macAddress += QString("%1").arg(firstByte, 2, 16, QLatin1Char('0')).toUpper();
        for (int i = 1; i < 6; ++i)
        {
            macAddress += ":";
            macAddress += QString("%1").arg(QRandomGenerator::global()->generate() % 256, 2, 16, QLatin1Char('0')).toUpper();
        }
        macDisplay->setText(macAddress);
    }

    bool eventFilter(QObject *obj, QEvent *event)
    // 鼠标点击文本框触发事件
    {
        if (obj == macDisplay && event->type() == QEvent::MouseButtonPress)
        {
            copyMACToClipboard();
            flashLineEditBorder();
            return true; // 事件被处理
        }
        return false; // 事件未被处理，继续传递
    }

    void copyMACToClipboard()
    // 将文本框中MAC地址复制到剪贴板
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(macDisplay->text());
    }

    void flashLineEditBorder()
    // 鼠标点击文本框时，文本框边框样式
    {
        macDisplay->setStyleSheet("border: 2px solid blue;");

        // 创建计时器
        QTimer *timer = new QTimer(this);
        timer->setInterval(200); // 闪烁间隔
        connect(timer, &QTimer::timeout, this, &MACGenerator::resetLineEditBorder);
        timer->start();
    }

    void resetLineEditBorder()
    // 重置文本框边框样式
    {
        macDisplay->setStyleSheet("");
        QObject::sender()->deleteLater(); // 删除计时器对象
    }

private:
    QLineEdit *macDisplay;
    QPushButton *generateButton;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MACGenerator generator;
    generator.show();
    return app.exec();
}

#include "main.moc"
