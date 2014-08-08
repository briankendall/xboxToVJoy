#ifndef QMAINWIDGET_H
#define QMAINWIDGET_H

#include <QWidget>

class QMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMainWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    void createTrayIcon();
};

#endif // QMAINWIDGET_H
