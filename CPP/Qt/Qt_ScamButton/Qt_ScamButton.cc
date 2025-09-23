/* ==========================================
 * A QtWidgets app showing a clickable button
 * ========================================== */

//#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>

using namespace std;


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QPushButton button("Click here to win $10000");
    button.show();

    QObject::connect(&button, &QPushButton::clicked, [&button](){
        //cout << "Just gained full access to your bank account" << endl;
        QMessageBox::information(&button, "IDIOT", "Just gained full access to your bank account");
    });

    return app.exec();  // app.exec() starts the event loop, which listens for the user's input
}
