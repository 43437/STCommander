
#ifndef HISTORYCMDMODEL_H
#define HISTORYCMDMODEL_H

#include <QAbstractTableModel>

class HistoryCMDModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum ECol
    {
        eCol_From,
        eCol_Name = eCol_From,
        eCol_CMD,
        eCol_To,
    };
    struct SRowData
    {
        QString m_Name;
        uint8_t m_cmd;
        QByteArray m_vecParam;
        void Reset();
        SRowData();
    };

public:
    explicit HistoryCMDModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)  override;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool AddData(uint8_t cmd, QByteArray& param);
    bool GetData(const int row, SRowData& dat) const;
    void ClearData();
    void ClearNoMarkData();

    int DataSize() const;
private:
    void Init();
    void Resort();

private:
    QStringList m_Headers;
    QList<SRowData> m_lstRowData;
    const static int MAX_CNT;
};

#endif // HISTORYCMDMODEL_H
