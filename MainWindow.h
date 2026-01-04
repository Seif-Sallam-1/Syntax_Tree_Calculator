#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QDialog>
#include <QTextEdit>
#include <QDateTime>
#include "AST.h"


class ZoomableView : public QGraphicsView {
public:
    explicit ZoomableView(QGraphicsScene *scene);
protected:
    void wheelEvent(QWheelEvent *event) override;
};

class CalcButton : public QPushButton {
public:
    CalcButton(const QString &text, const QString &color = "#2D2D2D");
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void clearHistory();
    void onEqualPressed();
    void onStepForward();
    void onStepBack();
    void showHistory();

private:
    QLineEdit *display;
    QGraphicsScene *scene;
    ZoomableView *view;
    QPushButton *btnStepBack;
    QPushButton *btnStepForward;
    QPushButton *btnHistory;

    std::vector<AST*> history;
    int currentStepIndex;

    void updateVisualization();

    int getLeafCount(AST::BNode* node);
    int calculateMaxNodeWidth(AST::BNode* node);
    void drawTreeRecursive(AST::BNode* node, double x, double y, double spacing);

    void saveToHistory(const QString &expression, const QString &result);
};

#endif