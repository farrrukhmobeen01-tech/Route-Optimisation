#include "mainwindow.h"
#include <QHeaderView>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <QTime>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>
#include <fstream>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// High-Tech Color Palette
const QColor CYBER_BG(5, 5, 10);
const QColor GRID_LINE(17, 17, 26);
const QColor NEON_CYAN(0, 240, 255);
const QColor NEON_GREEN(0, 255, 102);
const QColor NEON_PINK(255, 0, 60);
const QColor NODE_DARK(13, 19, 33);
const QColor PURPLE_CUSTOM(167, 139, 250);

// ==========================================================
// FULL-SCREEN LOGIN DIALOG IMPLEMENTATION
// ==========================================================
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), attempts(0), progressValue(0), isAuthenticating(false) {
    setWindowTitle("System Authentication");

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    showFullScreen();

    setStyleSheet("QDialog { background-color: #020204; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // --- TOP BAR ---
    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addStretch();

    minBtn = new QPushButton("—", this);
    closeBtn = new QPushButton("✕", this);
    minBtn->setFixedSize(45, 45);
    closeBtn->setFixedSize(45, 45);

    minBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setCursor(Qt::PointingHandCursor);

    minBtn->setStyleSheet("QPushButton { background: transparent; color: #94A3B8; font-size: 20px; border: none; font-weight: bold; } QPushButton:hover { color: #00F0FF; background: rgba(0, 240, 255, 0.1); border-radius: 5px;}");
    closeBtn->setStyleSheet("QPushButton { background: transparent; color: #94A3B8; font-size: 20px; border: none; font-weight: bold; } QPushButton:hover { color: #FF003C; background: rgba(255, 0, 60, 0.1); border-radius: 5px;}");
    topBar->addWidget(minBtn);
    topBar->addWidget(closeBtn);
    mainLayout->addLayout(topBar);

    // --- CENTER SPLIT LOGIN BOX ---
    QHBoxLayout *centerLayout = new QHBoxLayout();

    loginBox = new QFrame(this);
    loginBox->setFixedSize(800, 480);

    loginBox->setStyleSheet(
        "QFrame#MainBox { background-color: #05050A; border: 2px solid #1E293B; border-radius: 12px; }"
        "QLineEdit { background-color: #0B1021; color: #00F0FF; border: 1px solid #1E293B; border-radius: 6px; padding: 15px; font-size: 14px; letter-spacing: 2px; }"
        "QLineEdit:focus { border: 1px solid #00F0FF; background-color: #0F172A; }"
        "QPushButton#LoginBtn { background: #0B1021; color: #00F0FF; border: 1px solid #00F0FF; padding: 12px; border-radius: 6px; font-weight: 900; font-size: 13px; letter-spacing: 1px; min-height: 45px; }"
        "QPushButton#LoginBtn:hover { background: rgba(0, 240, 255, 0.1); color: #00F0FF; }"
        "QProgressBar { border: 1px solid #1E293B; border-radius: 4px; background: #0B1021; text-align: center; color: transparent; height: 12px; }"
        "QProgressBar::chunk { background-color: #00FF66; width: 5px; margin: 1px; }"
        );
    loginBox->setObjectName("MainBox");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(80);
    shadow->setColor(QColor(0, 240, 255, 40));
    shadow->setOffset(0, 0);
    loginBox->setGraphicsEffect(shadow);

    QHBoxLayout *boxLayout = new QHBoxLayout(loginBox);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(0);

    // --- LEFT PANEL (VISUALS) ---
    QFrame *leftPanel = new QFrame(loginBox);
    leftPanel->setFixedWidth(300);
    leftPanel->setStyleSheet("background-color: #0B1021; border-right: 1px solid #1E293B; border-top-left-radius: 10px; border-bottom-left-radius: 10px;");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(20, 40, 20, 40);

    QLabel *sysIcon = new QLabel("⛬");
    sysIcon->setAlignment(Qt::AlignCenter);
    sysIcon->setStyleSheet("color: #00F0FF; font-size: 90px; background: transparent; border: none;");

    QLabel *sysTitle = new QLabel("GLOBAL\nLOGISTICS\nNETWORK");
    sysTitle->setAlignment(Qt::AlignCenter);
    sysTitle->setStyleSheet("color: #E2E8F0; font-size: 20px; font-weight: 900; letter-spacing: 4px; background: transparent; border: none; font-family: 'Segoe UI Black', Arial; line-height: 1.2;");

    leftLayout->addStretch();
    leftLayout->addWidget(sysIcon);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(sysTitle);
    leftLayout->addStretch();

    // --- RIGHT PANEL (FORM) ---
    QFrame *rightPanel = new QFrame(loginBox);
    rightPanel->setStyleSheet("background: transparent; border: none;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(50, 50, 50, 50);
    rightLayout->setSpacing(25);

    QLabel *brandLabel = new QLabel("BUROBRAIN N E X U S", rightPanel);
    brandLabel->setAlignment(Qt::AlignCenter);
    brandLabel->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: 900; letter-spacing: 6px; font-family: 'Segoe UI Black', Arial;");

    QLabel *subLabel = new QLabel("AUTHENTICATION REQUIRED", rightPanel);
    subLabel->setAlignment(Qt::AlignCenter);
    subLabel->setStyleSheet("color: #94A3B8; font-size: 12px; margin-bottom: 10px; letter-spacing: 3px; font-weight: bold;");

    userEdit = new QLineEdit(rightPanel);
    userEdit->setPlaceholderText("OPERATOR ID");

    passEdit = new QLineEdit(rightPanel);
    passEdit->setPlaceholderText("PASSCODE");
    passEdit->setEchoMode(QLineEdit::Password);

    statusLabel = new QLabel("", rightPanel);
    statusLabel->setStyleSheet("color: #FF003C; font-size: 13px; font-weight: bold; font-family: monospace;");
    statusLabel->setAlignment(Qt::AlignCenter);

    loginBtn = new QPushButton("INITIALIZE UPLINK", rightPanel);
    loginBtn->setObjectName("LoginBtn");
    loginBtn->setCursor(Qt::PointingHandCursor);

    progressBar = new QProgressBar(rightPanel);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->hide();

    rightLayout->addStretch();
    rightLayout->addWidget(brandLabel);
    rightLayout->addWidget(subLabel);
    rightLayout->addWidget(userEdit);
    rightLayout->addWidget(passEdit);
    rightLayout->addWidget(progressBar);
    rightLayout->addWidget(statusLabel);
    rightLayout->addWidget(loginBtn);
    rightLayout->addStretch();

    boxLayout->addWidget(leftPanel);
    boxLayout->addWidget(rightPanel);

    centerLayout->addStretch();
    centerLayout->addWidget(loginBox);
    centerLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &LoginDialog::updateProgress);
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
}

void LoginDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        attemptLogin();
        return;
    }
    QDialog::keyPressEvent(event);
}

void LoginDialog::attemptLogin() {
    if (isAuthenticating) return;

    QString user = userEdit->text().trimmed().toLower();
    QString pass = passEdit->text().trimmed();

    if ((user == "farrukh" && pass == "033") || (user == "sharyar" && pass == "083")) {
        isAuthenticating = true;
        userEdit->hide();
        passEdit->hide();
        loginBtn->hide();

        progressBar->show();
        statusLabel->setStyleSheet("color: #00F0FF; font-size: 14px; font-weight: bold; font-family: monospace;");
        statusLabel->setText("AUTHENTICATING CREDENTIALS...");

        progressValue = 0;
        loadingTimer->start(100);
    } else {
        attempts++;
        int remaining = 3 - attempts;

        if (remaining <= 0) {
            std::exit(0);
        } else {
            statusLabel->setText(QString("ACCESS DENIED. %1 ATTEMPTS REMAINING.").arg(remaining));
            passEdit->clear();
            passEdit->setFocus();
        }
    }
}

void LoginDialog::updateProgress() {
    progressValue += 4;
    progressBar->setValue(progressValue);

    if (progressValue > 25 && progressValue <= 50) {
        statusLabel->setText("DECRYPTING PACKETS...");
    } else if (progressValue > 50 && progressValue <= 80) {
        statusLabel->setText("BYPASSING FIREWALL...");
    } else if (progressValue > 80 && progressValue < 100) {
        statusLabel->setStyleSheet("color: #00FF66; font-size: 14px; font-weight: bold; font-family: monospace;");
        statusLabel->setText("UPLINK ESTABLISHED.");
    } else if (progressValue >= 100) {
        loadingTimer->stop();
        accept();
    }
}

// ==========================================================
// MAIN WINDOW IMPLEMENTATION
// ==========================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), master_mat(nullptr), master_list(nullptr), currentGraphSize(0), isGraphLoaded(false)
{
    LoginDialog firewallDialog(this);
    if (firewallDialog.exec() != QDialog::Accepted) {
        std::exit(0);
    }

    setupUI();
    setupPremiumTheme();
    createWelcomePage();
}

MainWindow::~MainWindow() {
    if (master_mat) delete master_mat;
    if (master_list) delete master_list;
}

// ==========================================================
// 0. HIGH-TECH CINEMATIC SPLASH SCREEN
// ==========================================================
void MainWindow::createWelcomePage()
{
    QPushButton *welcomeBtn = new QPushButton();
    welcomeBtn->setObjectName("welcomePageBtn");
    welcomePage = welcomeBtn;

    welcomeBtn->setCursor(Qt::PointingHandCursor);

    connect(welcomeBtn, &QPushButton::clicked, this, &MainWindow::startApplication);

    welcomePage->setStyleSheet(
        "#welcomePageBtn { "
        "background: qradialgradient(cx: 0.5, cy: 0.5, radius: 1.0, fx: 0.5, fy: 0.5, stop: 0 #0B1021, stop: 1 #020204); "
        "border: none; outline: none; "
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(welcomePage);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);

    QLabel *gpsIcon = new QLabel("⏣");
    gpsIcon->setStyleSheet("color: #00F0FF; font-size: 120px; background: transparent; font-weight: 100;");
    gpsIcon->setAlignment(Qt::AlignCenter);

    QGraphicsDropShadowEffect *iconGlow = new QGraphicsDropShadowEffect();
    iconGlow->setBlurRadius(50); iconGlow->setColor(NEON_CYAN); iconGlow->setOffset(0,0);
    gpsIcon->setGraphicsEffect(iconGlow);

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    gpsIcon->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity", this);
    a->setDuration(2000);
    a->setStartValue(0.4);
    a->setEndValue(1.0);
    a->setLoopCount(-1);
    a->setEasingCurve(QEasingCurve::SineCurve);
    a->start();

    QLabel *appName = new QLabel("BUROBRAIN N E X U S");
    appName->setStyleSheet("color: #FFFFFF; font-size: 60px; font-weight: 900; letter-spacing: 15px; font-family: 'Segoe UI Black', Arial; background: transparent;");
    appName->setAlignment(Qt::AlignCenter);

    QLabel *tagline = new QLabel("A D V A N C E D   L O G I S T I C S   M A I N F R A M E");
    tagline->setStyleSheet("color: #00F0FF; font-size: 16px; font-weight: 800; letter-spacing: 8px; background: transparent;");
    tagline->setAlignment(Qt::AlignCenter);

    QFrame *creditFrame = new QFrame();
    creditFrame->setMinimumSize(600, 130);
    creditFrame->setStyleSheet("background: rgba(0, 240, 255, 0.05); border: 1px solid rgba(0, 240, 255, 0.3); border-radius: 12px; border-left: 5px solid #00F0FF;");

    QVBoxLayout *creditLayout = new QVBoxLayout(creditFrame);
    creditLayout->setContentsMargins(30, 25, 30, 25);
    creditLayout->setSpacing(5);

    QLabel *creditsRole = new QLabel("SYSTEM ARCHITECTS");
    creditsRole->setStyleSheet("color: #64748B; font-size: 14px; font-weight: bold; background: transparent; letter-spacing: 6px; border: none;");
    creditsRole->setAlignment(Qt::AlignCenter);

    QLabel *creditsNames = new QLabel("FARRUKH MUBEEN & SHARYAR ALI");
    creditsNames->setStyleSheet("color: #FFFFFF; font-size: 24px; font-weight: 900; background: transparent; letter-spacing: 4px; font-family: 'Segoe UI', Arial; border: none;");
    creditsNames->setAlignment(Qt::AlignCenter);

    creditLayout->addWidget(creditsRole);
    creditLayout->addWidget(creditsNames);

    QLabel *instruction = new QLabel("[ INITIALIZE SEQUENCE: PRESS ENTER OR CLICK ANYWHERE ]");
    instruction->setStyleSheet("color: #00FF66; font-size: 14px; margin-top: 50px; font-weight: 900; background: transparent; letter-spacing: 4px;");
    instruction->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *blinkEff = new QGraphicsOpacityEffect(this);
    instruction->setGraphicsEffect(blinkEff);
    QPropertyAnimation *blink = new QPropertyAnimation(blinkEff, "opacity", this);
    blink->setDuration(800); blink->setStartValue(0.1); blink->setEndValue(1.0);
    blink->setLoopCount(-1); blink->setEasingCurve(QEasingCurve::InOutQuad);
    blink->start();

    layout->addStretch();
    layout->addWidget(gpsIcon);
    layout->addWidget(appName);
    layout->addWidget(tagline);
    layout->addSpacing(40);
    layout->addWidget(creditFrame, 0, Qt::AlignHCenter);
    layout->addWidget(instruction);
    layout->addStretch();

    rootWorkspace->insertWidget(0, welcomePage);
    rootWorkspace->setCurrentIndex(0);
}

void MainWindow::startApplication() {
    rootWorkspace->setCurrentIndex(1);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (rootWorkspace->currentIndex() == 0) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Space) {
            startApplication();
        }
    }
}

// ==========================================================
// 1. CORE UI SETUP
// ==========================================================
void MainWindow::setupUI()
{
    setWindowTitle("BuroBrain Terminal - Logistics");
    resize(1200, 750);

    rootWorkspace = new QStackedWidget(this);
    setCentralWidget(rootWorkspace);

    applicationPage = new QWidget();
    applicationPage->setObjectName("applicationPage");

    QVBoxLayout *masterLayout = new QVBoxLayout(applicationPage);
    masterLayout->setContentsMargins(15, 15, 15, 15);
    masterLayout->setSpacing(10);

    topBanner = new QLabel("✦ BUROBRAIN LOGISTICS NETWORK (TSP) ✦");
    topBanner->setObjectName("topBanner");
    topBanner->setAlignment(Qt::AlignCenter);
    masterLayout->addWidget(topBanner);

    QHBoxLayout *workspaceLayout = new QHBoxLayout();
    workspaceLayout->setSpacing(15);

    QScrollArea *sidebarScroll = new QScrollArea();
    sidebarScroll->setObjectName("scrollArea");
    sidebarScroll->setFixedWidth(340);
    sidebarScroll->setWidgetResizable(true);
    sidebarScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QFrame *commandDeck = new QFrame();
    commandDeck->setObjectName("glassPanel");
    QVBoxLayout *cmdLayout = new QVBoxLayout(commandDeck);
    cmdLayout->setSpacing(10);
    cmdLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *lblConfig = new QLabel("1. TOPOLOGY DATA");
    lblConfig->setObjectName("sectionHeader");
    QPushButton *btnLoad = new QPushButton("📂 Initialize Map Payload");
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadFile);

    QPushButton *btnViewMem = new QPushButton("💾 View Matrix & List");
    connect(btnViewMem, &QPushButton::clicked, this, &MainWindow::showMemoryAnalysis);

    citySelector = new QComboBox();
    citySelector->addItem("Select Origin Hub...");

    QLabel *lblExec = new QLabel("2. OPTIMIZATION ENGINES");
    lblExec->setObjectName("sectionHeader");
    QPushButton *btnGreedy = new QPushButton("⚡ Run Greedy Engine");
    connect(btnGreedy, &QPushButton::clicked, this, &MainWindow::runGreedy);
    QPushButton *btnBrute = new QPushButton("🔍 Run Brute Force");
    connect(btnBrute, &QPushButton::clicked, this, &MainWindow::runBruteForce);
    QPushButton *btnCustom = new QPushButton("🎲 Run Custom (Random)");
    connect(btnCustom, &QPushButton::clicked, this, &MainWindow::runCustom);
    QPushButton *btnCompare = new QPushButton("📊 Benchmark Algorithms");
    connect(btnCompare, &QPushButton::clicked, this, &MainWindow::runComparison);

    QLabel *lblScen = new QLabel("3. FIELD SCENARIOS");
    lblScen->setObjectName("sectionHeader");
    scenarioSelector = new QComboBox();
    scenarioSelector->addItems({"🚚 Fleet Delivery Routing", "📍 Satellite GPS Navigation", "🚁 UAV Patrol Path"});
    QPushButton *btnScenario = new QPushButton("🚀 Deploy Scenario");
    connect(btnScenario, &QPushButton::clicked, this, &MainWindow::simulateScenario);

    QLabel *lblViews = new QLabel("4. DISPLAY TOGGLES");
    lblViews->setObjectName("sectionHeader");
    QPushButton *btnViewMap = new QPushButton("🗺️ Return to Live Map");
    connect(btnViewMap, &QPushButton::clicked, this, &MainWindow::showMap);

    btnLoad->setCursor(Qt::PointingHandCursor);
    btnViewMem->setCursor(Qt::PointingHandCursor);
    citySelector->setCursor(Qt::PointingHandCursor);
    btnGreedy->setCursor(Qt::PointingHandCursor);
    btnBrute->setCursor(Qt::PointingHandCursor);
    btnCustom->setCursor(Qt::PointingHandCursor);
    btnCompare->setCursor(Qt::PointingHandCursor);
    scenarioSelector->setCursor(Qt::PointingHandCursor);
    btnScenario->setCursor(Qt::PointingHandCursor);
    btnViewMap->setCursor(Qt::PointingHandCursor);

    cmdLayout->addWidget(lblConfig);
    cmdLayout->addWidget(btnLoad);
    cmdLayout->addWidget(btnViewMem);
    cmdLayout->addWidget(citySelector);
    cmdLayout->addSpacing(5);
    cmdLayout->addWidget(lblExec);
    cmdLayout->addWidget(btnGreedy);
    cmdLayout->addWidget(btnBrute);
    cmdLayout->addWidget(btnCustom);
    cmdLayout->addWidget(btnCompare);
    cmdLayout->addSpacing(5);
    cmdLayout->addWidget(lblScen);
    cmdLayout->addWidget(scenarioSelector);
    cmdLayout->addWidget(btnScenario);
    cmdLayout->addSpacing(5);
    cmdLayout->addWidget(lblViews);
    cmdLayout->addWidget(btnViewMap);

    cmdLayout->addStretch();

    QLabel *lblSys = new QLabel("5. SYSTEM");
    lblSys->setObjectName("sectionHeader");
    QPushButton *btnLogout = new QPushButton("🔴 Terminate Uplink (Logout)");
    btnLogout->setStyleSheet(
        "QPushButton { border-left: 3px solid #FF003C; color: #94A3B8; } "
        "QPushButton:hover { border: 1px solid #FF003C; border-left: 6px solid #FF003C; background-color: rgba(255,0,60,0.1); color: #FF003C; }"
        );
    btnLogout->setCursor(Qt::PointingHandCursor);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::logout);

    cmdLayout->addWidget(lblSys);
    cmdLayout->addWidget(btnLogout);

    sidebarScroll->setWidget(commandDeck);
    workspaceLayout->addWidget(sidebarScroll);

    mainWorkspace = new QStackedWidget();
    mainWorkspace->setObjectName("mainWorkspace");

    QFrame *mapContainer = new QFrame();
    mapContainer->setObjectName("mapPanel");
    QVBoxLayout *innerMapLayout = new QVBoxLayout(mapContainer);
    innerMapLayout->setContentsMargins(5, 5, 5, 5);
    innerMapLayout->setSpacing(5);

    mapScene = new QGraphicsScene(this);
    mapScene->setSceneRect(0, 0, 750, 460);

    mapView = new QGraphicsView(mapScene);
    mapView->setRenderHint(QPainter::Antialiasing);
    mapView->setStyleSheet("border: none; background-color: #05050A;");
    mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    resultPanel = new QTextBrowser();
    resultPanel->setObjectName("resultPanel");
    resultPanel->setFixedHeight(130);
    resultPanel->setHtml("<div style='color:#00F0FF; font-size:14px; margin-top:38px; text-align:center; font-family: \"Segoe UI\", Arial, sans-serif; letter-spacing: 4px; font-weight: bold;'>[ SYSTEM IDLE — AWAITING COORDINATES ]</div>");

    innerMapLayout->addWidget(mapView, 1);
    innerMapLayout->addWidget(resultPanel);
    mainWorkspace->addWidget(mapContainer);

    QFrame *memFrame = new QFrame();
    memFrame->setObjectName("glassPanel");
    QVBoxLayout *memLayout = new QVBoxLayout(memFrame);
    memLayout->setContentsMargins(20,20,20,20);
    QLabel *memTitle = new QLabel("CORE MEMORY DUMP");
    memTitle->setObjectName("viewTitle");
    matrixTable = new QTableWidget();
    matrixTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    listBrowser = new QTextBrowser();
    listBrowser->setObjectName("listBrowser");
    memLayout->addWidget(memTitle);
    memLayout->addWidget(new QLabel("STATIC ADJACENCY MATRIX [ O(V²) ]"));
    memLayout->addWidget(matrixTable, 1);
    memLayout->addWidget(new QLabel("DYNAMIC ADJACENCY LIST [ O(V+E) ]"));
    memLayout->addWidget(listBrowser, 1);
    mainWorkspace->addWidget(memFrame);

    workspaceLayout->addWidget(mainWorkspace, 1);
    masterLayout->addLayout(workspaceLayout);
    rootWorkspace->insertWidget(1, applicationPage);
}

void MainWindow::setupPremiumTheme()
{
    this->setStyleSheet(R"(
        #applicationPage { background-color: #020204; font-family: 'Segoe UI', Arial, sans-serif; }

        #topBanner {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #05050A, stop:0.5 #0F172A, stop:1 #05050A);
            color: #00F0FF;
            font-size: 20px;
            font-weight: 900;
            padding: 15px;
            border-bottom: 2px solid #00F0FF;
            letter-spacing: 5px;
        }

        #scrollArea { background: transparent; border: none; }

        #glassPanel {
            background-color: rgba(10, 15, 30, 0.8);
            border-radius: 8px;
            border: 1px solid #1E293B;
            border-top: 2px solid #00F0FF;
        }

        #sectionHeader {
            color: #475569;
            font-size: 11px;
            font-weight: 900;
            letter-spacing: 2px;
            margin-top: 5px;
            font-family: monospace;
        }

        #viewTitle {
            color: #00F0FF;
            font-size: 22px;
            font-weight: bold;
            border-bottom: 1px solid #1E293B;
            padding-bottom: 10px;
            letter-spacing: 2px;
        }

        QLabel { color: #E2E8F0; font-weight: bold; }

        QPushButton {
            background: #0B1021;
            color: #94A3B8;
            border-radius: 4px;
            padding: 12px;
            font-size: 13px;
            font-weight: 800;
            text-align: left;
            border: 1px solid #1E293B;
            border-left: 3px solid #3B82F6;
        }
        QPushButton:hover {
            background: #0F172A;
            color: #00F0FF;
            border: 1px solid #00F0FF;
            border-left: 6px solid #00F0FF;
        }
        QPushButton:pressed { background: #00F0FF; color: #000000; }

        QComboBox {
            padding: 10px;
            background-color: #0B1021;
            color: #00F0FF;
            border: 1px solid #1E293B;
            border-radius: 4px;
            font-size: 12px;
            font-weight: bold;
        }

        #mapPanel {
            background-color: #05050A;
            border-radius: 8px;
            border: 1px solid #1E293B;
        }

        #resultPanel {
            background-color: rgba(10, 15, 30, 0.8);
            border: 2px solid #00F0FF;
            border-radius: 8px;
            padding: 15px;
            color: white;
        }

        QTableWidget {
            background-color: #05050A;
            color: #00F0FF;
            border: 1px solid #1E293B;
            gridline-color: #1E293B;
            font-size: 14px;
            font-family: monospace;
        }
        QHeaderView::section {
            background-color: #0B1021;
            color: #94A3B8;
            font-weight: bold;
            padding: 10px;
            font-size: 12px;
            border: none;
            border-bottom: 1px solid #1E293B;
        }

        #listBrowser {
            background-color: #05050A;
            color: #00FF66;
            font-family: 'Consolas', monospace;
            font-size: 15px;
            border: 1px solid #1E293B;
            padding: 15px;
        }

        QScrollBar:vertical { background: #020204; width: 6px; }
        QScrollBar::handle:vertical { background: #3B82F6; border-radius: 3px; }
    )");
}

// ==========================================================
// 2. GRAPHICS ENGINE
// ==========================================================
void MainWindow::delayAnimation(int milliseconds) {
    QTime dieTime= QTime::currentTime().addMSecs(milliseconds);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::drawBackgroundGraph() {
    mapScene->clear();

    QPen gridPen(GRID_LINE, 1, Qt::SolidLine);
    for (int x = 0; x <= 800; x += 40) mapScene->addLine(x, 0, x, 500, gridPen);
    for (int y = 0; y <= 500; y += 40) mapScene->addLine(0, y, 800, y, gridPen);

    if (!isGraphLoaded) return;

    QPen bgEdgePen(QColor(30, 41, 59, 150), 2);

    for (int i = 0; i < currentGraphSize; i++) {
        for (int j = i + 1; j < currentGraphSize; j++) {
            int w = master_mat->g[i][j];
            if (w > 0) {
                QPointF p1 = cityPositions[i];
                QPointF p2 = cityPositions[j];
                mapScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), bgEdgePen);

                double midX = (p1.x() + p2.x()) / 2.0;
                double midY = (p1.y() + p2.y()) / 2.0;

                QGraphicsRectItem *badge = mapScene->addRect(midX-12, midY-10, 24, 20, QPen(Qt::NoPen), QBrush(NODE_DARK));
                badge->setOpacity(0.8);

                QGraphicsTextItem *weightText = mapScene->addText(QString::number(w));
                weightText->setPos(midX - 12, midY - 12);
                weightText->setDefaultTextColor(QColor(100, 116, 139));
                weightText->setFont(QFont("Consolas", 9, QFont::Bold));
            }
        }
    }
}

void MainWindow::drawCityNodes(int startNode) {
    cityPositions.clear();
    if (currentGraphSize == 0) return;

    int radius = 170;
    int centerX = 375;
    int centerY = 210;

    for (int i = 0; i < currentGraphSize; i++) {
        double angle = (2.0 * M_PI * i) / currentGraphSize - (M_PI / 2.0);
        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);
        cityPositions.push_back(QPointF(x, y));
    }

    drawBackgroundGraph();

    for (int i = 0; i < currentGraphSize; i++) {
        double x = cityPositions[i].x();
        double y = cityPositions[i].y();

        if (i == startNode) {
            mapScene->addEllipse(x-20, y-20, 40, 40, QPen(NEON_PINK, 2, Qt::DashLine), Qt::NoBrush);
            QGraphicsEllipseItem *core = mapScene->addEllipse(x-12, y-12, 24, 24, Qt::NoPen, QBrush(NEON_PINK));

            QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect();
            glow->setBlurRadius(30); glow->setColor(NEON_PINK); glow->setOffset(0, 0);
            core->setGraphicsEffect(glow);

            QGraphicsTextItem *icon = mapScene->addText("📍");
            icon->setDefaultTextColor(Qt::white); icon->setFont(QFont("Arial", 14));
            icon->setPos(x - 14, y - 20);

            QString fullName = QString::fromStdString(getCityName(i)).toUpper();
            QGraphicsTextItem *label = mapScene->addText(fullName);
            label->setDefaultTextColor(NEON_PINK); label->setFont(QFont("Segoe UI", 11, QFont::Bold));
            label->setPos(x - (label->boundingRect().width()/2), y + 20);
        } else {
            int nodeSize = 24;
            int offset = nodeSize / 2;

            QGraphicsEllipseItem *circle = mapScene->addEllipse(x - offset, y - offset, nodeSize, nodeSize, QPen(NEON_CYAN, 2), QBrush(NODE_DARK));

            QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
            shadow->setBlurRadius(15); shadow->setColor(NEON_CYAN); shadow->setOffset(0,0);
            circle->setGraphicsEffect(shadow);

            QString fullName = QString::fromStdString(getCityName(i)).toUpper();
            QGraphicsTextItem *text = mapScene->addText(fullName);
            text->setDefaultTextColor(Qt::white);
            text->setFont(QFont("Segoe UI", 10, QFont::Bold));

            text->setPos(x - (text->boundingRect().width() / 2), y + 12);
        }
    }
}

// ==========================================================
// 3. RESULTS & ROUTING
// ==========================================================
void MainWindow::updateResultPanel(const RouteResult& res) {
    if (res.cost >= INF || res.pathLen == 0) {
        resultPanel->setHtml("<div style='color:#FF003C; font-size:16px; margin-top:35px; text-align:center;'><b>[ CRITICAL ERROR ] NETWORK DISCONNECTED.</b></div>");
        return;
    }

    QString html = "<table width='100%' cellpadding='5'><tr>";
    html += "<td width='35%' valign='top' style='border-right: 1px solid #1E293B;'>";
    html += "<b style='color:#00F0FF; font-size:16px; letter-spacing: 1px;'>PROTOCOL: " + QString::fromStdString(res.algoName).toUpper() + "</b><br>";
    html += "<span style='color:#94A3B8; font-size:13px;'>OPTIMIZED DISTANCE:</span> <b style='color:#00FF66; font-size: 15px;'>" + QString::number(res.cost) + " KM</b><br>";
    html += "<span style='color:#94A3B8; font-size:13px;'>COMPUTATION TIME:</span> <b style='color:#A78BFA; font-size: 15px;'>" + QString::number(res.timeTaken) + " µS</b></td>";

    html += "<td width='65%' valign='top' style='padding-left: 20px;'>";
    html += "<b style='color:#475569; font-size:12px; letter-spacing: 1px;'>VERIFIED ROUTING SEQUENCE</b><br>";
    html += "<span style='color:#E2E8F0; font-size:14px; font-family: monospace; line-height: 1.8;'>";

    for (int i = 0; i < res.pathLen; i++) {
        if (i == 0 || i == res.pathLen - 1) html += "<span style='color:#FF003C;'>[" + QString::fromStdString(getCityName(res.path[i])).toUpper() + "]</span>";
        else html += QString::fromStdString(getCityName(res.path[i])).toUpper();

        if (i < res.pathLen - 1) html += "<span style='color:#00F0FF;'> ⇢ </span>";
        if (i > 0 && i % 4 == 0 && i != res.pathLen -1) html += "<br>";
    }
    html += "</span></td></tr></table>";

    resultPanel->setHtml(html);
}

void MainWindow::drawRoute(const RouteResult& res, QColor routeColor) {
    mainWorkspace->setCurrentIndex(0);
    drawCityNodes(res.pathLen > 0 ? res.path[0] : 0);

    if (res.cost >= INF || res.pathLen == 0) {
        updateResultPanel(res);
        return;
    }

    QPen coreLaser(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen neonGlow(routeColor, 12, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    for (int i = 0; i < res.pathLen - 1; i++) {
        QPointF p1 = cityPositions[res.path[i]];
        QPointF p2 = cityPositions[res.path[i+1]];

        QGraphicsLineItem *glow = mapScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), neonGlow);
        glow->setOpacity(0.6);
        mapScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), coreLaser);

        delayAnimation(300);
    }

    updateResultPanel(res);
}

void MainWindow::generateMemoryView() {
    if (!isGraphLoaded) return;

    matrixTable->clear();
    matrixTable->setRowCount(currentGraphSize);
    matrixTable->setColumnCount(currentGraphSize);

    QStringList headers;
    for (int i = 0; i < currentGraphSize; i++) {
        headers << QString::fromStdString(getCityName(i)).left(3).toUpper();
    }
    matrixTable->setHorizontalHeaderLabels(headers);
    matrixTable->setVerticalHeaderLabels(headers);

    for (int i = 0; i < currentGraphSize; i++) {
        for (int j = 0; j < currentGraphSize; j++) {
            int w = master_mat->g[i][j];
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(w));
            item->setTextAlignment(Qt::AlignCenter);
            if (w == 0) item->setForeground(QBrush(QColor(30, 41, 59)));
            else item->setForeground(QBrush(NEON_CYAN));
            matrixTable->setItem(i, j, item);
        }
    }

    QString html = "<div style='line-height: 2.0;'>";
    for (int i = 0; i < currentGraphSize; i++) {
        html += "<span style='color: #05050A; background-color: #00F0FF; padding: 2px 6px; font-weight:bold;'> 0x" + QString::number(i) + " </span> ";
        html += "<span style='color: #94A3B8;'>:: " + QString::fromStdString(getCityName(i)).left(4).toUpper() + " </span>";

        NODEPTR ptr = master_list->adjList[i];
        while (ptr != nullptr) {
            html += "<span style='color: #3B82F6;'> ⇢ </span>";
            html += "<span style='color: #E2E8F0;'>[ " + QString::fromStdString(getCityName(ptr->adj)).left(3).toUpper() + " : <b style='color:#00FF66;'>" + QString::number(ptr->weight) + "</b> ]</span>";
            ptr = ptr->next;
        }
        html += "<br>";
    }
    html += "</div>";
    listBrowser->setHtml(html);
}

// ==========================================================
// SYSTEM OPERATION SLOTS
// ==========================================================
void MainWindow::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "MOUNT DATABANK", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    std::ifstream inFile(fileName.toStdString().c_str());
    if (!inFile) { QMessageBox::critical(this, "FATAL", "DATA STREAM CORRUPTED."); return; }

    inFile >> currentGraphSize;
    if (currentGraphSize <= 1) { QMessageBox::warning(this, "ABORT", "INSUFFICIENT NODES FOR ROUTING."); inFile.close(); return; }
    if (currentGraphSize > MAX_CITIES) currentGraphSize = MAX_CITIES;

    if (master_mat) delete master_mat;
    if (master_list) delete master_list;

    master_mat = new StaticGraph(currentGraphSize);
    master_list = new DynamicGraph(currentGraphSize);

    for (int i = 0; i < currentGraphSize; i++) {
        for (int j = 0; j < currentGraphSize; j++) {
            int w; inFile >> w;
            master_mat->addEdge(i, j, w);
            master_list->addEdge(i, j, w);
        }
    }
    inFile.close(); isGraphLoaded = true;

    citySelector->clear();
    for (int i = 0; i < currentGraphSize; i++) citySelector->addItem(QString::fromStdString(getCityName(i)).toUpper());

    generateMemoryView();
    showMap();
    resultPanel->setHtml("<div style='color:#00FF66; font-size:16px; margin-top:35px; text-align:center;'><b>[ UPLINK ESTABLISHED ] TOPOLOGY LOADED SECURELY.</b></div>");
}

void MainWindow::showMap() { mainWorkspace->setCurrentIndex(0); if(isGraphLoaded) drawCityNodes(citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0); }
void MainWindow::showMemoryAnalysis() { mainWorkspace->setCurrentIndex(1); }

void MainWindow::logout() {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("TERMINATE UPLINK");
    msgBox.setText("Are you sure you want to logout and close the current session?");
    msgBox.setStyleSheet("QLabel{color:#FFFFFF; font-size: 14px;} QPushButton{background:#0B1021; color:#00F0FF; padding:8px 15px; border:1px solid #00F0FF;} QPushButton:hover{background:#0F172A;} QMessageBox{background:#05050A;} ");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        this->hide();

        isGraphLoaded = false;
        currentGraphSize = 0;
        if (master_mat) { delete master_mat; master_mat = nullptr; }
        if (master_list) { delete master_list; master_list = nullptr; }
        mapScene->clear();
        citySelector->clear();
        citySelector->addItem("Select Origin Hub...");

        resultPanel->setHtml("<div style='color:#00F0FF; font-size:14px; margin-top:38px; text-align:center; font-family: \"Segoe UI\", Arial, sans-serif; letter-spacing: 4px; font-weight: bold;'>[ SYSTEM IDLE — AWAITING COORDINATES ]</div>");

        matrixTable->clear();
        matrixTable->setRowCount(0);
        matrixTable->setColumnCount(0);
        listBrowser->clear();

        LoginDialog firewallDialog(this);
        if (firewallDialog.exec() != QDialog::Accepted) {
            std::exit(0);
        }

        rootWorkspace->setCurrentIndex(0);
        this->showMaximized();
    }
}

void MainWindow::runGreedy() {
    if (!isGraphLoaded) return;
    int start = citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0;
    drawRoute(Greedy_Matrix(*master_mat, start), NEON_CYAN);
}

void MainWindow::runBruteForce() {
    if (!isGraphLoaded) return;
    int start = citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0;
    if (currentGraphSize > 11) { QMessageBox::warning(this, "OVERLOAD", "COMPLEXITY O(N!) WILL CAUSE SYSTEM FAILURE ON >11 NODES."); return; }
    drawRoute(BruteForce_Matrix(*master_mat, start), NEON_PINK);
}

void MainWindow::runCustom() {
    if (!isGraphLoaded) return;
    int start = citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0;
    drawRoute(RandomSearch_Matrix(*master_mat, start), PURPLE_CUSTOM);
}

void MainWindow::runComparison() {
    if (!isGraphLoaded) return;
    int start = citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0;

    RouteResult bfm = BruteForce_Matrix(*master_mat, start);
    RouteResult grm = Greedy_Matrix(*master_mat, start);
    RouteResult rsm = RandomSearch_Matrix(*master_mat, start);

    mainWorkspace->setCurrentIndex(0); drawCityNodes(start);

    QString html = "<b style='color:#00F0FF; font-size:16px;'>METRIC BENCHMARK OUTPUT:</b><br>";
    html += "<table width='100%' style='color:#E2E8F0; font-size:14px; margin-top:5px;' cellpadding='3'>";
    html += "<tr style='border-bottom: 1px solid #1E293B;'><th align='left' style='color:#94A3B8;'>ENGINE</th><th style='color:#94A3B8;'>COST</th><th style='color:#94A3B8;'>CYCLES</th></tr>";

    auto addRow = [&](RouteResult r) {
        html += "<tr><td>" + QString::fromStdString(r.algoName).toUpper() + "</td>";
        html += "<td align='center'><b style='color:#00FF66;'>" + (r.cost >= INF ? "FAIL" : QString::number(r.cost) + " KM") + "</b></td>";
        html += "<td align='center'><b style='color:#A78BFA;'>" + QString::number(r.timeTaken) + " µS</b></td></tr>";
    };

    addRow(grm); addRow(bfm); addRow(rsm);
    html += "</table>"; resultPanel->setHtml(html);
}

void MainWindow::simulateScenario() {
    if (!isGraphLoaded) return;
    int startCity = citySelector->currentIndex() >= 0 ? citySelector->currentIndex() : 0;
    int mode = scenarioSelector->currentIndex();

    if (mode == 0) {
        // --- SCENARIO 1: FLEET DELIVERY ---
        RouteResult res = Greedy_Matrix(*master_mat, startCity);
        res.algoName = "FLEET Delivery Protocol";

        drawRoute(res, NEON_GREEN);

        if (res.cost < INF && res.pathLen > 0) {
            double fuelRequired = res.cost / 10.0;

            QString html = "<table width='100%' cellpadding='5'><tr>";
            html += "<td width='40%' valign='top' style='border-right: 1px solid #1E293B;'>";
            html += "<b style='color:#00FF66; font-size:16px; letter-spacing: 1px;'>[ FLEET LOGISTICS ]</b><br><br>";
            html += "<span style='color:#94A3B8; font-size:13px;'>TOTAL DISTANCE:</span> <b style='color:#FFFFFF; font-size: 15px;'>" + QString::number(res.cost) + " KM</b><br>";
            html += "<span style='color:#94A3B8; font-size:13px;'>EST. FUEL REQ:</span> <b style='color:#00F0FF; font-size: 15px;'>" + QString::number(fuelRequired, 'f', 1) + " LITERS</b><br>";
            html += "<span style='color:#475569; font-size:11px;'>(Based on 10 km/L efficiency)</span></td>";

            html += "<td width='60%' valign='top' style='padding-left: 20px;'>";
            html += "<b style='color:#475569; font-size:12px; letter-spacing: 1px;'>DELIVERY MANIFEST</b><br>";
            html += "<span style='color:#E2E8F0; font-size:13px; font-family: monospace;'>";
            for (int i = 0; i < res.pathLen - 1; i++) {
                html += "[Stop " + QString::number(i + 1) + "] Deliver to <span style='color:#00FF66;'>" + QString::fromStdString(getCityName(res.path[i + 1])).toUpper() + "</span><br>";
            }
            html += "</span></td></tr></table>";
            resultPanel->setHtml(html);
        }
    }
    else if (mode == 1) {
        // --- SCENARIO 2: SATELLITE GPS ---
        QStringList items;
        for (int i = 0; i < currentGraphSize; i++) {
            items << QString::fromStdString(getCityName(i)).toUpper();
        }

        bool ok;
        QString dest = QInputDialog::getItem(this, "GPS", "LOCK DESTINATION COORDINATES:", items, 0, false, &ok);

        if (ok && !dest.isEmpty()) {
            int endCity = items.indexOf(dest);
            int dist = master_mat->g[startCity][endCity];

            if (dist > 0) {
                mainWorkspace->setCurrentIndex(0);
                drawCityNodes(startCity);
                mapScene->addLine(cityPositions[startCity].x(), cityPositions[startCity].y(), cityPositions[endCity].x(), cityPositions[endCity].y(), QPen(NEON_PINK, 5, Qt::DashLine));

                int hours = dist / 80;
                int minutes = (dist % 80) * 60 / 80;

                QString html = "<div style='padding: 5px;'><b style='color:#FF003C; font-size:16px; letter-spacing: 2px;'>[ ORBITAL SATELLITE LOCK ]</b><br><br>";
                html += "<span style='color:#94A3B8; font-size:14px;'>DIRECTIONS: </span><span style='color:#E2E8F0; font-size:14px;'>Drive straight from " + QString::fromStdString(getCityName(startCity)).toUpper() + " to " + dest + ".</span><br>";
                html += "<span style='color:#94A3B8; font-size:14px;'>DISTANCE: </span><b style='color:#00FF66; font-size:15px;'>" + QString::number(dist) + " KM</b><br>";
                html += "<span style='color:#94A3B8; font-size:14px;'>EST. TIME: </span><b style='color:#A78BFA; font-size:15px;'>" + QString::number(hours) + " HOURS " + QString::number(minutes) + " MINUTES</b> <span style='color:#475569; font-size:11px;'>(@ 80 km/h)</span></div>";

                resultPanel->setHtml(html);
            } else {
                QMessageBox::warning(this, "ERROR", "NO DIRECT HIGHWAY FOUND. REROUTING REQUIRED.");
                resultPanel->setHtml("<div style='color:#FF003C; font-size:14px; margin-top:35px; text-align:center;'><b>[ ERROR ] NO DIRECT HIGHWAY FOUND.</b></div>");
            }
        }
    }
    else if (mode == 2) {
        // --- SCENARIO 3: UAV PATROL ---
        RouteResult res = Greedy_Matrix(*master_mat, startCity);
        if (res.cost >= INF) {
            resultPanel->setHtml("<div style='color:#FF003C; font-size:14px; margin-top:35px; text-align:center;'><b>[ SYSTEM ERROR: Graph disconnect. Cannot complete patrol loop. ]</b></div>");
            return;
        }

        mainWorkspace->setCurrentIndex(0);
        drawCityNodes(startCity);

        QPen glowPen(QColor(0, 240, 255, 60), 10);
        QPen linePen(NEON_CYAN, 2, Qt::DotLine);

        QString logHtml = "<b style='color:#00F0FF; font-size:14px; letter-spacing: 1px;'>[ AUTONOMOUS UAV PATROL LOG ]</b><br>";
        logHtml += "<span style='color:#94A3B8; font-size:12px; font-family: monospace;'>[SYS] Uploading coordinate matrix to drone...<br>[SYS] Engaging rotors...</span><br><br>";
        logHtml += "<div style='height: 70px; overflow-y: auto; font-family: monospace; font-size: 12px;'>";

        for (int i = 0; i < res.pathLen - 1; i++) {
            QPointF p1 = cityPositions[res.path[i]];
            QPointF p2 = cityPositions[res.path[i+1]];

            mapScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), glowPen);
            mapScene->addLine(p1.x(), p1.y(), p2.x(), p2.y(), linePen);

            logHtml += "<span style='color:#00FF66;'>-> [ARRIVED] Waypoint: " + QString::fromStdString(getCityName(res.path[i+1])).toUpper() + "</span><br>";
            logHtml += "<span style='color:#A78BFA;'>&nbsp;&nbsp;&nbsp;|-- Scanning sector... Clear. Proceeding.</span><br>";

            resultPanel->setHtml(logHtml + "</div>");
            delayAnimation(400);
        }

        logHtml += "<br><span style='color:#00F0FF; font-weight:bold;'>[SYS] Patrol complete. Battery optimal. Entering sleep mode.</span>";
        resultPanel->setHtml(logHtml + "</div>");
    }
}