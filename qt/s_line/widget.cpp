#include "widget.h"
#include "ui_widget.h"
#include "math.h"
#include <QMessageBox>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit_2->setReadOnly(true);
}

Widget::~Widget()
{
    delete data;
    delete ui;
}

void Widget::on_pushButton_clicked()
{


    bool ok;
    motor_start_arr = ui->lineEdit->text().toInt(&ok,10);
    if(!ok)
    {
        QMessageBox::warning(this,"warning","MOTOR_START_SPEED err!");
        return;
    }
    motor_max_arr = ui->lineEdit_2->text().toInt(&ok,10);
    if(!ok){

        QMessageBox::warning(this,"warning","MOTOR_MAX_SPEED err!");
        return ;
    }
    else if(motor_start_arr<motor_max_arr)
    {
        QMessageBox::warning(this,"warning"," 不满足 MOTOR_START_ARR >= MOTOR_MAX_ARR ?");
    }

    motor_add_del_arr = ui->lineEdit_3->text().toInt(&ok,10);
    if(!ok) {
        QMessageBox::warning(this,"warning","MOTOR_ADD_DEL_STEP err!");
        return ;
    }

    if(motor_add_del_arr>(motor_start_arr-motor_max_arr))
    {
         QMessageBox::warning(this,"warning","不满足 MOTOR_ADD_DEL_STEP <= (MOTOR_START_SPEED-MOTOR_MAX_SPEED) ?");
        return;
    }

    x = ui->lineEdit_4->text().toInt(&ok,10);
    if(!ok) {
        QMessageBox::warning(this,"warn","x err!");
        return;
    }
    if(x>motor_add_del_arr/2)x=1;
    delete data;
    uint32_t *data = new uint32_t[motor_add_del_arr/x+1];

    double temp=(motor_start_arr-motor_max_arr);
    double tt;
    QString str;
    int j=0;
    for(uint32_t i=0;i<motor_add_del_arr;)
    {
        tt = exp(-(10*i/(double)motor_add_del_arr-5));
        //data[j] = motor_start_arr - temp/(1+tt);
        data[j] = temp/(1+tt);
        str += QString("0x%1,").arg(data[j],4,16,QLatin1Char('0'));

        j++;
        i += x;
        if((j&0x07)==0) str += "\r\n";
    }

    QString aa = QString("#define MOTOR_ADD_DEL_STEP %1//多少步完成加减速过程  调试参数").arg(motor_add_del_arr);
    QString bb = QString("#define MOTOR_RATE %1//多少个脉冲调速一次").arg(x);

    QString cc = QString("#define MOTOR_START_SPEED %1//静止启动初速度").arg(motor_start_arr);
    QString dd = QString("#define MOTOR_MAX_SPEED %1//最大速度").arg(motor_max_arr);

    ui->plainTextEdit_2->show();
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->appendPlainText("#ifndef __MOTOR_DATA_H");
    ui->plainTextEdit_2->appendPlainText("#define __MOTOR_DATA_H");
    ui->plainTextEdit_2->appendPlainText("#include \"main.h\"\r\n\r\n");

    ui->plainTextEdit_2->appendPlainText(aa);
    ui->plainTextEdit_2->appendPlainText(bb);
    ui->plainTextEdit_2->appendPlainText(cc);
    ui->plainTextEdit_2->appendPlainText(dd);
    ui->plainTextEdit_2->appendPlainText("\r\n\r\nuint16_t motor_data[]={");
    ui->plainTextEdit_2->appendPlainText(str);
    ui->plainTextEdit_2->appendPlainText("};\r\n#endif\r\n");

}



void Widget::on_pushButton_2_clicked()
{
   // ->copy();

    QClipboard *cb = QApplication::clipboard();
//    QString text;

//    // 从剪贴板中复制文本（粘贴）
//    text = cb->text();
//    if ( text )
//        qDebug( "The clipboard contains: %s", text );

    // 把文本复制到剪贴板
    cb->setText(ui->plainTextEdit_2->toPlainText());
}
