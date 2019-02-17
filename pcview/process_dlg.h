#ifndef PROCESS_DLG_H
#define PROCESS_DLG_H

#include <QDialog>
#include <qdebug.h>

enum ITEM_TYPE{
    MODULE_ITEM,
    THREAD_ITEM
};

namespace Ui {
class process_dlg;
}

class process_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit process_dlg(QWidget *parent = 0);
    ~process_dlg();

    bool add_item(ITEM_TYPE type, QString, QString, QString);

private:
    Ui::process_dlg *ui;
    int module_row;
    int thread_row;
};

#endif // PROCESS_DLG_H
