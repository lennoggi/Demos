/* ========================================================================
 * Simple model/window example create a base table and allowing the user to
 * choose a specialized table type by selecting the desired option from a
 * drop-down menu
 * ======================================================================== */
#include <QApplication>
#include <QWidget>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>

#include "include/BaseTableModel.hh"
#include "include/FancyTableModel.hh"


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

    auto *controls = new QVBoxLayout(&window);
    controls->addWidget(menu);

    auto *view = new QTableView(&window);
    //view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    controls->addWidget(view);


    // Start with a basic table
    BaseTableModel *model = new BaseTableModel();
    view->setModel(model);

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

            view->setModel(model);
        }
    );


    window.show();
    window.resize(800, 400);

    return app.exec();
}
