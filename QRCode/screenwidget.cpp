#include "screenwidget.h"
#include "QZXing.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QPainter>
#include <QFileDialog>
#include <QClipboard>


ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent)
{ 
    InitWdiget();
    InitConnect();
    m_rubberband->installEventFilter(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

ScreenWidget::~ScreenWidget()
{
    hide();
    delete a_begin;
    delete a_get;
    delete a_save;
    delete a_cancel;
    delete a_search;
    delete a_exit;
    delete trayicon;
    delete traymenu;
    delete m_rubberband;
    delete m_shortcut;
}
void ScreenWidget::InitWdiget()     //初始化变量
{
    m_isDrawing = false;
    m_selected = false;
    leftMousePress = false;
    m_rubberband = new QRubberBand(QRubberBand::Rectangle);

    /*————————————————————————————————————————————————————————————————————————————————————-*/
    a_begin = new QAction(tr("截屏"),this);
    a_get = new QAction(tr("完成"),this);
    a_save = new QAction(tr("保存"),this);
    a_cancel = new QAction(tr("取消"),this);
    a_search = new QAction(tr("扫码"),this);
    a_exit = new QAction(tr("退出"),this);

    trayicon = new QSystemTrayIcon(this);
    traymenu = new QMenu(this);
    /*————————————————————————————————————————————————————————————————————————————————————-*/
    trayicon->setIcon(QIcon(":/Icon/trayicon"));
    trayicon->setContextMenu(traymenu);
    traymenu->addAction(a_begin);
    traymenu->addSeparator();
    traymenu->addAction(a_exit);
    trayicon->show();       //显示托盘
    trayicon->setToolTip(tr("截图工具"));
    trayicon->showMessage(tr("Yoooo~~~"), tr("Alt+Q开始截图"), QSystemTrayIcon::Information, 1000);
    m_rubberband->addAction(a_get);
    m_rubberband->addAction(a_save);
    m_rubberband->addAction(a_search);
    m_rubberband->addAction(a_cancel);
    m_rubberband->setContextMenuPolicy(Qt::ActionsContextMenu);
    /*————————————————————————————————————————————————————————————————————————————————————-*/
    m_shortcut = new QxtGlobalShortcut(QKeySequence("Alt+Q"),this);
}

void ScreenWidget::InitConnect() //初始化槽
{
    connect(a_begin,SIGNAL(triggered()),this,SLOT(BeginScreenShot()));
    connect(a_get,SIGNAL(triggered()),this,SLOT(GetScreenShot()));
    connect(a_save,SIGNAL(triggered()),this,SLOT(SaveScreenShot()));
    connect(a_cancel,SIGNAL(triggered()),this,SLOT(CancelScreenShot()));
    connect(a_search,SIGNAL(triggered()),this,SLOT(SearchCode()));
    connect(a_exit,SIGNAL(triggered()),this,SLOT(close()));
    connect(m_shortcut,SIGNAL(activated()),this,SLOT(BeginScreenShot()));
}

bool ScreenWidget::eventFilter(QObject *o, QEvent *e)       //rubberband点击事件
{
    if (o != m_rubberband)
    {
        return QWidget::eventFilter(o, e);
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (e);
    //true 鼠标左键按下且按键还未弹起
    leftMousePress = true;
    if ((mouseEvent->button() == Qt::LeftButton)
        && (mouseEvent->type() == QEvent::MouseButtonPress))
    {
        m_beginPos = mouseEvent->pos();
        return true;
    }
    //true 鼠标左键按下并拖动
    if ((mouseEvent->type() == QEvent::MouseMove)
        && (leftMousePress))
    {
        m_endPos = mouseEvent->pos();
        int x = m_rubberband->x(),y = m_rubberband->y();
        int w = m_rubberband->width(),h = m_rubberband->height();
        int offx = m_endPos.x() - m_beginPos.x();
        int offy = m_endPos.y() - m_beginPos.y();
        if(x+offx>=0 && x+offx+w <= QApplication::desktop()->width())
            x += offx;
        if(y + offy >= 0 && y+offy+h <= QApplication::desktop()->height())
            y += offy;
        m_rubberband->move(x,y);
        return true;
    }

    //鼠标左键松开
    if ((mouseEvent->button() == Qt::LeftButton)
        && (mouseEvent->type() == QEvent::MouseButtonRelease))
    {
        leftMousePress = false;
        return true;
    }

    return false;
}


void ScreenWidget::showEvent(QShowEvent *event)
{
    showFullScreen();
    setCursor(Qt::CrossCursor);
    QWidget::showEvent(event);
}

void ScreenWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_screen);
    if(m_selected == false)
    {
        QPen pen(QColor(0,255,255));
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(QApplication::desktop()->rect());
    }
    QWidget::paintEvent(event);
}

void ScreenWidget::mousePressEvent(QMouseEvent *event)  //鼠标按下事件
{

    m_beginPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        if(m_rubberband->isVisible())
        {
            m_rubberband->hide();
            m_rubberband->show();
            return;
        }
        m_isDrawing = true;
        m_selected = true;
        update();
        m_rubberband->setGeometry(QRect(m_beginPos,QSize()));
        m_rubberband->show();
    }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *event)   //鼠标移动事件
{
    if (m_isDrawing) {
        m_endPos = event->pos();
        m_rubberband->setGeometry(QRect(m_beginPos, m_endPos).normalized());
    }
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *event)    //鼠标释放事件
{
    m_endPos = event->pos();
    if (event->button() == Qt::LeftButton
            && (!m_rubberband->isVisible())) {
        m_isDrawing = false;
    }
    else if(event->button() == Qt::RightButton){
        CancelScreenShot();
    }
}

void ScreenWidget::mouseDoubleClickEvent(QMouseEvent *event)    //双击事件
{
    if(m_rubberband->size() == QSize(0,0)){
        m_rubberband->hide();
        m_rubberband->setGeometry(QApplication::desktop()->rect().normalized());
        GetScreenShot();
    }
    else
        GetScreenShot();
    QWidget::mouseDoubleClickEvent(event);
}

void ScreenWidget::BeginScreenShot()    //开始截图
{
    m_screen = QPixmap::grabWindow(QApplication::desktop()->winId());
    show();
}


void ScreenWidget::SaveScreenShot()  //保存截图
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                tr("Open Image"),
                                                ".",
                                                tr("Image Files(*.JPG *.PNG)"));
    if (!fileName.isNull())
    {
        QPixmap pix = m_screen.grabWidget(this,m_rubberband->x(),m_rubberband->y(),m_rubberband->width(),m_rubberband->height());
        pix.save(fileName);
        m_rubberband->hide();
        hide();
        m_screen = QPixmap();
    }
    else{
        m_rubberband->hide();
        m_rubberband->show();
    }
}

void ScreenWidget::GetScreenShot()      //完成截图
{
    QPixmap pix = m_screen.grabWidget(this,m_rubberband->x(),m_rubberband->y(),m_rubberband->width(),m_rubberband->height());
    QApplication::clipboard()->setImage(pix.toImage());
    m_selected = false;
    m_rubberband->hide();
    hide();
    m_screen = QPixmap();
}

void ScreenWidget::CancelScreenShot() //取消截图
{
    if(m_selected){
        m_selected = false;
        m_rubberband->hide();
        update();
    }
    else{
        m_selected = false;
        m_rubberband->hide();
        hide();
        m_screen = QPixmap();
    }
}

void ScreenWidget::SearchCode() //扫描二维码
{
    QZXing qzxing;
    QString s = qzxing.decodeImage(m_screen.grabWidget(
               this,m_rubberband->x(),m_rubberband->y(),m_rubberband->width(),m_rubberband->height()).toImage());
    if("" != s)
    {
        GetScreenShot();
        QDesktopServices::openUrl(QUrl(s));
    }
}



