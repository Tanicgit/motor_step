#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QClipboard>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;

     uint32_t *data;
     uint32_t motor_start_arr;//MOTOR_START_SPEED
     uint32_t motor_max_arr;//MOTOR_MAX_SPEED
     uint32_t motor_add_del_arr;//MOTOR_ADD_DEL_STEP
     uint32_t x;
     uint8_t show_tp=0;
};

#endif // WIDGET_H
