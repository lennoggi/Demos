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


bool BaseTableModel::insertRows(int row,
                                int count,
                                const QModelIndex &parent) {
    Q_UNUSED(parent);
    const auto old_row_count = (this->rowCount());

    if (row >= 0 and count > 0) {
        // Clamp row insertion to valid range
        if (row > old_row_count) {
            row = old_row_count;
        }

        beginInsertRows(QModelIndex(), row, row + count - 1);

        for (int i = 0; i < count; ++i) {
            (this->Rows).insert(row + i, QStringList((this->columnCount()), QString()));
        }

        endInsertRows();
        return true;
    } else {
        return false;
    }
}


bool BaseTableModel::removeRows(int row,
                                int count,
                                const QModelIndex &parent) {
    Q_UNUSED(parent);
    const auto old_row_count = (this->rowCount());

    if (row > 0 and row < old_row_count and count > 0) {
        // Clamp row removal to valid range
        if (row + count > old_row_count) {
            count = old_row_count - row;
        }

        beginRemoveRows(QModelIndex(), row, row + count - 1);
        (this->Rows).remove(row, count);
        //(this->Rows).squeeze();  // TODO: expensive, but do this if (this->Rows).capacity() "largely" exceeds  (this->rowCount())
        endRemoveRows();
        return true;
    } else {
        return false;
    }
}


void BaseTableModel::addRow() {
    this->insertRows(this->rowCount(), 1, QModelIndex());
    return;
}

//void BaseTableModel::removeRow() {
//    this->removeRows(this->rowCount(), 1, QModelIndex());
//    return;
//}


QStringList BaseTableModel::columnHeaders() const {
    return QStringList{ "Base A", "Base B" };
}
