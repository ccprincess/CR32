#include "process_dlg.h"
#include "ui_process_dlg.h"

process_dlg::process_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::process_dlg)
{
    ui->setupUi(this);
    module_row = 0;
    thread_row = 0;
    // set header
    QStringList tw_header;
    QStringList th_header;
    tw_header<<"模块id"<< "进程id" << "基地址大小";
    th_header << QString("线程id")<<QString("优先") <<QString("属主id");
    ui->tw_module->setHorizontalHeaderLabels(tw_header);
    ui->tw_thread->setHorizontalHeaderLabels(th_header);
}

process_dlg::~process_dlg()
{
    delete ui;
}

bool process_dlg::add_item(ITEM_TYPE type, QString col1, QString col2, QString col3)
{
    switch(type)
    {
        case MODULE_ITEM:
        {
            ui->tw_module->insertRow(module_row);
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(col1));
            ui->tw_module->setItem(module_row, 0, newItem);

            QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(col2));
            ui->tw_module->setItem(module_row, 1, newItem2);

            QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(col3));
            ui->tw_module->setItem(module_row, 2, newItem3);

            module_row++;
            qDebug() << module_row << endl;

            //change color
            for (int i = 0;i <  ui->tw_module->rowCount();i++)
            {
                if (i % 2 == 0)
                 {
                     for (int j = 0;j <  ui->tw_module->columnCount();j++)
                     {
                          QTableWidgetItem *item =  ui->tw_module->item(i,j);
                         if (item)
                         {
                              const QColor color = QColor(215,255,225);
                              item->setBackgroundColor(color);
                         }

                     }
                 }
            }

            //不可编辑
            ui->tw_module->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tw_module->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
            ui->tw_module->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行

        }
            break;
        case THREAD_ITEM:
        {
            ui->tw_thread->insertRow(thread_row);
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(col1));
            ui->tw_thread->setItem(thread_row, 0, newItem);

            QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(col2));
            ui->tw_thread->setItem(thread_row, 1, newItem2);

            QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(col3));
            ui->tw_thread->setItem(thread_row, 2, newItem3);

            thread_row++;
            qDebug() << thread_row << endl;

            //change color
            for (int i = 0;i < ui->tw_thread->rowCount();i++)
            {
                if (i % 2 == 0)
                 {
                     for (int j = 0;j < ui->tw_thread->columnCount();j++)
                     {
                          QTableWidgetItem *item = ui->tw_thread->item(i,j);
                         if (item)
                         {
                              const QColor color = QColor(215,255,225);
                              item->setBackgroundColor(color);
                         }

                     }
                 }
            }

            //不可编辑
            ui->tw_thread->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tw_thread->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
            ui->tw_thread->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行

        }
            break;
        default:

            break;
    }
    return true;
}
