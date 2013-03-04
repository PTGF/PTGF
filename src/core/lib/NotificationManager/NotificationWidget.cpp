#include "NotificationWidget.h"

namespace Core {
namespace NotificationManager {


NotificationWidget::NotificationWidget(QWidget *parent) :
    QFrame(parent)
{
    setupUi();
}

NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons, QWidget *parent) :
    QFrame(parent)
{
    setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);
}

NotificationWidget::NotificationWidget(const QString &text, Icon icon, StandardButtons buttons,
                                  const QObject *reciever, const char *member, QWidget *parent) :
    QFrame(parent)
{
    setupUi();

    setIcon(icon);
    setText(text);
    setStandardButtons(buttons);

    if(reciever && member) {
        connect(this, SIGNAL(buttonClicked(StandardButton)), reciever, member);
    }
}

NotificationWidget::~NotificationWidget()
{
}

void NotificationWidget::setupUi()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setFrameShape(QFrame::Box);

    setStyleSheet("QFrame { background-color: rgb(255,255,225); }");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(9,0,9,0);
    layout->setMargin(0);
    this->setLayout(layout);

    m_IconLabel = new QLabel(this);
    m_IconLabel->setMaximumHeight(16);
    m_IconLabel->setMaximumWidth(16);
    m_IconLabel->setScaledContents(true);
    layout->addWidget(m_IconLabel);

    m_Label = new QLabel(this);
    m_Label->setWordWrap(true);
    layout->addWidget(m_Label);

    m_ButtonBox = new QDialogButtonBox(this);
    m_ButtonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_ButtonBox->setMaximumHeight(16);
    connect(m_ButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(on_buttonBox_clicked(QAbstractButton*)));
    layout->addWidget(m_ButtonBox);

    m_CloseButton = new QToolButton(this);
    m_CloseButton->setMaximumHeight(16);
    m_CloseButton->setMaximumWidth(16);
    m_CloseButton->setAutoRaise(true);
    m_CloseButton->setIcon(QIcon(":/CoreWindow/notificationClose.svg"));
    connect(m_CloseButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
    layout->addWidget(m_CloseButton);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

QString NotificationWidget::text() const
{
    return m_Label->text();
}
void NotificationWidget::setText(const QString &text)
{
    m_Label->setText(text);
}

NotificationWidget::Icon NotificationWidget::icon() const
{
    return m_Icon;
}

void NotificationWidget::setIcon(const Icon &icon)
{
    switch(m_Icon = icon) {
    case Information:
        m_IconLabel->setPixmap(QPixmap(":/CoreWindow/information.svg"));
        break;
    case Warning:
        m_IconLabel->setPixmap(QPixmap(":/CoreWindow/warning.svg"));
        break;
    case Critical:
        m_IconLabel->setPixmap(QPixmap(":/CoreWindow/critical.svg"));
        break;
    case Question:
        m_IconLabel->setPixmap(QPixmap(":/CoreWindow/question.svg"));
        break;
    case Loading:
    {
        QMovie *movie = new QMovie(":/CoreWindow/loading.gif", QByteArray(), this);
        m_IconLabel->setMovie(movie);
        movie->start();
    }
        break;
    default:
        m_IconLabel->setPixmap(QPixmap());
    }
}

QPixmap NotificationWidget::pixmap() const
{
    return *m_IconLabel->pixmap();
}
void NotificationWidget::setPixmap(const QPixmap &pixmap)
{
    m_Icon = NoIcon;
    m_IconLabel->setPixmap(pixmap);
}

NotificationWidget::StandardButtons NotificationWidget::standardButtons() const
{
    return StandardButtons(int(m_ButtonBox->standardButtons()));
}
void NotificationWidget::setStandardButtons(StandardButtons standardButtons)
{
    m_ButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(standardButtons)));
}

void NotificationWidget::on_buttonBox_clicked(QAbstractButton *button)
{
    this->hide();
    emit buttonClicked((StandardButton)m_ButtonBox->standardButton(button));
    emit closing();
    this->close();
}

void NotificationWidget::on_closeButton_clicked()
{
    this->hide();
    emit buttonClicked(Close);
    emit closing();
    this->close();
}


QList<QAbstractButton *> NotificationWidget::buttons() const
{
    return m_ButtonBox->buttons();
}


void NotificationWidget::addButton(StandardButton button)
{
    m_ButtonBox->addButton((QDialogButtonBox::StandardButton)button);
}

QPushButton *NotificationWidget::button(StandardButton button) const
{
    return m_ButtonBox->button((QDialogButtonBox::StandardButton)button);
}


void NotificationWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->count() == 1) {
        if(event->key() == Qt::Key_Escape) {
            m_CloseButton->animateClick();
            return;
        } else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            if(buttons().count() == 0) {
                m_CloseButton->animateClick();
                return;
            }
        }
    }

    QFrame::keyReleaseEvent(event);
}


} // namespace CoreWindow
} // namespace Core
