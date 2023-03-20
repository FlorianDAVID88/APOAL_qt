#include <QMainWindow>
#include <QPlainTextEdit>
#include <QCloseEvent>
#include <QSessionManager>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void createView();
    void openBox(QPushButton* button);
    std::vector<QPushButton *> boxes;
    std::vector<QString> prices;
    std::vector<QLabel *> labels;

};