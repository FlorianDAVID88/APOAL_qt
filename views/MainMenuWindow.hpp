#include <QWidget>

class MainMenuWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainMenuWindow(QWidget *parent = 0);
private:
    void play();
    void inputName();
};
