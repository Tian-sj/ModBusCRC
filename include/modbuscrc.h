#ifndef MODBUSCRC_H
#define MODBUSCRC_H

#include <QWidget>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class ModBusCRC; }
QT_END_NAMESPACE

class ModBusCRC : public QWidget
{
    Q_OBJECT

public:
    ModBusCRC(QWidget *parent = nullptr);
    ~ModBusCRC();

private:
    void init_table();
    quint16 ModbusCRC16(QByteArray senddata);

private slots:

    void on_btn_calc_clicked();

    void on_btn_clear_clicked();

private:
    Ui::ModBusCRC *ui;

    QStandardItemModel *the_model;
};
#endif // MODBUSCRC_H
