#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QRubberBand>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <windows.h>
#include "MyGlobalShortCut.h"

class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = 0);

private:
    QAction* a_begin;           //截屏
    QAction* a_get;             //完成
    QAction* a_save;            //保存
    QAction* a_cancel;          //取消
    QAction* a_search;          //扫码
    QAction* a_exit;            //退出

    QSystemTrayIcon* trayicon;  //托盘
    QMenu* traymenu;            //托盘菜单
    QRubberBand* m_rubberband;  //橡皮框

    QPoint m_beginPos;           //起始点
    QPoint m_endPos;             //终点
    QPixmap m_screen;            //截图
    MyGlobalShortCut* m_shortcut;//全局热键

    bool m_isDrawing;           //是否在画
    bool m_selected;            //是否选中
    bool leftMousePress;        //左键按下

private slots:
    void BeginScreenShot();     //开始截图
    void SaveScreenShot();      //保存截图
    void GetScreenShot();       //完成截图
    void CancelScreenShot();    //取消截图
    void SearchCode();          //扫描二维码
private:
    void InitWdiget();          //初始化变量
    void InitConnect();         //初始化槽

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);    //触发事件
    void closeEvent(QCloseEvent* event);                //关闭事件
    void showEvent(QShowEvent* event);                  //显示事件
    void paintEvent(QPaintEvent* event);                //绘图事件
    void mousePressEvent(QMouseEvent *event);           //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);            //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);         //鼠标释放事件
    void mouseDoubleClickEvent(QMouseEvent *event);     //双击事件
};

#endif // SCREENWIDGET_H
