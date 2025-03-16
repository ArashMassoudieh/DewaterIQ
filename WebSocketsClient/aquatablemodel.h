#ifndef AQUATABLEMODEL_H
#define AQUATABLEMODEL_H

#include <QAbstractTableModel>
#include "aquatable.h"  // Include your AquaTable definition

class AquaTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit AquaTableModel(AquaTable *table, const QStringList &columns, QObject *parent = nullptr);

    // Required QAbstractTableModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool SetTable(AquaTable *table);
    // Custom methods
    void updateTable();

private:
    AquaTable *m_table;  // Pointer to AquaTable (data source)
};

#endif // AQUATABLEMODEL_H
