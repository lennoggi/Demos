#include "BaseTableModel.hh"


BaseTableModel::BaseTableModel(QObject *parent) : QAbstractTableModel(parent) {
    // Do nothing
}


int BaseTableModel::rowCount(const QModelIndex &parent) const {
    return (this->Rows).size();
}

int BaseTableModel::columnCount(const QModelIndex &parent) const {
    return (this->columnHeaders()).size();
}


/* Called by the view (or anything else using this model) when they want to know
 * what to show and how to show it for a particular cell and row                */
QVariant BaseTableModel::data(const QModelIndex &index,
                              int role) const {
    if (index.isValid() and (role == Qt::DisplayRole or role == Qt::EditRole)) {
        return (this->Rows).at(index.row()).at(index.column());
    } else {
        return QVariant();
    }
}


QVariant BaseTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return (this->columnHeaders()).at(section);
    } else {
        return QVariant();
    }
}


// Called each time a cell is edited
bool BaseTableModel::setData(const QModelIndex &index,
                             const QVariant    &value,
                             int role) {
    if (role == Qt::EditRole and index.isValid()) {
        (this->Rows)[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index);
        return true;
    } else {
        return false;
    }
}


Qt::ItemFlags BaseTableModel::flags(const QModelIndex &index) const {
    if (index.isValid()) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return Qt::NoItemFlags;
    }
}


QStringList BaseTableModel::columnHeaders() const {
    return QStringList{ "Base A", "Base B" };
}
