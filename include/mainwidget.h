#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include <vector>
#include <string>

using namespace std;

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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::MainWidget *ui;
    int totalPeopleCount;
    int groupEntityCount;
    int groupCount;
    vector<vector<string>> team;

    bool m_isDragging;
    QPoint m_dragStartPos;

    void displayResult();
    void updateTotalPeopleCount();
    QStringList getNames();
};

#endif // MAINWIDGET_H
