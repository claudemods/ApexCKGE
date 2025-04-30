#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QStringList>
#include <QWidget>
#include <QFrame>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVBoxLayout *mainLayout;
    QList<QPushButton*> buttons;

    void executeCommand(const QString &command);
    void createHeaderSection();
};

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), mainLayout(new QVBoxLayout) {

    QWidget *centralWidget = new QWidget(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Add title at the very top
    QLabel *title = new QLabel("Apex System Tool v3.0", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 16pt;"
        "font-weight: bold;"
        "color: #e06c75;"
        "margin-bottom: 15px;"
    );
    mainLayout->addWidget(title);

    // Create the header section
    createHeaderSection();

    // Middle box (compact)
    QWidget *middleBox = new QWidget(this);
    middleBox->setStyleSheet(
        "background-color: #2d2d2d;"
        "border-radius: 8px;"
        "padding: 10px;"
    );
    middleBox->setMaximumWidth(280);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleBox);
    middleLayout->setSpacing(10);
    middleLayout->setContentsMargins(10, 10, 10, 10);

    QStringList buttonTexts = {
        "KDE Dev Options",
        "Theme Apps",
        "Custom Scripts",
        "Information",
        "Konsole"
    };

    QStringList commands = {
        "https://www.youtube.com/watch?v=WYiBjX0bHuI",  // Updated KDE Dev Options to open YouTube
        "./claudemods-themechanger.bin",
        "./corescripts.bin",
        "kate /opt/claudemods-apps/Information.txt",
        "konsole"
    };

    for (int i = 0; i < buttonTexts.size(); ++i) {
        QPushButton *button = new QPushButton(buttonTexts[i], middleBox);
        button->setFixedHeight(35);
        button->setStyleSheet(
            "QPushButton {"
            "   background-color: #3a3a3a;"
            "   color: #e06c75;"  // Changed text color to red (#e06c75)
        "   border: 1px solid #4a4a4a;"
        "   border-radius: 6px;"
        "   padding: 5px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4a4a4a;"
        "   border: 1px solid #5a5a5a;"
        "}"
        );
        middleLayout->addWidget(button);
        buttons.append(button);

        connect(button, &QPushButton::clicked, [this, commands, i]() {
            if (i == 0) {  // Special handling for KDE Dev Options (YouTube link)
                QDesktopServices::openUrl(QUrl(commands[i]));
            } else {
                executeCommand(commands[i]);
            }
        });
    }

    mainLayout->addWidget(middleBox, 0, Qt::AlignCenter);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Claudemods Tool v3.0");
    resize(420, 700);
}

void MainWindow::createHeaderSection() {
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setStyleSheet(
        "background-color: #1a1a1a;"
        "border-radius: 10px;"
        "padding: 15px;"
    );

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setSpacing(15);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    // First row of badges
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->setSpacing(10);

    QStringList row1Texts = {"GTA Mods", "OS Linux"};
    QStringList row1Links = {
        "https://www.gtainside.com/user/mapmods100",
        "https://www.linux.org"
    };

    for (int i = 0; i < row1Texts.size(); ++i) {
        QLabel *badge = new QLabel(headerWidget);
        badge->setText(QString("<a href='%1' style='text-decoration:none; color:white;'>%2</a>").arg(row1Links[i], row1Texts[i]));
        badge->setStyleSheet(
            "background-color: #e06c75;"
            "color: white;"
            "padding: 8px 15px;"
            "border-radius: 6px;"
            "font-weight: bold;"
        );
        badge->setOpenExternalLinks(true);
        row1->addWidget(badge);
    }
    headerLayout->addLayout(row1, 0);

    // Second row of badges (distros)
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->setSpacing(10);

    QStringList row2Texts = {"DISTRO Arch", "DISTRO CachyOS", "DISTRO Debian", "DISTRO Ubuntu"};
    QStringList row2Links = {
        "https://archlinux.org",
        "https://cachyos.org/",
        "https://www.debian.org",
        "https://ubuntu.com/"
    };
    QStringList row2Colors = {"#56b6c2", "#00FFFF", "#CE0058", "#E95420"};

    for (int i = 0; i < row2Texts.size(); ++i) {
        QLabel *badge = new QLabel(headerWidget);
        badge->setText(QString("<a href='%1' style='text-decoration:none; color:white;'>%2</a>").arg(row2Links[i], row2Texts[i]));
        badge->setStyleSheet(
            QString("background-color: %1; color: white; padding: 8px 12px; border-radius: 6px; font-weight: bold;").arg(row2Colors[i])
        );
        badge->setOpenExternalLinks(true);
        row2->addWidget(badge);
    }
    headerLayout->addLayout(row2, 0);

    // Divider
    QFrame *divider = new QFrame(headerWidget);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #e06c75;");
    divider->setFixedHeight(2);
    headerLayout->addWidget(divider);

    // Third row of badges (website links)
    QHBoxLayout *row3 = new QHBoxLayout();
    row3->setSpacing(10);

    QStringList row3Texts = {"GTA Inside v1.5", "Google Drive v2.0", "Github v2.1", "Pling v2.0"};
    QStringList row3Links = {
        "https://www.gtainside.com/user/mapmods100",
        "https://drive.google.com/drive/folders/1MH0CHGvwdDzGSXpjgfBqvfty_asq6cqf",
        "https://github.com/claudemods",
        "https://www.pling.com/u/claudemods/"
    };

    for (int i = 0; i < row3Texts.size(); ++i) {
        QLabel *badge = new QLabel(headerWidget);
        badge->setText(QString("<a href='%1' style='text-decoration:none; color:black;'>%2</a>").arg(row3Links[i], row3Texts[i]));
        badge->setStyleSheet(
            "background-color: #FFD700;"
            "color: black;"
            "padding: 8px 12px;"
            "border-radius: 6px;"
            "font-weight: bold;"
        );
        badge->setOpenExternalLinks(true);
        row3->addWidget(badge);
    }
    headerLayout->addLayout(row3, 0);

    mainLayout->addWidget(headerWidget);
}

MainWindow::~MainWindow() {
    qDeleteAll(buttons);
}

void MainWindow::executeCommand(const QString &command) {
    QProcess process;
    process.startDetached(command);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"
