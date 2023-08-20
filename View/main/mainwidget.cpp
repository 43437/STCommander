#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "msgprocess.h"
#include "MSG/Protocol.h"
#include "historycmdmodel.h"
#include "Common/CommonFunction.h"
#include "Config.h"
#include "InfoHelper.h"
#include <QLabel>
#include <QDateTime>
#include <QSerialPortInfo>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    Init();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::Init()
{
    m_pModel = new HistoryCMDModel;

    ui->textEdit->setReadOnly(true);

    ui->tableView->setModel(m_pModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    switch (NSConfig::ePortType)
    {
    case NSConfig::ePortType_Tcp:
        ui->wgtSerial->hide();
        break;
    case NSConfig::ePortType_Serial:
        ui->wgtTcp->hide();
        break;
    default:
        KOT_LOG << "invalid type" << NSConfig::ePortType;
        break;
    }

    AttachConnection();
}

void MainWidget::AttachConnection()
{
    connect(&InfoHelper::Instance(), &InfoHelper::SigInfo, this, [=](QString info){

        ui->textEdit->append(info);
    });

    connect(ui->btnScan, &QPushButton::clicked, this, [=](){
        auto lstPort = QSerialPortInfo::availablePorts();
        ui->cbCom->clear();
        for (auto& port : lstPort)
        {
            ui->cbCom->addItem(port.portName());
        }
    });
    connect(ui->btnConnect, &QPushButton::clicked, this, [=](){
        if (MsgProcess::Instance().StartProcess(ui->cbCom->currentText()))
        {
            ui->textEdit->append("serial connect success.");
        }
    });

    connect(ui->btnListen, &QPushButton::clicked, this, [=](){
        if (MsgProcess::Instance().StartProcess(ui->edtListenPort->text().toInt()))
        {
            ui->textEdit->append("tcp listen success.");
        }
    });

    connect(ui->tableView, &QAbstractItemView::clicked, this, [=](const QModelIndex &index){

        auto row = index.row();
        HistoryCMDModel::SRowData dat;
        if (m_pModel->GetData(row, dat))
        {
            ui->edtCMD->setText(QString("%1").arg(dat.m_cmd, 2, 16, QChar('0')));
            QStringList lstParam;
            for (auto& param : dat.m_vecParam)
            {
                lstParam << QString("%1").arg((uint8_t)param, 2, 16, QChar('0'));
            }
            ui->edtParam->setText(lstParam.join(" "));
        }
        else
        {

        }
    });

    connect(ui->btnSend, &QPushButton::clicked, this, [=](){
        MsgProcess::Instance().Send(ui->lineEdit->text().toLocal8Bit());
    });
    connect(&MsgProcess::Instance(), &MsgProcess::SigRecv, this, [=](const NSProtocol::SCMD stuCMD){
        QStringList lstDat;
        lstDat << QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss.zzz");
        lstDat << QString("cmd[%1]").arg(stuCMD.m_cmd);
        lstDat << QString("sn[%1]").arg(stuCMD.m_sn);
        lstDat << QString("type[%1]").arg(stuCMD.m_type);
        QStringList lstParam;
        for (auto& param : stuCMD.m_vecParam)
        {
            lstParam << QString::number(param);
        }
        lstDat << QString("param[%1]").arg(lstParam.join(" "));
        ui->textEdit->append(lstDat.join(" "));

        //info type analyze
        if (NSProtocol::TYPE_INFO == stuCMD.m_type)
        {
            QByteArray info;
            for (auto& dat : stuCMD.m_vecParam)
            {
                info.append(dat);
            }
            ui->textEdit->append(info);
        }
    });
    connect(ui->btnCMD, &QPushButton::clicked, this, [=](){

        bool bOk = true;
        uint8_t cmd = ui->edtCMD->text().toUInt(&bOk, 16);

        QByteArray arrParam;
        auto param = ui->edtParam->text();
        if (!param.isEmpty())
        {
            auto lstParam = param.split(" ");

            for (auto& param : lstParam)
            {
                KOT_LOG << lstParam;
                arrParam.append((uint8_t)param.toUInt(&bOk, 16));
            }
        }

        QByteArray arrCMD;
        MsgProcess::BuildCMD(NSProtocol::TYPE_CMD, MsgProcess::GetSN(), cmd, arrParam, arrCMD);
        MsgProcess::Instance().Send(arrCMD);

        m_pModel->AddData(cmd, arrParam);
    });
    connect(ui->btnClear, &QPushButton::clicked, this, [=](){
        ui->textEdit->clear();
    });
    connect(ui->btnClearHistory, &QPushButton::clicked, this, [=](){
        m_pModel->ClearNoMarkData();
    });
    connect(ui->btnClearAllHistory, &QPushButton::clicked, this, [=](){
        m_pModel->ClearData();
    });
}
