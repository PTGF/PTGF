#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QtGui>

class ConsoleWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget *parent = 0);

    void setEventLevelColor(const int &EventLevel, const QColor &color);
    void setEventLevelCharFormat(const int &EventLevel, const QTextCharFormat &charFormat);

    bool isScrolledToBottom() const;


public slots:
    void messageEvent(const int &eventLevel, const QString &message);

    void scrollToBottom();


signals:

private:
    QMap<int, QTextCharFormat> m_EventLevelCharFormats;
    QTextCharFormat m_DefaultCharFormat;

};

#endif // CONSOLEWIDGET_H
