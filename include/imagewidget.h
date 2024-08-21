#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QTimer>
#include <random>

class ImageWidget : public QWidget
{
    Q_OBJECT

    QPixmap cat, mouth, leftPawUp, leftPawDown, rightPawUp, rightPawDown;
    int pawState;
    QTimer *resetTimer;
    QRect clickBox;

    std::default_random_engine generator;

signals:
    void catPunchedButton();

private slots:
    void resetScreen();

public:
    ImageWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
};

#endif // IMAGEWIDGET_H
