#include "ConsoleWidget.h"
#include "ConsoleWidgetPrivate.h"

#include <QScrollBar>

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QPlainTextEdit(parent),
    d(NULL)
{
    d = new ConsoleWidgetPrivate(this);

    d->m_DefaultCharFormat.setForeground(palette().foreground());
    d->m_DefaultCharFormat.setFontWeight(QFont::Normal);

    setReadOnly(true);
}

void ConsoleWidget::setEventLevelColor(const int &EventLevel, const QColor &color)
{
    QTextCharFormat charFormat;
    charFormat.setForeground(color);
    setEventLevelCharFormat(EventLevel, charFormat);
}
void ConsoleWidget::setEventLevelCharFormat(const int &EventLevel, const QTextCharFormat &charFormat)
{
    d->m_EventLevelCharFormats[EventLevel] = charFormat;
}
void ConsoleWidget::messageEvent(const int &eventLevel, const QString &message)
{
    bool wasScrolledToBottom = isScrolledToBottom();

    QString text = message;
    text.remove('\r');
    if(!text.endsWith('\n')) {
        text.append('\n');
    }

    QTextCursor cursor = QTextCursor(document());
    cursor.movePosition(QTextCursor::End);
    cursor.beginEditBlock();
    cursor.insertText(text, d->m_EventLevelCharFormats.value(eventLevel, d->m_DefaultCharFormat));
    cursor.endEditBlock();

    // Scroll to bottom, only if the display was there when we started
    if(wasScrolledToBottom) {
        scrollToBottom();
    }
}


bool ConsoleWidget::isScrolledToBottom() const
{
    QScrollBar *vert = verticalScrollBar();
    return vert->value() == vert->maximum();
}

void ConsoleWidget::scrollToBottom()
{
    QScrollBar *vert = verticalScrollBar();
    vert->setValue(vert->maximum());
}

void ConsoleWidget::resizeEvent(QResizeEvent *event)
{
    bool wasScrolledToBottom = isScrolledToBottom();

    QPlainTextEdit::resizeEvent(event);

    // Remain scrolled to the bottom after the resize event!
    if(wasScrolledToBottom) {
        scrollToBottom();
    }
}



/***** PRIVATE IMPLEMENTATION *****/

ConsoleWidgetPrivate::ConsoleWidgetPrivate(ConsoleWidget *parent) :
    QObject(parent),
    q(parent)
{
}
