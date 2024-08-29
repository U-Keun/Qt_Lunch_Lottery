#include "mainwidget.h"
#include "imagewidget.h"
#include "ui_mainwidget.h"
#include "community.h"

#include <fstream>

#include <QPushButton>
#include <QLayoutItem>
#include <QLabel>
#include <QRegularExpression>
#include <QMessageBox>
#include <QIcon>
#include <QTimer>
#include <QIntValidator>
#include <QApplication>

#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include <QClipboard>
#include <QEvent>

#include <vector>
#include <string>

using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget), m_isDragging(false)
{
    ui->setupUi(this);
    community = new Community();
    cat = new ImageWidget(ui->inputWidget);

    this->setWindowTitle("RanBab");

    qApp->installEventFilter(this); // cat->sefFocus의 우선순위를 최우선으로 설정

    connect(ui->chooseFileButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("학생 데이터 파일 선택"), "",
                                                        tr("텍스트 파일 (*.txt);;모든 파일 (*)"));
        if (fileName.isEmpty()) return;

        ifstream fin;
        fin.open(fileName.toStdString());

        fin >> totalPeopleCount >> groupCount;
        groupEntityCount = ((totalPeopleCount - 1) / groupCount) + 1;

        QString str = tr("");
        for (int i = 0; i < totalPeopleCount; i++) {
            string name; fin >> name;
            str += name + "\n";
        }

        ui->groupCountLineEdit->setText(QString::number(groupCount));
        ui->groupEntryText->setText(str);
        updateTotalPeopleCount();
        fin.close();
    });

    connect(ui->saveButton, &QPushButton::clicked, this, [=]() {
        if (team.empty()) return;
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("학생 데이터 저장 파일 선택"), "",
                                                        tr("텍스트 파일 (*.txt);;모든 파일 (*)"));

        if (fileName.isEmpty()) return;

        ofstream fout;
        fout.open(fileName.toStdString());

        fout << totalPeopleCount << " " << groupCount << endl;
        for (int j = 0; j < team[0].size(); j++) {
            for (int i = 0; i < team.size(); i++) {
                if (team[i][j] == "none") continue;
                fout << team[i][j] << " ";
            }
        }

        fout.close();
    });

    connect(ui->copyButton, &QPushButton::clicked, this, [=]() {
        QString copyStr;
        for (int i = 0; i < team.size(); i++) {
            copyStr += QString::number(i+1) + tr("조: ");
            for (int j = 0; j < team[0].size(); j++) {
                if (team[i][j] == "none") continue;
                copyStr += QString::fromStdString(team[i][j]) + tr(", ");
            }
            copyStr.chop(2);
            copyStr += "\n";
        }
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copyStr);

        QIcon defaultIcon(":/resource/copy-icon.svg");
        QIcon newIcon(":/resource/check-icon.svg");
        ui->copyButton->setIcon(newIcon);
        QTimer::singleShot(2000, [=]() {
            ui->copyButton->setIcon(defaultIcon);
        });
    });

    connect(ui->generateGroupButton, &QPushButton::clicked, this, [=]() {
        if (ui->groupCountLineEdit->text() == "") {
            QMessageBox::warning(this, "경고", "그룹 수를 입력해주세요.");
            return;
        }
        if (ui->groupCountLineEdit->text().toInt() == 0) {
            QMessageBox::warning(this, "경고", "그룹 수는 1이상 입니다.");
            return;
        }
        QStringList names = getNames();
        if (names.empty()) return;
        community->shuffle();
        team = community->getTeam();

        if (team.empty()) return;
        displayResult();
    });

    connect(ui->groupCountLineEdit, &QLineEdit::textChanged, this, [=]() {
        updateCommunity();
    });

    connect(ui->groupEntryText, &QTextEdit::textChanged, this, [=]() {
        updateTotalPeopleCount();
        updateCommunity();
    });
    QIntValidator *validator = new QIntValidator(1, 99, this);
    ui->groupCountLineEdit->setValidator(validator);

    // 키보드 입력에 대한 focus 전환의 경우 text위젯에 대해서는 예외처리를 했으니까 이건 필요
    connect(ui->groupCountLineEdit, &QLineEdit::returnPressed, this, [=]() {
        cat->setFocus();
    });

    connect(cat, &ImageWidget::catPunchedButton, ui->generateGroupButton, &QPushButton::click);

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::updateTotalPeopleCount() {
    QStringList names = getNames();
    int nameCount = names.size();
    ui->totalPeopleCountLabel->setText(tr("총 인원: ") + QString::number(nameCount) + tr("명"));
    totalPeopleCount = nameCount;
}

void MainWidget::updateCommunity() {
    QString groupCountTemp = ui->groupCountLineEdit->text();
    if (groupCountTemp == "") return;
    QStringList names = getNames();
    totalPeopleCount = names.size();
    groupCount = groupCountTemp.toInt();
    if (groupCount == 0) return;
    groupEntityCount = ((totalPeopleCount - 1) / groupCount) + 1;
    vector<string> students;
    for (int i = 0; i < names.size(); i++) {
        students.push_back(names[i].toStdString());
    }
    for (int i = 0; i < groupCount * groupEntityCount - totalPeopleCount; i++) {
        students.push_back("none");
    }

    community->setGroupCount(groupCount);
    community->setGroupEntityCount(groupEntityCount);
    community->setStudent(students);
}

void MainWidget::displayResult() {
    // 레이 아웃 아이템 삭제
    QLayoutItem* item;
    while ((item = ui->verticalLayout_3->itemAt(0)) != nullptr) {
        item->widget()->deleteLater();
        ui->verticalLayout_3->removeItem(item);
        delete item;
    }
    ui->verticalLayout_3->update();
    for (int i = 0; i < team.size(); i++) {
        QString member;
        QLabel* label = new QLabel();
        for (int j = 0; j < team[i].size(); j++) {
            if (team[i][j] == "none") continue;

            member += QString::fromStdString(team[i][j]) + tr(" ");
        }
        label->setText(member);
        label->setStyleSheet("QLabel {"
                             "font-size: 14pt; "
                             "color: black; "
                             "background-color: rgb(247, 248, 249);"
                             "border: 1px solid gray;"
                             "border-radius: 4px;}");
        label->setMargin(12);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);

        ui->verticalLayout_3->addWidget(label);
    }
    ui->verticalLayout_3->addStretch();

}

QStringList MainWidget::getNames() {
    QString text = ui->groupEntryText->toPlainText();
    static QRegularExpression rExpression("\\s+");
    QStringList names = text.split(rExpression, Qt::SkipEmptyParts);
    return names;
}

void MainWidget::updateCatPosition() {
    if (cat) {
        cat->setGeometry(0, height() - 175, 250, 160);
    }
}

void MainWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateCatPosition();
}

bool MainWidget::eventFilter(QObject* obj, QEvent* event) {
    // text 위젯에 대해서는 focus를 잃어야 이미지로 focus가 전환되도록 설정
    if (obj == ui->groupCountLineEdit || obj == ui->groupEntryText) {
        if (event->type() == QEvent::FocusOut) {
            cat->setFocus();
        }
    }
    // 나머지 위젯(버튼들)에 대해서는 마우스 및 키보드 클릭 이후에는 이미지로 focus가 전환되도록 설정
    // 근데 1번째 if문에서 text 위젯에 대해 설정했으면 else if 문에서는 생략해도 되지 않나? 왜 안되지...?
    else if ((!ui->groupCountLineEdit->hasFocus() && !ui->groupEntryText->hasFocus()) && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress)) {
        cat->setFocus();
    }

    return QWidget::eventFilter(obj, event);
}
