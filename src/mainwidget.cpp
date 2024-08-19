#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "community.h"

#include <fstream>

#include <QPushButton>
#include <QLayoutItem>
#include <QLabel>
#include <QRegularExpression>
#include <QMessageBox>

#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include <QClipboard>

#include <vector>
#include <string>

using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget), m_isDragging(false)
{
    ui->setupUi(this);
    community = new Community();
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
            copyStr += "\n";
        }
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copyStr);
    });

    connect(ui->generateGroupButton, &QPushButton::clicked, this, [=]() {
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
    if (groupCountTemp == "") {
        // 안내 창 띄우기
        QMessageBox::warning(this, "경고", "그룹 수를 입력해주세요.");
        return;
    }
    QStringList names = getNames();
    totalPeopleCount = names.size();
    groupCount = ui->groupCountLineEdit->text().toInt();
    if (groupCount == 0) {
        QMessageBox::warning(this, "경고", "그룹 수는 1이상 입니다.");
        return;
    }
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
        label->setStyleSheet("QLabel {font-size: 14pt;}");
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

