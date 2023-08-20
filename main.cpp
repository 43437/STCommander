
#include "View/mainwindow.h"
#include "Common/CommonFunction.h"
#include <QApplication>
#include <QMutex>
#include <QDir>

static QMutex mutex;
void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QMutexLocker lock(&mutex);

    auto folder = NSCommonFunction::GetLogPath() + "/" + QDate::currentDate().toString("yyyyMM");
    QDir dir(folder);
    if (!dir.exists())
    {
        dir.mkpath(folder);
    }

    auto fileName = QDate::currentDate().toString("MMdd") + ".log";
    QFile f(folder+"/"+fileName);
    if (f.open( QIODevice::Text | QIODevice::Append | QIODevice::WriteOnly) )
    {
        QTextStream ts(&f);
        ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") << msg << Qt::endl;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(myMessageOutput);

    KOT_LOG << "main";

    MainWindow w;
    w.show();
    return a.exec();
}
