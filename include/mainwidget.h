#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <vector>
#include <string>

using namespace std;

class ImageWidget;


namespace Ui {
class MainWidget;
}
class Community;
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject*, QEvent*) override;

private:
    Ui::MainWidget *ui;
    Community* community;
    int totalPeopleCount;
    int groupEntityCount;
    int groupCount;
    vector<vector<string>> team;

    bool m_isDragging;
    QPoint m_dragStartPos;

    void displayResult();
    void updateTotalPeopleCount();
    void updateCommunity();
    QStringList getNames();

    ImageWidget *cat;
    void updateCatPosition();

};

#endif // MAINWIDGET_H
