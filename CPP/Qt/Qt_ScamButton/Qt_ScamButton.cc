/* ==========================================
 * A QtWidgets app showing a clickable button
 * ========================================== */

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>

#define IMG_SIZE 128

using namespace std;


int main(int argc, char **argv) {
    QApplication app(argc, argv);

    //QPushButton button("Click here to win $10000");
    QPushButton button;
    button.setText("Click here to win $10000");
    button.setToolTip("Click!");

    QFont font("Monospace");
    button.setFont(font);

    QIcon icon("../MickeyMouse.svg");
    //button.setIcon(QIcon::fromTheme("face-smile"));
    button.setIcon(icon);
    button.setIconSize(QSize(IMG_SIZE, IMG_SIZE));

    button.show();

    QObject::connect(&button, &QPushButton::clicked, [&button](){
        QMessageBox::information(&button, "IDIOT", "Just gained full access to your bank account");
    });

    return app.exec();  // app.exec() starts the event loop, which listens for the user's input
}
