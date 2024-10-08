#include "../include/imagewidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), pawState(0), generator(std::random_device{}()) {
    QPixmap originalCat = QPixmap(":/resource/animal.png");
    QPixmap originalMouth = QPixmap(":/resource/mouth.png");
    QPixmap originalLeftPaw = QPixmap(":/resource/paw-left.png");
    QPixmap originalRightPaw = QPixmap(":/resource/paw-right.png");

    qreal scaleFactor = 0.6;

    cat = originalCat.scaled(originalCat.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    mouth = originalMouth.scaled(originalMouth.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 이미지 크기 정의
    int imageWidth = originalLeftPaw.width() / 2; // 1600 / 2 = 800
    int imageHeight = originalLeftPaw.height() / 2; // 900 / 2 = 450

    // 왼쪽 위
    leftPawUp = originalLeftPaw.copy(0, 0, imageWidth, imageHeight).scaled(imageWidth * scaleFactor, imageHeight * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    rightPawUp = originalRightPaw.copy(0, 0, imageWidth, imageHeight).scaled(imageWidth * scaleFactor, imageHeight * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 오른쪽 위
    leftPawDown = originalLeftPaw.copy(imageWidth, 0, imageWidth, imageHeight).scaled(imageWidth * scaleFactor, imageHeight * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    rightPawDown = originalRightPaw.copy(imageWidth, 0, imageWidth, imageHeight).scaled(imageWidth * scaleFactor, imageHeight * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    clickBox = QRect(0, 0, 200, 120);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);
    connect(resetTimer, &QTimer::timeout, this, &ImageWidget::resetScreen);

    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void ImageWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.drawPixmap(-200, -266, cat);
    // painter.drawPixmap(-200, 0, mouth);

    switch (pawState) {
    case 0:
        painter.drawPixmap(-198, 6, leftPawUp);
        painter.drawPixmap(-190, 4, rightPawUp);
        break;
    case 1:
        painter.drawPixmap(-198, 6, leftPawDown);
        painter.drawPixmap(-190, 4, rightPawUp);
        break;
    case 2:
        painter.drawPixmap(-198, 6, leftPawUp);
        painter.drawPixmap(-190, 4, rightPawDown);
        break;
    case 3:
        painter.drawPixmap(-198, 6, leftPawDown);
        painter.drawPixmap(-190, 4, rightPawDown);
        break;
    }
}

void ImageWidget::keyPressEvent(QKeyEvent *event) {
    std::uniform_int_distribution<int> distribution(1, 3);

    if (event->key() == Qt::Key_Space) {
        pawState = distribution(generator);
        update();

        if (resetTimer->isActive()) {
            resetTimer->stop();
        }

        resetTimer->start(300);

        if (pawState == 1 || pawState == 3) {
            emit catPunchedButton();
        }
    }

    QWidget::keyPressEvent(event);
}

void ImageWidget::resetScreen() {
    pawState = 0;
    update();
}
