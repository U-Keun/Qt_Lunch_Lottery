#include "mainwidget.h"
#include "imagewidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    cat = new ImageWidget(this);
    cat->setGeometry(10, 430, 400, 200);

}

MainWidget::~MainWidget()
{
    delete ui;
}
