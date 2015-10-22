#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMouseEvent>

class ScreenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenWidget(QWidget *parent = 0);

private:
    QAction* a_begin;           //截屏
    QAction* a_save;            //保存
    QAction* a_cancel;          //取消
    QAction* a_search;          //扫码
    QAction* a_exit;            //退出

    QSystemTrayIcon* trayicon;  //托盘
    QMenu* traymenu;            //托盘菜单

    QPoint m_beginPos;           //起始点
    QPoint m_endPos;             //终点
    QPixmap m_screen;            //截图
    QPixmap m_tempscreen;        //缓冲区

    QLabel* m_Rectsize;         //截屏大小显示标签
    QLabel* m_Func;             //截屏后功能选择标签

    int m_screenwidth;          //整个屏幕的宽度
    int m_screenheight;         //整个屏幕的高度
    bool m_isDrawing;           //是否在画
    bool m_selected;            //是否选中


public slots:
    void BeginScreenShot();     //开始截图
    void GetScreenShot();       //完成截图
    void CancelScreenShot();    //取消截图
    void SearchCode();          //扫描二维码
public:
    void Init();                //初始化菜单

public:
    void closeEvent(QCloseEvent* event);                //关闭事件
    void showEvent(QShowEvent* event);                  //显示事件
    void paintEvent(QPaintEvent* event);                //绘图事件
    void mousePressEvent(QMouseEvent *event);           //鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);            //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event);         //鼠标释放事件
};

#endif // SCREENWIDGET_H
