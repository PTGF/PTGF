#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace Ui {
class AboutWidget;
}

class AboutWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit AboutWidget(QWidget *parent = 0);
    ~AboutWidget();
    
private:
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
