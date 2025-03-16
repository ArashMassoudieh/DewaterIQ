#include "aquatablemodel.h"

AquaTableModel::AquaTableModel(AquaTable *table, const QStringList &columns, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_table = new AquaTable();
    for (int i=0; i<columns.count(); i++)
    {
        if (table->ColumnNames().contains(columns[i]))
            m_table->AppendColumn(columns[i],table->GetColumn(columns[i]));
    }

}

int AquaTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_table->size();  // Number of columns

}

int AquaTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_table->isEmpty() ? 0 : m_table->first().size(); // Number of rows based on first column size
}

bool AquaTableModel::SetTable(AquaTable *table)
{
    m_table = table;
    return true;
}

QVariant AquaTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (index.row() < m_table->size() && index.column() < m_table->at(index.column()).size())
    {
        //return m_table->at(index.row()).at(index.column()); // Return the value at (row, col)
        return QLocale(QLocale::English).toString(m_table->at(index.row()).at(index.column()), 'f', 1);
    }

    return QVariant();
}

QVariant AquaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)  // Column headers
    {
        return m_table->ColumnNames().at(section);
    }
    else if (orientation == Qt::Vertical)  // Row numbers
    {
        return section + 1;
    }

    return QVariant();
}

bool AquaTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.column() < m_table->size() && index.row() < m_table->at(index.column()).size())
    {
        (*m_table)[index.column()][index.row()] = value.toDouble(); // Modify data
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags AquaTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void AquaTableModel::updateTable()
{
    beginResetModel();
    endResetModel();
}
