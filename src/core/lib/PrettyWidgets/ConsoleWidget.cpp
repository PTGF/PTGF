#include "ConsoleWidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QPlainTextEdit(parent)
{
    m_DefaultCharFormat.setForeground(palette().foreground());
    m_DefaultCharFormat.setFontWeight(QFont::Normal);

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
    m_EventLevelCharFormats[EventLevel] = charFormat;
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
    cursor.insertText(text, m_EventLevelCharFormats.value(eventLevel, m_DefaultCharFormat));
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
