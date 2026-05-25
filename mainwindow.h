#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QGraphicsTextItem>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QDialog>
#include <QLineEdit>
#include <QFrame>
#include <QProgressBar>
#include <QTimer>
#include <vector>

#include "Graph.h"
#include "Algorithms.h"

// ==========================================
// FULL-SCREEN LOGIN DIALOG
// ==========================================
class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void attemptLogin();
    void updateProgress();

private:
    QFrame *loginBox;
    QLineEdit *userEdit;
    QLineEdit *passEdit;
    QPushButton *loginBtn;
    QLabel *statusLabel;
    QProgressBar *progressBar;
    QTimer *loadingTimer;

    QPushButton *closeBtn;
    QPushButton *minBtn;

    int attempts;
    int progressValue;
    bool isAuthenticating;
};

// ==========================================
// MAIN WINDOW CLASS
// ==========================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void startApplication();
    void loadFile();
    void runGreedy();
    void runBruteForce();
    void runCustom();
    void runComparison();
    void simulateScenario();
    void showMap();
    void showMemoryAnalysis();
    void logout();

private:
    void setupUI();
    void setupPremiumTheme();
    void createWelcomePage();

    void drawBackgroundGraph();
    void drawCityNodes(int startNode = 0);
    void drawRoute(const RouteResult& res, QColor routeColor);
    void updateResultPanel(const RouteResult& res);
    void generateMemoryView();
    void delayAnimation(int milliseconds);

    // Root Level Window Management
    QStackedWidget *rootWorkspace;
    QWidget *welcomePage;
    QWidget *applicationPage;

    // Application UI Elements
    QLabel *topBanner;
    QStackedWidget *mainWorkspace;

    // View 0: Map & Results
    QGraphicsScene *mapScene;
    QGraphicsView *mapView;
    QTextBrowser *resultPanel;

    // View 1: Diagnostics
    QTableWidget *matrixTable;
    QTextBrowser *listBrowser;

    // Controls
    QComboBox *citySelector;
    QComboBox *scenarioSelector;

    // Backend Variables
    StaticGraph* master_mat;
    DynamicGraph* master_list;
    int currentGraphSize;
    bool isGraphLoaded;

    std::vector<QPointF> cityPositions;
};

#endif // MAINWINDOW_H