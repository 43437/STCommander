
#ifndef QMLINWIDGET_H
#define QMLINWIDGET_H


#include <QWidget>

class QTabWidget;
class QmlInWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QmlInWidget(QWidget *parent = nullptr);

signals:

private:
    void Init();

private:
    QTabWidget* m_pTabWidget;
};

#endif // QMLINWIDGET_H
