#ifndef BASETABLEMODEL_HH
#define BASETABLEMODEL_HH

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>


class BaseTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        explicit BaseTableModel(QObject *parent = nullptr);

        // Methods overridden from QAbstractTableModel
        int    rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex &index,
                      int role) const override;

        QVariant headerData(int section,
                            Qt::Orientation orientation,
                            int role) const override;

        // Called each time a cell is edited
        bool setData(const QModelIndex &index,
                     const QVariant    &value,
                     int role) override;

        Qt::ItemFlags flags(const QModelIndex &index) const override;

        bool insertRows(int row,
                        int count,
                        const QModelIndex &parent = QModelIndex()) override;

        bool removeRows(int row,
                        int count,
                        const QModelIndex &parent = QModelIndex()) override;
        void addRow();

    protected:
        virtual QStringList columnHeaders() const;
        QVector<QStringList> Rows;
};


#endif  // BASETABLEMODEL_HH
