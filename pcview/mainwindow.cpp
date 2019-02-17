#include "mainwindow.h"
#include "ui_mainwindow.h"

BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   this->setStatusBar(0);
    process_pop_menu = new QMenu();

    enum_process();
    is_enum_reged = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enum_process()
{
    for(int i = ui->tableWidget->rowCount(); i >= 0; i--)
    {
        ui->tableWidget->removeRow(i);
    }
    // set header
    QStringList header;
    QString str = QString("进程name");
    header<<"进程id"<< str << "父进程ID";
    ui->tableWidget->setHorizontalHeaderLabels(header);


    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
           return ;
    }
//    typedef struct tagPROCESSENTRY32 {
//                                      DWORD dwSize;//结构大小
//                                      DWORD cntUsage;//此进程的引用计数
//                                      DWORD th32ProcessID;//进程ID
//                                      DWORD th32DefaultHeapID;//进程默认堆ID
//                                      DWORD th32ModuleID;//进程模块ID
//                                      DWORD cntThreads;//此进程开启的线程计数
//                                      DWORD th32ParentProcessID;//父进程ID
//                                      LONG pcPriClassBase;//线程优先权
//                                      DWORD dwFlags;//保留
//                                      char szExeFile[MAX_PATH];//进程名
//                                    } PROCESSENTRY32;
    PROCESSENTRY32 pe32      = {0};

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //  Walk the snapshot of the processes, and for each process,
    //  display information.

    if (Process32First(hSnapShot, &pe32))
    {
        int nRow = 0;

        do
        {
            std::wstring strName = pe32.szExeFile;
            QString str2 = QString::fromStdWString(strName);

            QString str = QString("pid: %1 name: %2").arg(pe32.th32ProcessID).arg(str2);

            //qDebug() <<  str << endl;

            ui->tableWidget->insertRow(nRow);

            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(pe32.th32ProcessID));
            ui->tableWidget->setItem(nRow, 0, newItem);

            QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(str2));
            ui->tableWidget->setItem(nRow, 1, newItem2);
            if(pe32.th32ProcessID > 4)
            {
                QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(pe32.th32ParentProcessID));
                ui->tableWidget->setItem(nRow, 2, newItem3);
            }
            else
            {
                QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg("--"));
                ui->tableWidget->setItem(nRow, 2, newItem3);
            }
            nRow++;
        }
        while (Process32Next(hSnapShot, &pe32));

//        //change color
//        for (int i = 0;i < ui->tableWidget->rowCount();i++)
//        {
//            if (i % 2 == 0)
//             {
//                 for (int j = 0;j < ui->tableWidget->columnCount();j++)
//                 {
//                      QTableWidgetItem *item = ui->tableWidget->item(i,j);
//                     if (item)
//                     {
//                          const QColor color = QColor(215,255,225);
//                          item->setBackgroundColor(color);
//                     }

//                 }
//             }
//        }
    }
    else
    {
    }
    // Do not forget to clean up the snapshot object.
    //不可编辑
//    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
//    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    change_style(ui->tableWidget);

    CloseHandle (hSnapShot);
}

void MainWindow::change_style(QTableWidget* tw)
{
    //change color
    for (int i = 0;i < tw->rowCount();i++)
    {
        if (i % 2 == 0)
         {
             for (int j = 0;j < tw->columnCount();j++)
             {
                  QTableWidgetItem *item = tw->item(i,j);
                 if (item)
                 {
                      const QColor color = QColor(255,241,249);
                      item->setBackgroundColor(color);
                 }

             }
         }
        else
        {
            for (int j = 0;j < tw->columnCount();j++)
            {
                 QTableWidgetItem *item = tw->item(i,j);
                if (item)
                {
                     const QColor color = QColor(255,221,235);
                     item->setBackgroundColor(color);
                }

            }

        }
    }
    tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tw->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    tw->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行

    tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:pink;}"); //设置表头背景色
    tw->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度

    QHeaderView *header = tw->verticalHeader();
    header->setHidden(true);// 隐藏行号
}

BOOL EnumChildProc(HWND hwndChild, LPARAM lParam)
{
    if (NULL == hwndChild)
    {
        qDebug() <<"EnumChildProc == NULL";
    }
    qDebug() <<"EnumChildProc" << hwndChild;
    return TRUE;
}


void MainWindow::enum_file()
{
    for(int i = ui->cb_file->count(); i >=0; i--)
    {
        ui->cb_file->removeItem(i);
    }

    //得到系统所有盘符
    foreach (QFileInfo my_info, QDir::drives())
        {
            qDebug()<<my_info.absoluteFilePath();
            ui->cb_file->addItem(my_info.absoluteFilePath());
        }

    // set header
    QStringList header;
    header<< "文件" << "大小" << "目录" << "修改日期";
    ui->tw_file->setHorizontalHeaderLabels(header);


    enum_file_dir("c:\\");
}

void MainWindow::enum_reg()
{
    if (is_enum_reged == true)
    {
        return;
    }

    // set header
    QStringList header;
    header<< "名称"
          << "类型"
          << "数据";
    ui->tw_regkey->setHorizontalHeaderLabels(header);

    for(int i = ui->tw_regkey->rowCount(); i >= 0; i--)
    {
        ui->tw_regkey->removeRow(i);
    }
    change_style(ui->tw_regkey);
    ui->tv_regkey->setHeaderLabels(QStringList()<<"Computer");
    ui->tv_regkey->clear();

    enum QStyle::StandardPixmap Icon = (enum QStyle::StandardPixmap)21;
    QTreeWidgetItem* item1=new QTreeWidgetItem(QStringList()<<"HKEY_CLASSES_ROOT");
    item1->setIcon(0 ,QApplication::style()->standardIcon(Icon));
    ui->tv_regkey->addTopLevelItem(item1);

    QTreeWidgetItem* item2=new QTreeWidgetItem(QStringList()<<"HKEY_CURRENT_USER");
    item2->setIcon(0 ,QApplication::style()->standardIcon(Icon));
    ui->tv_regkey->addTopLevelItem(item2);

    QTreeWidgetItem* item3=new QTreeWidgetItem(QStringList()<<"HKEY_LOCAL_MACHINE");
    item3->setIcon(0 ,QApplication::style()->standardIcon(Icon));
    ui->tv_regkey->addTopLevelItem(item3);

    QTreeWidgetItem* item4=new QTreeWidgetItem(QStringList()<<"HKEY_USERS");
    item4->setIcon(0 ,QApplication::style()->standardIcon(Icon));
    ui->tv_regkey->addTopLevelItem(item4);

    QTreeWidgetItem* item5=new QTreeWidgetItem(QStringList()<<"HKEY_CURRENT_CONFIG");
    item5->setIcon(0, QApplication::style()->standardIcon(Icon));
    ui->tv_regkey->addTopLevelItem(item5);
    qDebug() <<"done";
    is_enum_reged = true;
}

void MainWindow::enum_service()
{
    // set header
    QStringList header;
    header<< "服务名称"
          << "服务显示名称"
          << "运行情况"
          << "详细描述信息";
    ui->tw_service->setHorizontalHeaderLabels(header);

    for(int i = ui->tw_service->rowCount(); i >= 0; i--)
    {
        ui->tw_service->removeRow(i);
    }


        BOOL bRet = FALSE;
        SC_HANDLE hSCM = NULL;              // 服务数据库句柄
        char *pBuf = NULL;                  // 缓冲区指针
        DWORD dwBufSize = 0;                // 传入的缓冲长度
        DWORD dwBufNeed = 0;                // 需要的缓冲长度
        DWORD dwNumberOfService = 0;        // 返回的服务个数
        ENUM_SERVICE_STATUS_PROCESS *pServiceInfo = NULL;   // 服务信息

        // 建立了一个到服务控制管理器的连接，并打开指定的数据库
        hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
        if(NULL == hSCM)
        {
            return;
        }

        // 获取需要的缓冲区大小
        EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
            NULL, dwBufSize, &dwBufNeed, &dwNumberOfService, NULL, NULL);

        // 多设置存放1个服务信息的长度
        dwBufSize = dwBufNeed + sizeof(ENUM_SERVICE_STATUS_PROCESS);
        pBuf = (char *)malloc(dwBufSize);
        if(NULL == pBuf)
        {
            return;
        }
        memset(pBuf, 0, dwBufSize);

        // 获取服务信息
        bRet = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
            (LPBYTE)pBuf, dwBufSize, &dwBufNeed, &dwNumberOfService, NULL, NULL);
        if(bRet == FALSE)
        {
            ::CloseServiceHandle(hSCM);
            free(pBuf);
            return;
        }

        pServiceInfo = (LPENUM_SERVICE_STATUS_PROCESS)pBuf;
        // 打印取得的服务信息
        int nRow = 0;
        for(unsigned int i = 0; i < dwNumberOfService; i++)
        {
            ui->tw_service->insertRow(nRow);
            //拿服务的启动方式
//            SC_HANDLE hSrv = OpenService(hSCM, pServiceInfo[i].lpServiceName, SERVICE_QUERY_CONFIG);
//            DWORD dwNeed;
//            LPQUERY_SERVICE_CONFIG pQsc;

//            QueryServiceConfig(hSrv, pQsc, 0, &dwNeed);
//            if (dwNeed > 0)
//            {
//                dwNeed = dwNeed + sizeof(LPQUERY_SERVICE_CONFIG);
//                pQsc = (LPQUERY_SERVICE_CONFIG)malloc(dwNeed);
//                ZeroMemory(pQsc, dwNeed);
//                if (QueryServiceConfig(hSrv, pQsc, dwNeed, &dwNeed) != 0)
//                {
//                    //TODO
//                     qDebug() << "query info"<<QString::number(pQsc->dwStartType) << endl;
//                     QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(QString::number(pQsc->dwStartType)));
//                     ui->tw_service->setItem(nRow, 0, newItem);
//                }
//                free(pQsc);
//            }

            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(QString::fromStdWString(pServiceInfo[i].lpServiceName)));
            ui->tw_service->setItem(nRow, 0, newItem);

            QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg(QString::fromStdWString(pServiceInfo[i].lpDisplayName)));
            ui->tw_service->setItem(nRow, 1, newItem1);

            QString str_status;
            switch (pServiceInfo[i].ServiceStatusProcess.dwCurrentState)
            {
            case SERVICE_STOPPED:
                str_status = "停止";
                break;
            case SERVICE_RUNNING:
                str_status = "正在运行";
                break;
            case SERVICE_PAUSED:
                str_status = "暂停";
                break;
            default:
                break;
            }

            QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(str_status));
            ui->tw_service->setItem(nRow, 2, newItem2);

            //拿服务的描述信息
            SC_HANDLE hSrv = OpenService(hSCM, (LPCTSTR)pServiceInfo[i].lpServiceName, SERVICE_QUERY_CONFIG);
            if (NULL == hSrv)
            {
                break;
            }
            DWORD dwNeed = 0;
            LPSERVICE_DESCRIPTION pSd = NULL;

            QueryServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwNeed);
            if (dwNeed > 0)
            {
                pSd = (LPSERVICE_DESCRIPTION)malloc(dwNeed);
                if (NULL == pSd)
                {
                    break;
                }
                ZeroMemory(pSd, dwNeed);
                //qDebug() << "QueryServiceConfig2 step1";
                if (QueryServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, (PBYTE)pSd, dwNeed, &dwNeed))
                {
                    //do some thing
                    if(pSd->lpDescription != NULL)
                    {
                      // qDebug() << "QueryServiceConfig2 step2" << QString::fromStdWString(pSd->lpDescription) << endl;
                      QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(QString::fromStdWString(pSd->lpDescription)));
                      ui->tw_service->setItem(nRow, 3, newItem3);
                    }
                    else
                    {
                        QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg("******这个服务很懒"));
                        ui->tw_service->setItem(nRow, 3, newItem3);
                    }
                }
                free(pSd);
            }
            ::CloseServiceHandle(hSrv);
            nRow++;
        }
        // 关闭打开的服务句柄
        bRet = ::CloseServiceHandle(hSCM);
        if(bRet == FALSE)
        {
            qDebug() << "CloseServiceHandle error." << endl;
        }

        free(pBuf);
    change_style(ui->tw_service);
}

void MainWindow::enum_window()
{
    ui->lab_MouseStyle->setStyleSheet("QPushButton{background-image: url(:/res/design_ic_visibility.png);}");
    for(int i = ui->cb_select_wnd->count(); i >= 0; i--)
    {
        ui->cb_select_wnd->removeItem(i);
    }
    ui->tw_wnd->clear();
    ui->tw_wnd->setHeaderLabels(QStringList()<<"窗口");

    //得到所有窗口
    auto hwnd = GetDesktopWindow();
    hwnd = GetWindow(hwnd, GW_CHILD);
    if(hwnd == NULL)
    {
        qDebug() <<"hwnd == NULL";
    }

    while (hwnd != NULL)
    {
        wchar_t name[255];
        GetWindowText(hwnd, (LPWSTR)name, 255);
        QString wnd_name = QString::fromWCharArray(name);
        if (wnd_name == NULL | wnd_name == ""){wnd_name = "no title";}

        QString qname = wnd_name.append("@").append(QString::number((int)hwnd));
        ui->cb_select_wnd->addItem(qname);
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<qname);
        ui->tw_wnd->addTopLevelItem(item);

        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);

    }


}

void MainWindow::spy_plusplus()
{

}

void MainWindow::show_wnd_info(HWND hwnd)
{
    qDebug() <<  hwnd;
    wchar_t pszFileName[MAX_PATH];
    wchar_t lpClassName[MAX_PATH];
    wchar_t windowText[MAX_PATH];
    RECT wnd_rect;
    RECT client_rect;

    /* 检索与指定的窗口句柄关联的模块的完整路径和文件名称 WINUSERAPI UINT WINAPI GetWindowModuleFileNameW(
                       _In_ HWND hwnd,
                       _Out_writes_to_(cchFileNameMax, return) LPWSTR pszFileName,
                       _In_ UINT cchFileNameMax
                   );
    */
    GetWindowModuleFileName(hwnd, pszFileName, MAX_PATH);
    //qDebug() << QString::fromWCharArray(pszFileName);

    GetClassName(hwnd, lpClassName, MAX_PATH);
    qDebug() << QString::fromWCharArray(lpClassName);
    ui->lb_class_name->setText(QString::fromWCharArray(lpClassName));

    GetWindowText(hwnd, windowText, MAX_PATH);
    qDebug() << QString::fromWCharArray(windowText);
    ui->lb_title->setText(QString::fromWCharArray(windowText));

    GetWindowRect(hwnd, &wnd_rect);
    qDebug() << "wnd_rect" << wnd_rect.bottom << wnd_rect.left;
    QString rect_str ;
    rect_str.append("LEFTTOP").append( "(").append( QString::number(wnd_rect.top) ).append( ",").append( QString::number(wnd_rect.left) ).append( ")")
            .append("RIGHTBOTTOM").append( "(").append( QString::number(wnd_rect.bottom) ).append( ",").append( QString::number(wnd_rect.right) ).append( ")");
    ui->lb_wnd_rect->setText(rect_str);

    GetClientRect(hwnd, &client_rect);
    qDebug() << "client_rect" << client_rect.bottom << client_rect.left;
    QString cl_str ;
    cl_str.append("LEFTTOP").append( "(").append( QString::number(client_rect.top) ).append( ",").append( QString::number(client_rect.left) ).append( ")")
            .append("RIGHTBOTTOM").append( "(").append( QString::number(client_rect.bottom) ).append( ",").append( QString::number(client_rect.right) ).append( ")");
    ui->lb_custom_rect->setText(cl_str);

    int hexnum = (int)hwnd;
    QString hexstr = QString::number(hexnum,16);//表示转换成16进制存入字符串
    qDebug() << hexstr;
    ui->lb_handle->setText(hexstr);

    int in_id = (int)GetWindowLong(hwnd, -12);
    qDebug() <<"id" << in_id;
    ui->lb_id->setText("0x00000000(0)");

    int linstsnce = (int)GetWindowLong(hwnd, -6);
    qDebug() <<"HINSTANCE" << linstsnce;
    ui->lb_instance->setText(QString::number(linstsnce));

    EnumChildWindows(hwnd, EnumChildProc, NULL);

}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << index << endl;
    switch(index)
    {
    case 0:
        {
            enum_process();
        }
        break;
    case 1:
        {
            enum_file();
        }
        break;
    case 2:
        {
            enum_reg();
        }
        break;
    case 3:
        {
            enum_service();
        }
        break;
    case 4:
        {
            enum_window();
        }
        break;
    case 5:
        {
            spy_plusplus();
        }
        break;
    default:
        break;
    }
}


void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    process_dlg* result_dlg = new process_dlg(this);

    //拿选中的进程id
    if (index.column() != 0)//其他列，不处理
    {
        qDebug() << "other column" << endl;
        return;
    }

    DWORD process_id = index.data().toInt();
    qDebug() << process_id << endl;
    if (process_id == 0)//没权限，不处理
    {
        qDebug() << "access denied" << endl;
        return;
    }

    //遍历模块
    MODULEENTRY32  me32 = {0};
    HANDLE hModule ;
    BOOL bModule = FALSE;
    me32.dwSize = sizeof(MODULEENTRY32);
    if (0 != process_id)                   //获取进程PID不为0的模块信息
    {
        hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);  //获取模块快照
        if (hModule != INVALID_HANDLE_VALUE)
        {
            bModule = Module32First(hModule, &me32);      //获取第一个模块信息,即进程相应可执行文件的信息
            while(bModule)
            {
                //TODO
                qDebug() << "me32.th32ModuleID" << me32.th32ModuleID << endl;
                qDebug() << me32.th32ProcessID << me32.modBaseSize<< endl;

                result_dlg->add_item(MODULE_ITEM,
                                     QString::number(me32.th32ModuleID),
                                     QString::number(me32.th32ProcessID),
                                     QString::number(me32.modBaseSize));
                bModule = Module32Next(hModule, &me32);  //获取其他模块信息
            }
            CloseHandle(hModule);
        }
    }
    /*typedef struct tagMODULEENTRY32 {
     DWORD   dwSize;
     DWORD   th32ModuleID;
     DWORD   th32ProcessID;
     DWORD   GlblcntUsage;
     DWORD   ProccntUsage;
     BYTE  * modBaseAddr;
     DWORD   modBaseSize;
     HMODULE hModule;
     TCHAR   szModule[MAX_MODULE_NAME32 + 1];
     TCHAR   szExePath[MAX_PATH];
    } MODULEENTRY32;
    typedef MODULEENTRY32 *PMODULEENTRY32;
     */


    //遍历线程
    THREADENTRY32 th32 = {0};
    HANDLE hThread ;
    BOOL bThread = FALSE;
    th32.dwSize = sizeof(THREADENTRY32);
    if (0 != process_id)                   //获取进程PID不为0的信息
    {
        hThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, process_id);  //获取快照
        if (hThread != INVALID_HANDLE_VALUE)
        {
            bThread = Thread32First(hThread, &th32);      //获取第一个模块信息,即进程相应可执行文件的信息
            while(bThread)
            {
                //TODO
                if(process_id == th32.th32OwnerProcessID)//仅列举本进程的
                {
                    qDebug() << "th32.th32ThreadID" << th32.th32ThreadID<< th32.tpBasePri <<th32.th32OwnerProcessID << endl;
                    result_dlg->add_item(THREAD_ITEM, QString::number(th32.th32ThreadID), QString::number(th32.tpBasePri), QString::number(th32.th32OwnerProcessID));
                }
                bThread = Thread32Next(hThread, &th32);  //获取其他模块信息
            }
            CloseHandle(hThread);
        }
    }
    /*
     typedef struct tagTHREADENTRY32 {
          DWORD dwSize;       //   sizeof(THREADENTRY32)
          DWORD cntUsage;
          DWORD th32ThreadID;  // 线程ID
          DWORD th32OwnerProcessID;  // 线程所有的进程ID
          LONG tpBasePri;     // 基础线程优先级
          LONG tpDeltaPri;
          DWORD dwFlags;

        } THREADENTRY32
     */


    //遍历句柄

    result_dlg->exec();
}

//遍历指定目录
void MainWindow::enum_file_dir(QString path)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }
    else
    {
        for(int i = ui->tw_file->rowCount(); i >= 0; i--)
        {
            ui->tw_file->removeRow(i);
        }
        ui->label_cur_dir->setText(path);
    }


    //设置项目
    int nRow = 0;
    for(int i=0; i<file_count; i++)
    {
        QFileInfo file_info = list.at(i);

        ui->tw_file->insertRow(nRow);

        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(file_info.fileName()));
        QFileIconProvider icon_provider;
        QIcon icon = icon_provider.icon(file_info);
        newItem->setIcon(icon);
        ui->tw_file->setItem(nRow, 0, newItem);

        QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1%2").arg((file_info.size() - 1024) / 1024 + 1).arg("KB"));
        ui->tw_file->setItem(nRow, 1, newItem2);

        QString str;
        if(file_info.isDir())
        {
            str = "目录";
        }
        else
        {
            str= "文件";
        }
        QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(str));
        ui->tw_file->setItem(nRow, 2, newItem3);

        QTableWidgetItem *newItem4 = new QTableWidgetItem(tr("%1").arg(file_info.lastModified().toString(("yyyy-MM-dd hh:mm:ss"))));
        ui->tw_file->setItem(nRow, 3, newItem4);

        nRow++;

    }

    change_style(ui->tw_file);

}

void MainWindow::on_cb_file_currentIndexChanged(int)
{
    qDebug()<<ui->cb_file->currentText();
    ui->label_cur_dir->setText(ui->cb_file->currentText());
    enum_file_dir(ui->cb_file->currentText());
}

void MainWindow::on_pb_file_clicked()
{
    qDebug()<<"pressed"<<endl;
}

void MainWindow::on_tw_file_itemDoubleClicked(QTableWidgetItem *item)
{
   qDebug() << item->text() << endl;
   QString path = ui->label_cur_dir->text() + item->text() + "\\";

   QDir dir(path);
   if(!dir.exists())
   {
       return;
   }
   else
   {
       ui->label_cur_dir->setText(path);
       enum_file_dir(path);
   }

}

void MainWindow::on_pb_start_svc_clicked()
{
    if (ui->tw_service->currentItem() == NULL)
    {
        qDebug() <<"nothing selected";
        return;
    }
    qDebug() << ui->tw_service->currentItem()->text() <<endl;

//    SC_HANDLE hSCM = NULL;
//    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
//    if(NULL == hSCM){ qDebug() <<"NULL == hSCM";return; }
//    SC_HANDLE hSrv = OpenService(hSCM, ui->tw_service->currentItem()->text().toStdWString().c_str(), SERVICE_QUERY_CONFIG);
//    if (NULL == hSrv){ ::CloseServiceHandle(hSCM);qDebug() <<"NULL == hSrv"; return; }

//    BOOL bret = StartService(hSrv, 0, NULL);
//    if (bret == 0)//fail
//    {
//        if (GetLastError() == ERROR_ACCESS_DENIED)
//        {
//            qDebug() << "ACCESS_DENIED";
//        }
//    }

//    ::CloseServiceHandle(hSrv);
//    ::CloseServiceHandle(hSCM);
    DoStartSvc(ui->tw_service->currentItem()->text().toStdWString().c_str());
    Sleep(3000);
    enum_service();
}

void MainWindow::on_pb_stop_svc_clicked()
{
    if (ui->tw_service->currentItem() == NULL)
    {
        qDebug() <<"nothing selected";
        return;
    }
    qDebug() << ui->tw_service->currentItem()->text() <<endl;

//    SC_HANDLE hSCM = NULL;
//    hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
//    if(NULL == hSCM){ qDebug() <<"NULL == hSCM";return; }
//    SC_HANDLE hSrv = OpenService(hSCM, ui->tw_service->currentItem()->text().toStdWString().c_str(), SERVICE_QUERY_CONFIG);
//    if (NULL == hSrv){ ::CloseServiceHandle(hSCM);qDebug() <<"NULL == hSrv"; return; }

//    ControlService(hSrv, SERVICE_CONTROL_STOP, );


//    ::CloseServiceHandle(hSrv);
//    ::CloseServiceHandle(hSCM);
    MyDoStopSvc(ui->tw_service->currentItem()->text().toStdWString().c_str());
    Sleep(3000);
    enum_service();
}

void MainWindow::on_tv_regkey_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << item->text(column);
    //拿全路径
    QStringList path;
    path << item->text(column);
    QTreeWidgetItem* rootkey = item;

    while(rootkey->parent() != NULL)
    {
        rootkey = rootkey->parent();
        path << rootkey->text(column);
        qDebug() << path;
    }
    qDebug() << "path size = " << path.size();
    QString reg_path = path.at(path.size() - 1);
    for(int i = path.size() - 2; i >= 0; i--)
    {
        reg_path = reg_path.append("\\").append(path.at(i));
        qDebug()<<"reg_path ="<< reg_path;
    }
    qDebug()<<"reg_path ="<< reg_path;

    //打开注册表
    QSettings reg(reg_path, QSettings::NativeFormat);

    QStringList groupsList=reg.childGroups();
    foreach(QString group,groupsList)
    {
       qDebug()<<"group ="<< group;
       if(!group.isEmpty())
       {
           QTreeWidgetItem* item5 = new QTreeWidgetItem(QStringList()<<group);

           enum QStyle::StandardPixmap Icon = (enum QStyle::StandardPixmap)21;
           item5->setIcon(column, QApplication::style()->standardIcon(Icon));

           item->addChild(item5);
       }
    }

}

void MainWindow::on_tv_regkey_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem*)
{
    int nRow = 0;
    for(int i = ui->tw_regkey->rowCount(); i >= 0; i--)
    {
        ui->tw_regkey->removeRow(i);
    }
    //qDebug() << "current->text(0)" << item->text(0);
    //拿全路径
    QStringList path;
    path << item->text(0);
    QTreeWidgetItem* rootkey = item;

    while(rootkey->parent() != NULL)
    {
        rootkey = rootkey->parent();
        path << rootkey->text(0);
        //qDebug() << path;
    }
    //qDebug() << "path size = " << path.size();
    QString reg_path = path.at(path.size() - 1);
    for(int i = path.size() - 2; i >= 0; i--)
    {
        reg_path = reg_path.append("\\").append(path.at(i));
       // qDebug()<<"reg_path ="<< reg_path;
    }
    qDebug()<<"reg_path ="<< reg_path;

    //打开注册表
   QSettings reg(reg_path, QSettings::NativeFormat);
   QStringList keyList=reg.childKeys();
   qDebug()<<"keyList ="<< keyList.size();
   foreach(QString key,keyList)
   {
        qDebug()<<"key ="<< key;
        QString value = reg.value(key).toString();//读
        QString szReg;
        auto reg_type = reg.value(key).type();
        if(reg_type == QVariant::Int)
        {
            qDebug()<<"type =" << reg.value(key).type();
            szReg = "REG_DWORD";
        }
        else if(reg_type == QVariant::String)
        {
            qDebug()<<"type =" << reg.value(key).type();
            szReg = "REG_SZ";
        }
        qDebug()<<"value ="<< value;

        //添加项目
        ui->tw_regkey->insertRow(nRow);
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(key));
        ui->tw_regkey->setItem(nRow, 0, newItem);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(szReg));
        ui->tw_regkey->setItem(nRow, 1, newItem2);
        QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg(value));
        ui->tw_regkey->setItem(nRow, 2, newItem3);
        nRow++;
   }
    change_style(ui->tw_regkey);
}

void MainWindow::on_pb_add_clicked()
{
    if ( ui->tv_regkey->currentItem() == NULL)
    {
        qDebug() << "no selection";
        return;
    }
    if ( ui->txt_key->toPlainText() == NULL)
    {
        qDebug() << "no key";
        return;
    }
    if ( ui->txt_value->toPlainText()== NULL)
    {
        qDebug() << "no value";
        return;
    }
     QTreeWidgetItem* item = ui->tv_regkey->currentItem();
     qDebug() <<"delete new";
     //拿全路径
     QStringList path;
     path << item->text(0);
     QTreeWidgetItem* rootkey = item;

     while(rootkey->parent() != NULL)
     {
         rootkey = rootkey->parent();
         path << rootkey->text(0);
         //qDebug() << path;
     }
     //qDebug() << "path size = " << path.size();
     QString reg_path = path.at(path.size() - 1);
     for(int i = path.size() - 2; i >= 0; i--)
     {
         reg_path = reg_path.append("\\").append(path.at(i));
        // qDebug()<<"reg_path ="<< reg_path;
     }
     qDebug()<<"reg_path ="<< reg_path;

     //打开注册表
     QSettings reg(reg_path, QSettings::NativeFormat);
     qDebug() <<"add new";
     reg.setValue(ui->txt_key->toPlainText(), ui->txt_value->toPlainText());

     if (reg.value(ui->txt_key->toPlainText()) == QVariant::Invalid)
     {
        qDebug() << reg.value(ui->txt_key->toPlainText());
        return;
     }

     int nRow = ui->tw_regkey->rowCount();
     ui->tw_regkey->insertRow(nRow);
     QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(ui->txt_key->toPlainText()));
     ui->tw_regkey->setItem(nRow, 0, newItem);
     QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg("REG_SZ"));
     ui->tw_regkey->setItem(nRow, 1, newItem2);
     QTableWidgetItem *newItem3 = new QTableWidgetItem(tr("%1").arg( ui->txt_value->toPlainText()));
     ui->tw_regkey->setItem(nRow, 2, newItem3);
     change_style(ui->tw_regkey);

     ui->txt_key->setText("");
     ui->txt_value->setText("");
}

void MainWindow::on_pb_delete_clicked()
{
    if ( ui->tv_regkey->currentItem() == NULL | ui->tw_regkey->currentItem() == NULL)
    {
        return;
    }
     QTreeWidgetItem* item = ui->tv_regkey->currentItem();
     qDebug() <<"delete new";
     //拿全路径
     QStringList path;
     path << item->text(0);
     QTreeWidgetItem* rootkey = item;

     while(rootkey->parent() != NULL)
     {
         rootkey = rootkey->parent();
         path << rootkey->text(0);
         //qDebug() << path;
     }
     //qDebug() << "path size = " << path.size();
     QString reg_path = path.at(path.size() - 1);
     for(int i = path.size() - 2; i >= 0; i--)
     {
         reg_path = reg_path.append("\\").append(path.at(i));
        // qDebug()<<"reg_path ="<< reg_path;
     }
     qDebug()<<"reg_path ="<< reg_path;

     //打开注册表
     QSettings reg(reg_path, QSettings::NativeFormat);
     QString key = ui->tw_regkey->item(ui->tw_regkey->currentRow(), 0)->text();
     qDebug() << key;
     reg.remove(key);
     ui->tw_regkey->removeRow(ui->tw_regkey->currentRow());

}


bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (is_spy == true)
        {
            // 当用户松开鼠标时, 恢复鼠标样式并且重新给鼠标样式标签设置图标
            setCursor(Qt::ArrowCursor);
            ui->lab_MouseStyle->setStyleSheet("QPushButton{background-image: url(:/res/design_ic_visibility.png);}");
            is_spy = false;
            return true;
        }
    }
    return QWidget::event(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (is_spy == true)
    {
//        QPoint pos = event->globalPos();
//        qDebug() << "move" << pos;
        POINT p;
        BOOL ret;
        ret = GetCursorPos(&p);
        if (!ret){return;}
        qDebug() << "move" << p.x << p.y;

        HWND hwnd = WindowFromPoint(p);
        if (hwnd == NULL || hwnd == INVALID_HANDLE_VALUE)
        {
            qDebug() << "null handle";
            return;
        }

        show_wnd_info(hwnd);

        RECT rect;
        if (!GetWindowRect(hwnd, &rect)){return;}
        qDebug() << rect.bottom << rect.left <<rect.right << rect.top;
        if( rect.left < 0 )
        {
            rect.left = 0;
        }

        if (rect.top < 0 )
        {
            rect.top = 0;
        }

        HWND DeskHwnd = ::GetDesktopWindow(); //取得桌面句柄
        HDC DeskDC = ::GetWindowDC(DeskHwnd); //取得桌面设备场景
        int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);

        HPEN newPen = ::CreatePen(0, 3, 0); //建立新画笔,载入DeskDC
        HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
        if(g_cur_wnd != NULL)
        {
            if (g_cur_wnd == hwnd)
            {
                   return;
            }
            else
            {
                RECT old_rect;
                if (!GetWindowRect(g_cur_wnd, &old_rect)){return;}
                if( old_rect.left < 0 )
                {
                    old_rect.left = 0;
                }

                if (old_rect.top < 0 )
                {
                    old_rect.top = 0;
                }

                ::Rectangle(DeskDC, rect.left, rect.top, rect.right, rect.bottom); //在窗口周围显示矩形
                ::Rectangle(DeskDC, old_rect.left, old_rect.top, old_rect.right, old_rect.bottom); //在窗口周围显示矩形
                g_cur_wnd = hwnd;

            }
        }
        else
        {
            ::Rectangle(DeskDC, rect.left, rect.top, rect.right, rect.bottom); //在窗口周围显示矩形
            g_cur_wnd = hwnd;
        }

        ::SetROP2(DeskDC, oldRop2);
        ::SelectObject( DeskDC, oldPen);
        ::DeleteObject(newPen);
        ::ReleaseDC( DeskHwnd, DeskDC);
        DeskDC = NULL;
    }
}



void MainWindow::on_lab_MouseStyle_clicked()
{

}

void MainWindow::on_lab_MouseStyle_pressed()
{
    ui->lab_MouseStyle->setStyleSheet("QPushButton{background-image: url(:/res/abc_popup_background_mtrl_mult.9.png);}");
     // 设置鼠标样式

     QCursor cursor ;
     QPixmap pixmap("://res/design_ic_visibility.png") ;
     cursor = QCursor(pixmap,-1,-1);
     setCursor(cursor) ;

     // 修改标志
     is_spy = true;
}

void MainWindow::on_cb_select_wnd_currentIndexChanged(int index)
{
    qDebug() << ui->cb_select_wnd->itemText(index);
    QString str = ui->cb_select_wnd->itemText(index).section('@', 1);
    qDebug() <<str;
    HWND hwnd = (HWND)str.toInt();
    show_wnd_info(hwnd);
}

void MainWindow::on_tw_wnd_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << ui->tw_wnd->currentIndex().data().toString();
    QString str = ui->tw_wnd->currentIndex().data().toString().section('@', 1);
    qDebug() <<str;
    HWND hwnd = (HWND)str.toInt();
    show_wnd_info(hwnd);
}

void MainWindow::on_tw_wnd_doubleClicked(const QModelIndex &index)
{


}

void MainWindow::on_tw_wnd_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << item->text(column);
    QString str = item->text(column).section('@', 1);
    qDebug() <<str;
    HWND hwnd_father = (HWND)str.toInt();

    HWND hwnd = GetWindow(hwnd_father, GW_CHILD);
    while (hwnd != NULL)
    {
        wchar_t name[255];
        GetWindowText(hwnd, (LPWSTR)name, 255);
        QString wnd_name = QString::fromWCharArray(name);
        if (wnd_name == NULL | wnd_name == ""){wnd_name = "no title";}

        QString qname = wnd_name.append("@").append(QString::number((int)hwnd));

        QTreeWidgetItem* item_child = new QTreeWidgetItem(QStringList()<<qname);
        item->addChild(item_child);

        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
    }

}
