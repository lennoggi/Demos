#include "FancyTableModel.hh"


// ***** Fancy table 1 *****
FancyTableModel1::FancyTableModel1(QObject *parent) : BaseTableModel(parent) {
    // Do nothing
}

QStringList FancyTableModel1::columnHeaders() const {
    auto headers = BaseTableModel::columnHeaders();
    headers.append({"Fancy1 A", "Fancy1 B"});
    return headers;
}


// ***** Fancy table 2 *****
FancyTableModel2::FancyTableModel2(QObject *parent) : BaseTableModel(parent) {
    // Do nothing
}

QStringList FancyTableModel2::columnHeaders() const {
    auto headers = BaseTableModel::columnHeaders();
    headers.append({"Fancy2 A", "Fancy2 B", "Fancy2 C"});
    return headers;
}
