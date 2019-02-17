#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <windows.h>
#include <Tlhelp32.h>
#include <QMessageBox>
#include <QTableWidget>
#include <QMenu>
#include "process_dlg.h"
#include <QDir>
#include <QDateTime>
#include <QFileIconProvider>
#include <winsvc.h>
#include <tchar.h>
#include "svccontrol.h"
#include "regkey.h"
#include <QTreeWidgetItem>
#include <QSettings>
#include <QVariant>
#include <dwmapi.h>
#include <WinUser.h>
#include <QMouseEvent>


#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void enum_process();
    void enum_file();
    void enum_reg();
    void enum_service();
    void enum_window();
    void spy_plusplus();
    void show_wnd_info(HWND hwnd);


private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void enum_file_dir(QString path);

    void on_cb_file_currentIndexChanged(int index);

    void on_pb_file_clicked();

    void on_tw_file_itemDoubleClicked(QTableWidgetItem *item);

    void on_pb_start_svc_clicked();

    void on_pb_stop_svc_clicked();

    void on_tv_regkey_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_tv_regkey_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_pb_add_clicked();

    void on_pb_delete_clicked();

    void on_lab_MouseStyle_clicked();

    void on_lab_MouseStyle_pressed();

    void on_cb_select_wnd_currentIndexChanged(int index);

    void on_tw_wnd_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_tw_wnd_doubleClicked(const QModelIndex &index);

    void on_tw_wnd_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    bool MainWindow::event(QEvent *event) override;
    void MainWindow::mouseMoveEvent(QMouseEvent *event);
    QMenu* process_pop_menu;

    //设置表格样式
    void change_style(QTableWidget* tw);

    bool is_enum_reged = false;
    bool is_spy = false;

    HWND g_cur_wnd;


};

#endif // MAINWINDOW_H
