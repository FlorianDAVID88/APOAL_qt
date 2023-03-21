#include <QPlainTextEdit>
#include <QCloseEvent>
#include <QSessionManager>
#include <QPushButton>
#include <QLabel>

class GameWindow : public QWidget {
Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);

private:
    void createView();
    void openBox(QPushButton* button);
    std::vector<QPushButton *> boxes;
    std::vector<QString> prices;
    std::vector<QLabel *> labels;

};