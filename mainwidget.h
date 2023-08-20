#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class HistoryCMDModel;
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    void Init();
    void AttachConnection();

private:
    Ui::MainWidget *ui;
    HistoryCMDModel* m_pModel;
};

#endif // MAINWIDGET_H
