#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <vector>
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

private:
    // UI Elements
    QLineEdit *display;
    QGraphicsScene *scene;
    ZoomableView *view;
    QPushButton *btnStepBack;
    QPushButton *btnStepForward;

    // Logic Members
    std::vector<AST*> history;
    int currentStepIndex;

    // Helper Functions
    void updateVisualization();
    int getLeafCount(AST::BNode* node);
    void drawTreeRecursive(AST::BNode* node, double x, double y);
};

#endif