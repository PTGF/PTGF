#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

class TabWidget;

namespace Core {
namespace WindowManager {

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(bool splash = false, QWidget *parent = 0);
    ~AboutDialog();

    static void splash(int ms = 2000);

private:
    Ui::AboutDialog *ui;
    TabWidget *m_TabWidget;
};

} // namespace WindowManager
} // namespace Core
#endif // ABOUTDIALOG_H
