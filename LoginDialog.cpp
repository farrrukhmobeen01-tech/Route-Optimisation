#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), attemptsLeft(3) {
    setWindowTitle("Operator Authentication");
    setFixedSize(350, 450);

    // Modern QSS Styling
    setStyleSheet(R"(
        QDialog {
            background-color: #1e1e2e;
        }
        QLabel {
            color: #cdd6f4;
            font-family: 'Segoe UI', sans-serif;
        }
        QLineEdit {
            background-color: #313244;
            color: #cdd6f4;
            border: 2px solid #45475a;
            border-radius: 6px;
            padding: 10px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #89b4fa;
        }
        QPushButton {
            background-color: #89b4fa;
            color: #11111b;
            border: none;
            border-radius: 6px;
            padding: 12px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #b4befe;
        }
        QPushButton:disabled {
            background-color: #45475a;
            color: #a6adc8;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 50, 40, 50);

    QLabel *titleLabel = new QLabel("System Login", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #89b4fa;");
    titleLabel->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Operator Name");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);

    statusLabel = new QLabel("", this);
    statusLabel->setStyleSheet("color: #f38ba8; font-size: 12px;");
    statusLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addSpacing(10);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);
    layout->addStretch();

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
}

void LoginDialog::attemptLogin() {
    QString user = usernameInput->text().trimmed();
    QString pass = passwordInput->text().trimmed();

    // Verification logic
    if ((user == "farrukh" && pass == "033") || (user == "sharyar" && pass == "083")) {
        accept(); // Closes dialog and returns QDialog::Accepted
    } else {
        attemptsLeft--;
        if (attemptsLeft > 0) {
            statusLabel->setText(QString("Invalid credentials. %1 attempts left.").arg(attemptsLeft));
            passwordInput->clear();
        } else {
            statusLabel->setText("Maximum attempts reached. Access Locked.");
            usernameInput->setDisabled(true);
            passwordInput->setDisabled(true);
            loginButton->setDisabled(true);
        }
    }
}