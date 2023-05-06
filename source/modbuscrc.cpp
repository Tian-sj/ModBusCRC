#include "modbuscrc.h"
#include "./ui_modbuscrc.h"

#include <QMessageBox>

ModBusCRC::ModBusCRC(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModBusCRC)
{
    ui->setupUi(this);

    init_table();
}

ModBusCRC::~ModBusCRC()
{
    delete ui;
}

void ModBusCRC::init_table()
{
    the_model = new QStandardItemModel(4, 2);
    ui->tableView->setModel(the_model);

    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList list;
    list << "字节数(10进制)"
         << "字节数(16进制)"
         << "CRC-16(MSB-LSB)"
         << "CRC-16(Modbus)";

    QFont font;
    font.setBold(true);

    for (int i = 0; i < 4; ++i) {
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setFont(font);
        item->setForeground(QColor(111, 152, 201));
        item->setText(list.at(i));
        the_model->setItem(i, 0, item);
    }
}

quint16 ModBusCRC::ModbusCRC16(QByteArray senddata)
{
    int len = senddata.size();
    quint16 wcrc = 0xFFFF;//预置16位crc寄存器，初值全部为1
    quint8 temp;//定义中间变量
    int i = 0, j = 0;//定义计数
    for(int i = 0; i < len; ++i)//循环计算每个数据
    {
        temp = senddata.at(i);
        wcrc ^= temp;
        for(int j = 0; j < 8; ++j){
            //判断右移出的是不是1，如果是1则与多项式进行异或。
            if(wcrc & 0x0001){
                wcrc >>= 1;//先将数据右移一位
                wcrc ^= 0xA001;//与上面的多项式进行异或
            }
            else//如果不是1，则直接移出
                wcrc >>= 1;//直接移出
        }
    }
    return wcrc;
}

void ModBusCRC::on_btn_calc_clicked()
{
    QString str = ui->text->toPlainText().replace(" ", "");
    QByteArray ba = QByteArray::fromHex(str.toUtf8());
    if (str.size() & 1) {
        QMessageBox::information(this, "", "请输入正确的十六进制数组，两个字符表示一个字节!");
        return;
    }

    quint16 crc = ModbusCRC16(ba);
    quint16 modbus_crc = (crc << 8) | (crc >> 8);

    QStringList list;
    list << QString("%1").arg(ba.size())
         << QString("%1").arg(ba.size(), 2, 16, QLatin1Char('0'))
         << QString("%1").arg(crc, 4, 16, QLatin1Char('0'))
         << QString("%1").arg(modbus_crc, 4, 16, QLatin1Char('0'));

    for (int i = 0; i < 4; ++i) {
        QStandardItem* item = new QStandardItem;
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setText(list.at(i).toUpper());
        the_model->setItem(i, 1, item);
    }
}


void ModBusCRC::on_btn_clear_clicked()
{
    ui->text->clear();
}
