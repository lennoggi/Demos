/* =========================================
 * A QtWidgets app showing an editable table
 * ========================================= */
#include <string>

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QHeaderView>

using namespace std;


// Number of rows and columns in the table
#define NROWS 10
#define NCOLS 20

// Other table parameters
#define TITLE "A simple QWidgetTable"


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QTableWidget table(NROWS, NCOLS);
    table.setWindowTitle(TITLE);


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

    table.setHorizontalHeaderLabels(horizontal_labels);
    table.setVerticalHeaderLabels(vertical_labels);


    // This stretches the table as it gets resized
    table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // Make all fields editable (with some default)
    for (auto i = decltype(NROWS){0}; i < NROWS; ++i) {
        for (auto j = decltype(NCOLS){0}; j < NCOLS; ++j) {
            //table.setItem(i, j, new QTableWidgetItem(""));
            //table.setItem(i, j, new QTableWidgetItem("Fill here"));
            //table.setItem(i, j, new QTableWidgetItem(QString("Cell %1").arg(i*NCOLS + j)));
            table.setItem(i, j, new QTableWidgetItem(QString::number(i*NCOLS + j)));
        }
    }

    table.show();
    return app.exec();
}
