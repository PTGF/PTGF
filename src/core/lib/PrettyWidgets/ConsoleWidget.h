#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QPlainTextEdit>

#include "Global.h"

class ConsoleWidgetPrivate;

class ConsoleWidget : public QPlainTextEdit
{
    Q_OBJECT
    DECLARE_PRIVATE(ConsoleWidget)

public:
    explicit ConsoleWidget(QWidget *parent = 0);

    void setEventLevelColor(const int &EventLevel, const QColor &color);
    void setEventLevelCharFormat(const int &EventLevel, const QTextCharFormat &charFormat);

    bool isScrolledToBottom() const;

public slots:
    void messageEvent(const int &eventLevel, const QString &message);

    void scrollToBottom();

protected:
    virtual void resizeEvent(QResizeEvent *event);

};

#endif // CONSOLEWIDGET_H
