#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWheelEvent>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QIcon>
#include <QGraphicsLineItem>
#include <QTimer>


ZoomableView::ZoomableView(QGraphicsScene *scene) : QGraphicsView(scene) {
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    viewport()->setCursor(Qt::CrossCursor);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setBackgroundBrush(QBrush(QColor("#1E1E1E")));
}

void ZoomableView::wheelEvent(QWheelEvent *event) {
    double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) scale(scaleFactor, scaleFactor);
    else scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

CalcButton::CalcButton(const QString &text, const QString &color) : QPushButton(text) {
    setFixedSize(60, 60);
    setStyleSheet("QPushButton { "
                  "  background-color: " + color + ";"
                  "  color: white; "
                  "  border-radius: 30px; "
                  "  font-size: 16px; "
                  "  font-family: Arial; "
                  "  font-weight: bold;"
                  "}"
                  "QPushButton:pressed { background-color: #505050; }"
                  "QPushButton:hover { border: 2px solid #888; }");
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), currentStepIndex(-1) {
    setWindowTitle("Data Structure SyntaxTree Calculator");
    setWindowIcon(QIcon(":/ress/App_Icon.png"));
    resize(600, 900);
    setStyleSheet("QMainWindow { background-color: #121212; }");

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Display Area
    display = new QLineEdit;
    display->setPlaceholderText("Enter expression (e.g. 5+3*2)");
    display->setAlignment(Qt::AlignRight);
    display->setStyleSheet(
        "QLineEdit { background-color: #1E1E1E; color: #00E5FF; font-size: 36px; border: 1px solid #333; border-radius: 10px; padding: 10px; }");
    display->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9+\\-*/().]+"), this));
    mainLayout->addWidget(display);

    // Control Buttons Layout (Expand / Shrink)
    QHBoxLayout *stepLayout = new QHBoxLayout();

    btnStepBack = new QPushButton("Expand (Undo)");
    btnStepBack->setStyleSheet(
        "background-color: #D32F2F; color: white; padding: 10px; border-radius: 5px; font-weight: bold;");
    btnStepBack->setEnabled(false);
    connect(btnStepBack, &QPushButton::clicked, this, &MainWindow::onStepBack);

    btnStepForward = new QPushButton("Shrink (Step)");
    btnStepForward->setStyleSheet(
        "background-color: #388E3C; color: white; padding: 10px; border-radius: 5px; font-weight: bold;");
    btnStepForward->setEnabled(false);
    connect(btnStepForward, &QPushButton::clicked, this, &MainWindow::onStepForward);

    stepLayout->addWidget(btnStepBack);
    stepLayout->addWidget(btnStepForward);
    mainLayout->addLayout(stepLayout);

    // Graphics View (The Tree)
    scene = new QGraphicsScene;
    view = new ZoomableView(scene);
    view->setStyleSheet("border: 1px solid #333; border-radius: 10px;");
    mainLayout->addWidget(view);

    // Keypad
    QWidget *keypadWidget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout(keypadWidget);
    gridLayout->setSpacing(10);
    struct BtnDef {
        QString label;
        QString color;
    };
    QList<QList<BtnDef> > rows = {
        {{"AC", "#D32F2F"}, {"DEL", "#C62828"}, {"(", "#424242"}, {")", "#424242"}},
        {{"7", "#303030"}, {"8", "#303030"}, {"9", "#303030"}, {"/", "#1976D2"}},
        {{"4", "#303030"}, {"5", "#303030"}, {"6", "#303030"}, {"*", "#1976D2"}},
        {{"1", "#303030"}, {"2", "#303030"}, {"3", "#303030"}, {"-", "#1976D2"}},
        {{"0", "#303030"}, {".", "#303030"}, {"=", "#388E3C"}, {"+", "#1976D2"}}
    };

    for (int r = 0; r < rows.size(); ++r) {
        for (int c = 0; c < rows[r].size(); ++c) {
            QString text = rows[r][c].label;
            CalcButton *btn = new CalcButton(text, rows[r][c].color);
            gridLayout->addWidget(btn, r, c);

            if (text == "AC")
                connect(btn, &QPushButton::clicked, this, [this]() {
                    display->clear();
                    clearHistory();
                    display->setFocus();
                });
            else if (text == "DEL") connect(btn, &QPushButton::clicked, this, [this]() {
                display->backspace();
                display->setFocus();
            });
            else if (text == "=") connect(btn, &QPushButton::clicked, this, &MainWindow::onEqualPressed);
            else connect(btn, &QPushButton::clicked, this, [this, text]() {
                display->insert(text);
                display->setFocus();
            });
        }
    }
    keypadWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addWidget(keypadWidget);
}

MainWindow::~MainWindow() {
    clearHistory();
}

void MainWindow::clearHistory() {
    for (auto tree: history) delete tree;
    history.clear();
    currentStepIndex = -1;
    scene->clear();
    btnStepBack->setEnabled(false);
    btnStepForward->setEnabled(false);
}

void MainWindow::onEqualPressed() {
    std::string text = display->text().toStdString();
    if (text.empty()) return;

    try {
        clearHistory();

        AST *initialTree = new AST(text);
        initialTree->buildTree();

        history.push_back(initialTree);
        currentStepIndex = 0;

        updateVisualization();

        double result = initialTree->calculate();
        display->setText(QString::number(result));
    } catch (const std::exception &e) {
        display->setText("Error");
        QMessageBox::critical(this, "Calculation Error", e.what());
    }
}

void MainWindow::onStepForward() {
    if (currentStepIndex < 0 || currentStepIndex >= (int) history.size()) return;

    AST *currentTree = history[currentStepIndex];
    AST *nextTree = new AST(*currentTree);

    bool changed = nextTree->simplifyLowestLevel();

    if (changed) {
        while ((int) history.size() > currentStepIndex + 1) {
            delete history.back();
            history.pop_back();
        }
        history.push_back(nextTree);
        currentStepIndex++;
        updateVisualization();
    } else {
        delete nextTree;
        QMessageBox::information(this, "Finished", "The expression is fully simplified!");
    }
}

void MainWindow::onStepBack() {
    if (currentStepIndex > 0) {
        currentStepIndex--;
        updateVisualization();
    }
}

void MainWindow::updateVisualization() {
    if (currentStepIndex < 0) return;

    scene->clear();
    AST *activeTree = history[currentStepIndex];
    drawTreeRecursive(activeTree->getRoot(), 0, 0);
    view->centerOn(0, 0);

    btnStepBack->setEnabled(currentStepIndex > 0);
    btnStepForward->setEnabled(true);
}

int MainWindow::getLeafCount(AST::BNode *node) {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return (node->left ? getLeafCount(node->left) : 0) + (node->right ? getLeafCount(node->right) : 0);
}

void MainWindow::drawTreeRecursive(AST::BNode *node, double x, double y) {
    if (!node) return;
    int nodeSize = 50;
    int verticalGap = 100;
    int leafSpacing = 70;
    std::vector<AST::BNode *> children;
    if (node->left) children.push_back(node->left);
    if (node->right) children.push_back(node->right);

    double totalWidth = 0;
    std::vector<double> childWidths;
    for (auto child: children) {
        double w = getLeafCount(child) * leafSpacing;
        if (w == 0) w = leafSpacing;
        childWidths.push_back(w);
        totalWidth += w;
    }

    double currentChildX = x - (totalWidth / 2.0);
    for (int i = 0; i < children.size(); ++i) {
        double childRealX = currentChildX + (childWidths[i] / 2.0);
        QGraphicsLineItem *line = scene->addLine(x, y + nodeSize / 2, childRealX, y + verticalGap - nodeSize / 2);
        line->setPen(QPen(QColor("#555"), 2));
        line->setZValue(0);
        drawTreeRecursive(children[i], childRealX, y + verticalGap);
        currentChildX += childWidths[i];
    }

    QGraphicsEllipseItem *circle = scene->addEllipse(x - nodeSize / 2, y - nodeSize / 2, nodeSize, nodeSize);
    circle->setBrush(QBrush(QColor("#00E5FF")));
    circle->setPen(QPen(Qt::white, 2));
    circle->setZValue(10);

    QString labelStr = (node->data == "_NEG_") ? "-" : QString::fromStdString(node->data);
    QGraphicsTextItem *text = scene->addText(labelStr);
    text->setDefaultTextColor(Qt::black);
    QFont font("Arial", 12, QFont::Bold);
    text->setFont(font);

    QRectF textRect = text->boundingRect();
    text->setPos(x - textRect.width() / 2, y - textRect.height() / 2);
    text->setZValue(11);
}
