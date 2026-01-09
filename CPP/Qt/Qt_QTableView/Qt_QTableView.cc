#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QStandardItemModel>
#include <QtWidgets/QHeaderView>

// Number of rows and columns in the table
#define NROWS 10
#define NCOLS 20

// Other table parameters
#define TITLE "A simple QTableView"


int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QStandardItemModel model(NROWS, NCOLS);

    // Set horizontal and vertical labels
    QStringList horizontal_labels, vertical_labels;

    for (auto j = decltype(NCOLS){0}; j < NCOLS; ++j) {
        // Or insert() or operators += or <<
        horizontal_labels.append(QString("Column %1").arg(j));
    }

    for (auto i = decltype(NROWS){0}; i < NROWS; ++i) {
        // Or insert() or operators += or <<
        vertical_labels.append(QString("Row %1").arg(i));
    }

    model.setHorizontalHeaderLabels(horizontal_labels);
    model.setVerticalHeaderLabels(vertical_labels);


    // Create a view object based on the model
    QTableView view;
    view.setModel(&model);
    view.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view.show();

    return app.exec();
}
