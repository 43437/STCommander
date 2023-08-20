
#include "historycmdmodel.h"

void HistoryCMDModel::SRowData::Reset()
{
    m_Name.clear();
    m_cmd = 0;
    m_vecParam.clear();
}

HistoryCMDModel::SRowData::SRowData()
{
    Reset();
}

const int HistoryCMDModel::MAX_CNT = 10;
HistoryCMDModel::HistoryCMDModel(QObject *parent/* = nullptr*/) : QAbstractTableModel(parent)
{
    Init();
}

void HistoryCMDModel::Init()
{
    m_Headers << "Name" << "CMD";
}

void HistoryCMDModel::Resort()
{
    beginResetModel();

    QList<SRowData> lstMarkData;
    for (auto itDat = m_lstRowData.begin(); m_lstRowData.end() != itDat; )
    {
        auto& dat = *itDat;
        if (!dat.m_Name.isEmpty())
        {
            lstMarkData << dat;
            itDat = m_lstRowData.erase(itDat);
        }
        else
        {
            ++itDat;
        }
    }

    lstMarkData.append(m_lstRowData);
    m_lstRowData = lstMarkData;

    endResetModel();
}

Qt::ItemFlags HistoryCMDModel::flags(const QModelIndex &index) const
{
    if (index.column() == eCol_Name)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    }
    return QAbstractTableModel::flags(index);
}

QVariant HistoryCMDModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (Qt::DisplayRole == role)
    {
        auto row = index.row();
        if (row >= 0 && row < m_lstRowData.size())
        {
            const auto& rowDat = m_lstRowData[row];
            auto col = index.column();
            switch (col)
            {
            case eCol_Name:
                return rowDat.m_Name;
                break;
            case eCol_CMD:
                return QString::number(rowDat.m_cmd, 16);
                break;
            }
        }
        else
        {
            //error
        }
    }
    return QVariant();
}

bool HistoryCMDModel::setData(const QModelIndex &index, const QVariant &value, int role/* = Qt::EditRole*/)
{
    if (Qt::EditRole == role
        && eCol_Name == index.column())
    {
        auto row = index.row();
        if (row >= 0 && row < DataSize())
        {
            auto& dat = m_lstRowData[row];
            dat.m_Name = value.toString();
            Resort();
        }
    }

    return QAbstractItemModel::setData(index, value, role);
}

int HistoryCMDModel::rowCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    return m_lstRowData.size();
}

int HistoryCMDModel::columnCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    return eCol_To - eCol_From;
}

QVariant HistoryCMDModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (Qt::Horizontal == orientation)
    {
        if (section >= eCol_From && section < eCol_To)
        {
            return m_Headers[section];
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool HistoryCMDModel::AddData(uint8_t cmd, QByteArray& param)
{
    bool bAdded = true;
    beginResetModel();
    if (m_lstRowData.size() >= MAX_CNT)
    {
        auto itRow = std::find_if(m_lstRowData.begin(), m_lstRowData.end(), [](auto& dat){
            return dat.m_Name.isEmpty();
        });
        if (m_lstRowData.end() != itRow)
        {
            m_lstRowData.erase(itRow);
        }
        else
        {
            bAdded = false;
        }
    }

    if (bAdded)
    {
        SRowData stuSRowData;
        stuSRowData.m_cmd = cmd;
        stuSRowData.m_vecParam = param;

        m_lstRowData << stuSRowData;
    }
    endResetModel();

    return bAdded;
}

bool HistoryCMDModel::GetData(const int row, SRowData& dat) const
{
    bool bRet = false;
    if (m_lstRowData.size() > row && row >= 0)
    {
        dat = m_lstRowData[row];
        bRet = true;
    }
    return bRet;
}

void HistoryCMDModel::ClearData()
{
    beginResetModel();
    m_lstRowData.clear();
    endResetModel();
}

void HistoryCMDModel::ClearNoMarkData()
{
    beginResetModel();
    m_lstRowData.removeIf([](auto& dat){
        return dat.m_Name.isEmpty();
    });
    endResetModel();
}

int HistoryCMDModel::DataSize() const
{
    return m_lstRowData.size();
}
