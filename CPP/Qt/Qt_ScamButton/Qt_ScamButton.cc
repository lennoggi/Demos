/* ==========================================
 * A QtWidgets app showing a clickable button
 * ========================================== */
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QMessageBox>
#include <QTimer>


// Window size
#define WX 1000.
#define WY 500.

// Button's bottom-left corner
#define BT_X0 100.
#define BT_Y0 100.

// Button parameters
#define FONT_SIZE 20.
#define  IMG_SIZE 128.

// Progress bar's bottom-left corner
#define PBAR_X0 100.
#define PBAR_Y0 100.

/* The progress bar timer fires every so milliseconds
 * NOTE: must be INTEGER to avoid floats in the following preprocessor expression */
#define PROG_TIMESTEP_MS 20
#if (PROG_TIMESTEP_MS <= 0)
#error "Progress bar timer firing timestep must be positive"
#endif

/* Progress percent step (must lie in (0,100])
 * NOTE: must be INTEGER to avoid floats in the following preprocessor expression */
#define PROG_PERCENT_STEP 1

#if (PROG_PERCENT_STEP <= 0 || PROG_PERCENT_STEP > 100)
#error "Progress bar percent step must lie in (0,100]"
#endif



using namespace std;

int main(int argc, char **argv) {
    /* -------------
     * Sanity checks
     * ------------- */
    // Set button width and height so as to center the button within the window
    constexpr auto bt_w = WX - 2.*BT_X0;
    constexpr auto bt_h = WY - 2.*BT_Y0;

    static_assert(bt_w > 0., "Button width must be positive");
    static_assert(bt_h > 0., "Button heigth must be positive");


    /* -----------------------------
     * Begin creating the Qt objects
     * ----------------------------- */
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(WX, WY);

    //QPushButton button("Click here to win $10000");
    //QPushButton button;
    QPushButton button(&window);
    button.setGeometry(BT_X0, BT_Y0, bt_w, bt_h);
    button.setText("Click here to win $10000");
    button.setToolTip("Click!");

    QFont font("Monospace");
    font.setPointSize(FONT_SIZE);
    button.setFont(font);

    QIcon icon("MickeyMouse.svg");
    //button.setIcon(QIcon::fromTheme("face-smile"));
    button.setIcon(icon);
    button.setIconSize(QSize(IMG_SIZE, IMG_SIZE));

    QProgressBar pbar(&window);
    pbar.setRange(0, 100);
    pbar.setValue(0);
    //pbar.setGeometry(BT_X0, BT_Y0, bt_w, bt_h);  // Same location and size as the button
    pbar.setGeometry(button.geometry());
    pbar.setVisible(false);

    QTimer timer(&pbar);

    window.show();


    // Actions performed once the button is clicked
    QObject::connect(&button, &QPushButton::clicked, [&button, &pbar, &timer](){
        pbar.show();

        QObject::connect(&timer, &QTimer::timeout, [&button, &pbar, &timer](){
            auto newprog = pbar.value() + PROG_PERCENT_STEP;
            pbar.setValue(newprog);
            if (newprog >= pbar.maximum()) {
                timer.stop();
                QMessageBox::information(&button, "IDIOT", "Just gained full access to your bank account");
            }
        });

        timer.start(PROG_TIMESTEP_MS);  // The timer fires every PROG_TIMESTEP_MS milliseconds
    });

    return app.exec();  // app.exec() starts the event loop, which listens for the user's input
}
