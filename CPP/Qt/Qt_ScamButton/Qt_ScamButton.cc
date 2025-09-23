/* =============================================================================
 * This is a simple QtWidgets app. QtWidgets typically used for desktop GUI apps.
 * ============================================================================= */

//#include <QtWidgets>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QPushButton button("Click here to win $10000");
    button.show();
    return app.exec();
}
