#include <QApplication>
#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QScrollBar>
#include <QInputDialog>
#include <QLineEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        this->setFixedSize(800, 600);
        QWidget *centralWidget = new QWidget(this);
        QGridLayout *layout = new QGridLayout(centralWidget);

        // Title
        QLabel *title = new QLabel("System Management Tool", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 16pt; font-weight: bold; margin-bottom: 15px;");
        layout->addWidget(title, 0, 0, 1, 2);

        // Output log
        outputLog = new QTextEdit(this);
        outputLog->setReadOnly(true);
        outputLog->setStyleSheet("font-family: monospace; font-size: 10pt;");
        layout->addWidget(outputLog, 1, 0, 1, 2);

        // Buttons
        addButton(layout, 2, 0, "Theme Changer", "./claudemods-themeapps.bin", false);
        addButton(layout, 2, 1, "Start Bauh", "./bauh-0.10.7-x86_64.AppImage", false);
        addButton(layout, 3, 0, "Start Stacer", "pkexec ./Stacer-1.1.0-x64.AppImage", true);
        addButton(layout, 3, 1, "Full System Upgrade", "sudo -S pacman -Syu --noconfirm", true);
        addButton(layout, 4, 0, "Refresh Database", "sudo -S pacman -Sy --noconfirm", true);
        addButton(layout, 4, 1, "Clear Cache", "sudo -S pacman -Sc --noconfirm && sudo -S rm -rf /var/cache/pacman/pkg/*", true);

        // Kernel management buttons
        QPushButton *btnUnlockKernel = new QPushButton("Unlock Kernel", this);
        btnUnlockKernel->setStyleSheet("font-size: 10pt; padding: 8px;");
        layout->addWidget(btnUnlockKernel, 5, 0);
        connect(btnUnlockKernel, &QPushButton::clicked, this, [this]() {
            if (checkPassword()) {
                unlockKernel();
            }
        });

        QPushButton *btnLockKernel = new QPushButton("Lock Kernel", this);
        btnLockKernel->setStyleSheet("font-size: 10pt; padding: 8px;");
        layout->addWidget(btnLockKernel, 5, 1);
        connect(btnLockKernel, &QPushButton::clicked, this, [this]() {
            if (checkPassword()) {
                lockKernel();
            }
        });

        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
    }

    ~MainWindow() = default;

private slots:
    void executeCommand(const QString &command, bool requirePassword) {
        if (requirePassword && !checkPassword()) {
            return;
        }

        appendToLog("Executing: " + command + "\n");
        appendToLog("----------------------------------------\n");

        QProcess *process = new QProcess(this);
        process->setProcessChannelMode(QProcess::MergedChannels);

        // If command contains sudo -S, write password to process
        if (command.contains("sudo -S") && !currentPassword.isEmpty()) {
            connect(process, &QProcess::started, this, [process, this]() {
                process->write(currentPassword.toUtf8() + "\n");
            });
        }

        connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
            QString output = process->readAllStandardOutput();
            appendToLog(output);
        });

        connect(process, &QProcess::readyReadStandardError, this, [this, process]() {
            QString error = process->readAllStandardError();
            appendToLog(error);
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                    if (exitStatus == QProcess::NormalExit) {
                        appendToLog("\nProcess finished with exit code: " + QString::number(exitCode) + "\n");
                    } else {
                        appendToLog("\nProcess crashed!\n");
                    }
                    appendToLog("----------------------------------------\n\n");
                    process->deleteLater();
                    currentPassword.clear(); // Clear password after use
                });

        process->start("bash", QStringList() << "-c" << command);
    }

    void lockKernel() {
        QString script = R"(
            #!/bin/bash

            if [ "$EUID" -ne 0 ]; then
              echo "Please run this script as root or using sudo."
              exit 1
            fi

            PACMAN_CONF="/etc/pacman.conf"
            cp "$PACMAN_CONF" "${PACMAN_CONF}.bak"

            if ! grep -q "IgnorePkg.*linux-zen" "$PACMAN_CONF"; then
              sed -i '/IgnorePkg/ s/$/ linux-zen/' "$PACMAN_CONF"
            fi

            if ! grep -q "NoUpgrade.*linux-zen" "$PACMAN_CONF"; then
              sed -i '/NoUpgrade/ s/$/ linux-zen/' "$PACMAN_CONF"
            fi

            if ! grep -q "IgnoreGroup.*linux-zen" "$PACMAN_CONF"; then
                sed -i '/IgnoreGroup/ s/$/ linux-zen/' "$PACMAN_CONF"
            fi

            echo "linux-zen kernel locked."
            )";

            runScriptWithLog(script);
    }

    void unlockKernel() {
        QString script = R"(
            #!/bin/bash

            if [ "$EUID" -ne 0 ]; then
              echo "Please run this script as root or using sudo."
              exit 1
            fi

            PACMAN_CONF="/etc/pacman.conf"
            cp "$PACMAN_CONF" "${PACMAN_CONF}.bak"

            sed -i '/IgnorePkg/ s/ linux-zen//' "$PACMAN_CONF"
            sed -i '/NoUpgrade/ s/ linux-zen//' "$PACMAN_CONF"
            sed -i '/IgnoreGroup/ s/ linux-zen//' "$PACMAN_CONF"

            echo "linux-zen kernel unlocked."
            )";

            runScriptWithLog(script);
    }

private:
    QTextEdit *outputLog;
    QString currentPassword;

    void addButton(QGridLayout *layout, int row, int col, const QString &text, const QString &command, bool requirePassword) {
        QPushButton *button = new QPushButton(text, this);
        button->setStyleSheet("font-size: 10pt; padding: 8px;");
        layout->addWidget(button, row, col);

        connect(button, &QPushButton::clicked, this, [this, command, requirePassword]() {
            executeCommand(command, requirePassword);
        });
    }

    bool checkPassword() {
        bool ok;
        QString password = QInputDialog::getText(this, "Authentication Required",
                                                 "Password:", QLineEdit::Password,
                                                 "", &ok);
        if (!ok || password.isEmpty()) {
            appendToLog("Authentication cancelled\n");
            return false;
        }
        currentPassword = password;
        return true;
    }

    void runScriptWithLog(const QString &script) {
        appendToLog("Executing kernel management script...\n");

        QProcess *process = new QProcess(this);
        process->setProcessChannelMode(QProcess::MergedChannels);

        if (!currentPassword.isEmpty()) {
            connect(process, &QProcess::started, this, [process, this]() {
                process->write(currentPassword.toUtf8() + "\n");
            });
        }

        connect(process, &QProcess::readyReadStandardOutput, this, [this, process]() {
            QString output = process->readAllStandardOutput();
            appendToLog(output);
        });

        connect(process, &QProcess::readyReadStandardError, this, [this, process]() {
            QString error = process->readAllStandardError();
            appendToLog(error);
        });

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                    if (exitStatus == QProcess::NormalExit) {
                        appendToLog("\nScript finished with exit code: " + QString::number(exitCode) + "\n");
                    } else {
                        appendToLog("\nScript execution failed!\n");
                    }
                    appendToLog("----------------------------------------\n\n");
                    process->deleteLater();
                    currentPassword.clear();
                });

        process->start("pkexec", QStringList() << "bash" << "-c" << script);
    }

    void appendToLog(const QString &text) {
        outputLog->moveCursor(QTextCursor::End);
        outputLog->insertPlainText(text);
        outputLog->verticalScrollBar()->setValue(outputLog->verticalScrollBar()->maximum());
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
