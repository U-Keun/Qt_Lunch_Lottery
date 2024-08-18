#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class ImageWidget;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ImageWidget *cat;
};

#endif // MAINWIDGET_H
