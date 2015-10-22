#include "screenwidget.h"
#include <QDialog>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QClipboard>
#include <QDebug>

ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent)
{
    Init();
    m_isDrawing = false;
    m_selected = false;
    setWindowFlags(Qt::WindowStaysOnTopHint);
}


void ScreenWidget::Init() //初始化菜单
{
    a_begin = new QAction(tr("截屏"),this);
    a_begin->setShortcut(tr("Ctrl+S"));
    a_save = new QAction(tr("完成"),this);
    a_cancel = new QAction(tr("取消"),this);
    a_search = new QAction(tr("扫码"),this);
    a_exit = new QAction(tr("退出"),this);
    a_exit->setShortcut(Qt::CTRL + Qt::Key_X);  //未解决问题

    connect(a_begin,SIGNAL(triggered()),this,SLOT(BeginScreenShot()));
    connect(a_save,SIGNAL(triggered()),this,SLOT(GetScreenShot()));
    connect(a_cancel,SIGNAL(triggered()),this,SLOT(CancelScreenShot()));
    connect(a_search,SIGNAL(triggered()),this,SLOT(SearchCode()));
    connect(a_exit,SIGNAL(triggered()),this,SLOT(close()));

    trayicon = new QSystemTrayIcon(this);
    trayicon->setIcon(QIcon(":/Icon/trayicon"));
    traymenu = new QMenu(this);
    trayicon->setContextMenu(traymenu);
    traymenu->addAction(a_begin);
    traymenu->addSeparator();
    traymenu->addAction(a_exit);   

    trayicon->show();       //显示托盘
    trayicon->setToolTip(tr("截图工具"));
    //trayicon->showMessage(tr("Yoooo~~~"), tr("莫慌，有我"), QSystemTrayIcon::Information, 5000);
    qDebug() << "初始化完成" ;
}

void ScreenWidget::closeEvent(QCloseEvent *event)       //关闭事件
{
    hide();
    delete a_begin;
    delete a_save;
    delete a_cancel;
    delete a_search;
    delete a_exit;
    delete trayicon;
    delete traymenu;
    qDebug() << "关闭";
    event->accept();
}


void ScreenWidget::showEvent(QShowEvent *event)
{
    showFullScreen();
    setCursor(Qt::CrossCursor);
}

void ScreenWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_tempscreen);
    //QDialog::paintEvent(event);
}

void ScreenWidget::mousePressEvent(QMouseEvent *event)  //鼠标按下事件
{
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        m_beginPos = event->pos();
    }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *event)   //鼠标移动事件
{
    if (m_isDrawing) {
        m_tempscreen = m_screen;
        m_endPos = event->pos();
        m_screenwidth = abs(m_endPos.x() - m_beginPos.x());
        m_screenheight = abs(m_endPos.y() - m_beginPos.y());
        qDebug() << m_screenwidth << "x" << m_screenheight;
        //paint(m_tempscreen);
    }
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *event)    //鼠标释放事件
{
    if (event->button() == Qt::LeftButton) {
        m_endPos = event->pos();
        if(m_beginPos == m_endPos)
        {
            QApplication::clipboard()->setImage(m_tempscreen.toImage());
            hide();
        }
        m_isDrawing = false;
        m_selected = true;
        //shotRect = QRect(m_beginPos, m_endPos);
        //paint(m_tempscreen);
        //savePixMap();
    }
    else if(event->button() == Qt::RightButton){
        hide();
    }
}


void ScreenWidget::BeginScreenShot()    //开始截图
{
    m_screen = QPixmap::grabWindow(QApplication::desktop()->winId());
    m_tempscreen = m_screen;
    show();
    qDebug() << "开始截图" ;
}


void ScreenWidget::GetScreenShot()  //完成截图
{
    qDebug() << "完成截图" ;
}

void ScreenWidget::CancelScreenShot() //取消截图
{
    qDebug() << "取消截图" ;
}

void ScreenWidget::SearchCode() //扫描二维码
{
    qDebug() << "扫描二维码" ;
}

