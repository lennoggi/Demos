/* ========================================================================
 * Simple model/window example create a base table and allowing the user to
 * choose a specialized table type by selecting the desired option from a
 * drop-down menu
 * ======================================================================== */
#include <QApplication>
#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include "include/BaseTableModel.hh"
#include "include/FancyTableModel.hh"

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 300


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Table choice example");

    auto *menu = new QComboBox(&window);
    menu->addItems({
        "Basic table",
        "Fancy table type 1",
        "Fancy table type 2"
    });

    auto *add_row_button = new QPushButton("New row", &window);
    add_row_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    add_row_button->setStyleSheet(
//        "QPushButton {\
//           background-color: darkGreen;\
//           color: white;\
//           border-radius: 3px;\
//           left-padding: 12px;\
//           right-padding: 12px;\
//         }\
//         QPushButton:hover {\
//           background-color: green;\
//         }\
//         QPushButton:pressed {\
//           font-weight: bold;\
//         }"\
//    );

    auto *remove_rows_button = new QPushButton("Remove selected rows", &window);
    remove_rows_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    remove_rows_button->setEnabled(false);

    auto *view = new QTableView(&window);
    //view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->setAlternatingRowColors(true);
    view->setStyleSheet(
        "QTableView {\
           alternate-background-color: darkGray;\
        }"
    );

    auto *buttons_layout = new QHBoxLayout();
    buttons_layout->addStretch();  // Fit to width and push to the right
    buttons_layout->addWidget(add_row_button);
    buttons_layout->addWidget(remove_rows_button);

    auto *window_layout = new QVBoxLayout(&window);
    window_layout->addWidget(menu);
    window_layout->addLayout(buttons_layout);
    window_layout->addWidget(view);


    // Start with a basic table
    BaseTableModel *model = new BaseTableModel();
    view->setModel(model);

    // Select table model from drop-down menu
    QObject::connect(
        menu,                                                 // Signal sender
        QOverload<int>::of(&QComboBox::currentIndexChanged),  // Signal type
        [&](int index) {         // What to do: choose the desired table model
            if (model) {
                view->setModel(nullptr);
                model->deleteLater();  // Schedule deletion (safe if signals/slots are active)
                model = nullptr;
            }

            switch (index) {
                case 0:
                    model = new BaseTableModel();
                    break;
                case 1:  // NOTE: implicit upcast
                    model = new FancyTableModel1();
                    break;
                case 2:  // NOTE: implicit upcast
                    model = new FancyTableModel2();
                    break;
                default:
                    model = new BaseTableModel();
                    break;
            }

            // Need to reconnect the button click when changing the table model
            QObject::disconnect(add_row_button, nullptr, nullptr, nullptr);
            QObject::connect(add_row_button, &QPushButton::clicked, model, &BaseTableModel::addRow);

            view->setModel(model);
        }
    );


    // Add row when button is clicked before any rows are added
    QObject::connect(add_row_button, &QPushButton::clicked, model, &BaseTableModel::addRow);

    window.show();
    window.resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    return app.exec();
}
