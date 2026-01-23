#ifndef FANCYTABLEMODEL_HH
#define FANCYTABLEMODEL_HH

#include "BaseTableModel.hh"


class FancyTableModel1 : public BaseTableModel {
    Q_OBJECT

    public:
        explicit FancyTableModel1(QObject *parent = nullptr);
    protected:
        QStringList columnHeaders() const override;
};

class FancyTableModel2 : public BaseTableModel {
    Q_OBJECT

    public:
        explicit FancyTableModel2(QObject *parent = nullptr);
    protected:
        QStringList columnHeaders() const override;
};


#endif  // FANCYTABLEMODEL_HH
