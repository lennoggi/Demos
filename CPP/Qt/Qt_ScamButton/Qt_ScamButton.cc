/* ==========================================
 * A QtWidgets app showing a clickable button
 * ========================================== */

#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>


// Window size
#define WX 1000.
#define WY 500.

// Button's bottom-left corner
#define BT_X0 100.
#define BT_Y0 100.

// Button parameters
#define FONT_SIZE 20.
#define  IMG_SIZE 128

using namespace std;



int main(int argc, char **argv) {
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(WX, WY);

    //QPushButton button("Click here to win $10000");
    //QPushButton button;
    QPushButton button(&window);


    // Set button width and height so as to center the button within the window
    constexpr auto bt_w = WX - 2.*BT_X0;
    constexpr auto bt_h = WY - 2.*BT_Y0;

    if (bt_w <= 0.) {
        cout << "Button width = " << bt_w << " is non-positive" << endl;
        return 1;
    }

    if (bt_h <= 0.) {
        cout << "Button height = " << bt_h << " is non-positive" << endl;
        return 1;
    }

    button.setGeometry(BT_X0, BT_Y0, bt_w, bt_h);


    button.setText("Click here to win $10000");
    button.setToolTip("Click!");

    QFont font("Monospace");
    font.setPointSize(FONT_SIZE);
    button.setFont(font);

    QIcon icon("../MickeyMouse.svg");
    //button.setIcon(QIcon::fromTheme("face-smile"));
    button.setIcon(icon);
    button.setIconSize(QSize(IMG_SIZE, IMG_SIZE));

    window.show();
    //button.show();

    QObject::connect(&button, &QPushButton::clicked, [&button](){
        QMessageBox::information(&button, "IDIOT", "Just gained full access to your bank account");
    });

    return app.exec();  // app.exec() starts the event loop, which listens for the user's input
}
