/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created: Sun Nov 4 13:08:58 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextBrowser *partyLine;
    QPushButton *pushButton;
    QHBoxLayout *upperDialZone;
    QVBoxLayout *gapZone;
    QLabel *gapLabel;
    QDial *constantG;
    QLCDNumber *gapValue;
    QVBoxLayout *blockadeZone;
    QLabel *blockadeLabel;
    QDial *constantB;
    QLCDNumber *blockadeValue;
    QLineEdit *screenName;
    QLabel *addressLabel;
    QLineEdit *serverAddress;
    QLineEdit *teamName;
    QLabel *nameLabel;
    QLabel *teamLabel;
    QHBoxLayout *lowerDialZone;
    QVBoxLayout *rowAddZone;
    QLabel *rowAddLabel;
    QDial *constantR;
    QLCDNumber *rowAddValue;
    QVBoxLayout *clearBonusZone;
    QLabel *clearBonusLabel;
    QDial *constantC;
    QLCDNumber *clearBonusValue;
    QLCDNumber *speedValue;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(512, 512);
        MainWindow->setMinimumSize(QSize(512, 512));
        MainWindow->setMaximumSize(QSize(512, 512));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        partyLine = new QTextBrowser(centralwidget);
        partyLine->setObjectName(QString::fromUtf8("partyLine"));

        gridLayout->addWidget(partyLine, 4, 2, 2, 1);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);

        upperDialZone = new QHBoxLayout();
        upperDialZone->setObjectName(QString::fromUtf8("upperDialZone"));
        gapZone = new QVBoxLayout();
        gapZone->setObjectName(QString::fromUtf8("gapZone"));
        gapLabel = new QLabel(centralwidget);
        gapLabel->setObjectName(QString::fromUtf8("gapLabel"));
        gapLabel->setAlignment(Qt::AlignCenter);

        gapZone->addWidget(gapLabel);

        constantG = new QDial(centralwidget);
        constantG->setObjectName(QString::fromUtf8("constantG"));
        constantG->setMinimum(-2000);
        constantG->setMaximum(2000);
        constantG->setPageStep(100);
        constantG->setValue(-500);
        constantG->setNotchTarget(15);
        constantG->setNotchesVisible(true);

        gapZone->addWidget(constantG);

        gapValue = new QLCDNumber(centralwidget);
        gapValue->setObjectName(QString::fromUtf8("gapValue"));
        gapValue->setSmallDecimalPoint(true);
        gapValue->setDigitCount(5);
        gapValue->setSegmentStyle(QLCDNumber::Flat);
        gapValue->setProperty("intValue", QVariant(-500));

        gapZone->addWidget(gapValue);


        upperDialZone->addLayout(gapZone);

        blockadeZone = new QVBoxLayout();
        blockadeZone->setObjectName(QString::fromUtf8("blockadeZone"));
        blockadeLabel = new QLabel(centralwidget);
        blockadeLabel->setObjectName(QString::fromUtf8("blockadeLabel"));
        blockadeLabel->setAlignment(Qt::AlignCenter);

        blockadeZone->addWidget(blockadeLabel);

        constantB = new QDial(centralwidget);
        constantB->setObjectName(QString::fromUtf8("constantB"));
        constantB->setMinimum(-2000);
        constantB->setMaximum(2000);
        constantB->setPageStep(100);
        constantB->setValue(-1000);
        constantB->setNotchTarget(15);
        constantB->setNotchesVisible(true);

        blockadeZone->addWidget(constantB);

        blockadeValue = new QLCDNumber(centralwidget);
        blockadeValue->setObjectName(QString::fromUtf8("blockadeValue"));
        blockadeValue->setSmallDecimalPoint(true);
        blockadeValue->setDigitCount(5);
        blockadeValue->setSegmentStyle(QLCDNumber::Flat);
        blockadeValue->setProperty("intValue", QVariant(-1000));

        blockadeZone->addWidget(blockadeValue);


        upperDialZone->addLayout(blockadeZone);


        gridLayout->addLayout(upperDialZone, 4, 0, 1, 2);

        screenName = new QLineEdit(centralwidget);
        screenName->setObjectName(QString::fromUtf8("screenName"));

        gridLayout->addWidget(screenName, 0, 1, 1, 1);

        addressLabel = new QLabel(centralwidget);
        addressLabel->setObjectName(QString::fromUtf8("addressLabel"));

        gridLayout->addWidget(addressLabel, 2, 0, 1, 1);

        serverAddress = new QLineEdit(centralwidget);
        serverAddress->setObjectName(QString::fromUtf8("serverAddress"));

        gridLayout->addWidget(serverAddress, 2, 1, 1, 1);

        teamName = new QLineEdit(centralwidget);
        teamName->setObjectName(QString::fromUtf8("teamName"));

        gridLayout->addWidget(teamName, 1, 1, 1, 1);

        nameLabel = new QLabel(centralwidget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        teamLabel = new QLabel(centralwidget);
        teamLabel->setObjectName(QString::fromUtf8("teamLabel"));

        gridLayout->addWidget(teamLabel, 1, 0, 1, 1);

        lowerDialZone = new QHBoxLayout();
        lowerDialZone->setObjectName(QString::fromUtf8("lowerDialZone"));
        rowAddZone = new QVBoxLayout();
        rowAddZone->setObjectName(QString::fromUtf8("rowAddZone"));
        rowAddLabel = new QLabel(centralwidget);
        rowAddLabel->setObjectName(QString::fromUtf8("rowAddLabel"));
        rowAddLabel->setAlignment(Qt::AlignCenter);

        rowAddZone->addWidget(rowAddLabel);

        constantR = new QDial(centralwidget);
        constantR->setObjectName(QString::fromUtf8("constantR"));
        constantR->setMinimum(-2000);
        constantR->setMaximum(2000);
        constantR->setPageStep(100);
        constantR->setValue(-50);
        constantR->setNotchTarget(15);
        constantR->setNotchesVisible(true);

        rowAddZone->addWidget(constantR);

        rowAddValue = new QLCDNumber(centralwidget);
        rowAddValue->setObjectName(QString::fromUtf8("rowAddValue"));
        rowAddValue->setSmallDecimalPoint(true);
        rowAddValue->setDigitCount(5);
        rowAddValue->setSegmentStyle(QLCDNumber::Flat);
        rowAddValue->setProperty("intValue", QVariant(-50));

        rowAddZone->addWidget(rowAddValue);


        lowerDialZone->addLayout(rowAddZone);

        clearBonusZone = new QVBoxLayout();
        clearBonusZone->setObjectName(QString::fromUtf8("clearBonusZone"));
        clearBonusLabel = new QLabel(centralwidget);
        clearBonusLabel->setObjectName(QString::fromUtf8("clearBonusLabel"));
        clearBonusLabel->setAlignment(Qt::AlignCenter);

        clearBonusZone->addWidget(clearBonusLabel);

        constantC = new QDial(centralwidget);
        constantC->setObjectName(QString::fromUtf8("constantC"));
        constantC->setMinimum(-2000);
        constantC->setMaximum(2000);
        constantC->setPageStep(100);
        constantC->setValue(1000);
        constantC->setNotchTarget(15);
        constantC->setNotchesVisible(true);

        clearBonusZone->addWidget(constantC);

        clearBonusValue = new QLCDNumber(centralwidget);
        clearBonusValue->setObjectName(QString::fromUtf8("clearBonusValue"));
        clearBonusValue->setSmallDecimalPoint(true);
        clearBonusValue->setDigitCount(5);
        clearBonusValue->setSegmentStyle(QLCDNumber::Flat);
        clearBonusValue->setProperty("intValue", QVariant(1000));

        clearBonusZone->addWidget(clearBonusValue);


        lowerDialZone->addLayout(clearBonusZone);


        gridLayout->addLayout(lowerDialZone, 5, 0, 1, 2);

        speedValue = new QLCDNumber(centralwidget);
        speedValue->setObjectName(QString::fromUtf8("speedValue"));
        speedValue->setSmallDecimalPoint(true);
        speedValue->setDigitCount(6);
        speedValue->setMode(QLCDNumber::Dec);
        speedValue->setSegmentStyle(QLCDNumber::Flat);
        speedValue->setProperty("value", QVariant(1.1234));

        gridLayout->addWidget(speedValue, 0, 2, 4, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 512, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
#ifndef QT_NO_SHORTCUT
        gapLabel->setBuddy(constantG);
        blockadeLabel->setBuddy(constantB);
        addressLabel->setBuddy(serverAddress);
        nameLabel->setBuddy(screenName);
        teamLabel->setBuddy(teamName);
        rowAddLabel->setBuddy(constantR);
        clearBonusLabel->setBuddy(constantC);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(screenName, teamName);
        QWidget::setTabOrder(teamName, serverAddress);
        QWidget::setTabOrder(serverAddress, pushButton);
        QWidget::setTabOrder(pushButton, constantG);
        QWidget::setTabOrder(constantG, constantB);
        QWidget::setTabOrder(constantB, constantR);
        QWidget::setTabOrder(constantR, constantC);
        QWidget::setTabOrder(constantC, partyLine);

        retranslateUi(MainWindow);
        QObject::connect(constantG, SIGNAL(dialMoved(int)), gapValue, SLOT(display(int)));
        QObject::connect(constantB, SIGNAL(dialMoved(int)), blockadeValue, SLOT(display(int)));
        QObject::connect(constantR, SIGNAL(dialMoved(int)), rowAddValue, SLOT(display(int)));
        QObject::connect(constantC, SIGNAL(dialMoved(int)), clearBonusValue, SLOT(display(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Deep Thought TetriNET AI", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        gapLabel->setText(QApplication::translate("MainWindow", "Gap Penalty", 0, QApplication::UnicodeUTF8));
        blockadeLabel->setText(QApplication::translate("MainWindow", "Blockade Penalty", 0, QApplication::UnicodeUTF8));
        screenName->setText(QApplication::translate("MainWindow", "DeepThought", 0, QApplication::UnicodeUTF8));
        addressLabel->setText(QApplication::translate("MainWindow", "Server Address", 0, QApplication::UnicodeUTF8));
        serverAddress->setInputMask(QString());
        nameLabel->setText(QApplication::translate("MainWindow", "Screen Name", 0, QApplication::UnicodeUTF8));
        teamLabel->setText(QApplication::translate("MainWindow", "Team Name", 0, QApplication::UnicodeUTF8));
        rowAddLabel->setText(QApplication::translate("MainWindow", "Row Add Penalty", 0, QApplication::UnicodeUTF8));
        clearBonusLabel->setText(QApplication::translate("MainWindow", "Clear Bonus", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_MAIN_H
