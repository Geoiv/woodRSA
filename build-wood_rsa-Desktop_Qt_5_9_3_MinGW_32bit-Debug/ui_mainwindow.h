/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_7;
    QGridLayout *gridLayout;
    QVBoxLayout *rsa2Layout;
    QLabel *rsa2Img;
    QLabel *rsa2Label;
    QPushButton *rsa2KeyButton;
    QPushButton *rsa2EncButton;
    QPushButton *rsa2SgnButton;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QVBoxLayout *rsa1Layout;
    QLabel *rsa1Img;
    QLabel *rsa1Label;
    QPushButton *rsa1KeyButton;
    QPushButton *rsa1EncButton;
    QPushButton *rsa1SgnButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1018, 586);
        QPalette palette;
        QBrush brush(QColor(120, 125, 121, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        MainWindow->setPalette(palette);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(120, 125, 121)"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(90, 10, 871, 471));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(widget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMaximumSize(QSize(16777215, 50));

        verticalLayout_4->addWidget(label_7);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        rsa2Layout = new QVBoxLayout();
        rsa2Layout->setSpacing(6);
        rsa2Layout->setObjectName(QStringLiteral("rsa2Layout"));
        rsa2Img = new QLabel(widget);
        rsa2Img->setObjectName(QStringLiteral("rsa2Img"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(rsa2Img->sizePolicy().hasHeightForWidth());
        rsa2Img->setSizePolicy(sizePolicy);
        rsa2Img->setMaximumSize(QSize(150, 150));
        rsa2Img->setTextFormat(Qt::RichText);
        rsa2Img->setPixmap(QPixmap(QString::fromUtf8(":/lock-solid.png")));
        rsa2Img->setScaledContents(true);
        rsa2Img->setAlignment(Qt::AlignCenter);

        rsa2Layout->addWidget(rsa2Img, 0, Qt::AlignHCenter);

        rsa2Label = new QLabel(widget);
        rsa2Label->setObjectName(QStringLiteral("rsa2Label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rsa2Label->sizePolicy().hasHeightForWidth());
        rsa2Label->setSizePolicy(sizePolicy1);
        rsa2Label->setMaximumSize(QSize(16777215, 20));

        rsa2Layout->addWidget(rsa2Label, 0, Qt::AlignHCenter);

        rsa2KeyButton = new QPushButton(widget);
        rsa2KeyButton->setObjectName(QStringLiteral("rsa2KeyButton"));
        rsa2KeyButton->setMaximumSize(QSize(350, 16777215));

        rsa2Layout->addWidget(rsa2KeyButton, 0, Qt::AlignHCenter);

        rsa2EncButton = new QPushButton(widget);
        rsa2EncButton->setObjectName(QStringLiteral("rsa2EncButton"));
        rsa2EncButton->setMaximumSize(QSize(350, 16777215));

        rsa2Layout->addWidget(rsa2EncButton, 0, Qt::AlignHCenter);

        rsa2SgnButton = new QPushButton(widget);
        rsa2SgnButton->setObjectName(QStringLiteral("rsa2SgnButton"));
        rsa2SgnButton->setMaximumSize(QSize(350, 16777215));

        rsa2Layout->addWidget(rsa2SgnButton, 0, Qt::AlignHCenter);


        gridLayout->addLayout(rsa2Layout, 0, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(150, 150));

        verticalLayout_2->addWidget(label_3, 0, Qt::AlignHCenter);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(16777215, 20));

        verticalLayout_2->addWidget(label_4, 0, Qt::AlignHCenter);

        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(250, 16777215));

        verticalLayout_2->addWidget(pushButton_4, 0, Qt::AlignHCenter);

        pushButton_5 = new QPushButton(widget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setMaximumSize(QSize(250, 16777215));

        verticalLayout_2->addWidget(pushButton_5, 0, Qt::AlignHCenter);

        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setMaximumSize(QSize(250, 16777215));

        verticalLayout_2->addWidget(pushButton_6, 0, Qt::AlignHCenter);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);

        rsa1Layout = new QVBoxLayout();
        rsa1Layout->setSpacing(6);
        rsa1Layout->setObjectName(QStringLiteral("rsa1Layout"));
        rsa1Img = new QLabel(widget);
        rsa1Img->setObjectName(QStringLiteral("rsa1Img"));
        rsa1Img->setEnabled(true);
        sizePolicy.setHeightForWidth(rsa1Img->sizePolicy().hasHeightForWidth());
        rsa1Img->setSizePolicy(sizePolicy);
        rsa1Img->setMaximumSize(QSize(150, 150));
        rsa1Img->setTextFormat(Qt::RichText);
        rsa1Img->setPixmap(QPixmap(QString::fromUtf8(":/lock-outline.png")));
        rsa1Img->setScaledContents(true);
        rsa1Img->setAlignment(Qt::AlignCenter);

        rsa1Layout->addWidget(rsa1Img, 0, Qt::AlignHCenter);

        rsa1Label = new QLabel(widget);
        rsa1Label->setObjectName(QStringLiteral("rsa1Label"));
        rsa1Label->setMaximumSize(QSize(16777215, 20));

        rsa1Layout->addWidget(rsa1Label, 0, Qt::AlignHCenter);

        rsa1KeyButton = new QPushButton(widget);
        rsa1KeyButton->setObjectName(QStringLiteral("rsa1KeyButton"));
        rsa1KeyButton->setMaximumSize(QSize(350, 16777215));

        rsa1Layout->addWidget(rsa1KeyButton, 0, Qt::AlignHCenter);

        rsa1EncButton = new QPushButton(widget);
        rsa1EncButton->setObjectName(QStringLiteral("rsa1EncButton"));
        rsa1EncButton->setMaximumSize(QSize(350, 16777215));

        rsa1Layout->addWidget(rsa1EncButton, 0, Qt::AlignHCenter);

        rsa1SgnButton = new QPushButton(widget);
        rsa1SgnButton->setObjectName(QStringLiteral("rsa1SgnButton"));
        rsa1SgnButton->setMaximumSize(QSize(350, 16777215));

        rsa1Layout->addWidget(rsa1SgnButton, 0, Qt::AlignHCenter);


        gridLayout->addLayout(rsa1Layout, 0, 0, 1, 1);


        verticalLayout_4->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1018, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:24pt;\">FIPS COMPLIANT RSA-2048 &amp; SHA-224</span></p></body></html>", Q_NULLPTR));
        rsa2Img->setText(QString());
        rsa2Label->setText(QApplication::translate("MainWindow", "RSA-2", Q_NULLPTR));
        rsa2KeyButton->setText(QApplication::translate("MainWindow", "Key Options", Q_NULLPTR));
        rsa2EncButton->setText(QApplication::translate("MainWindow", "Encryption and Decryption", Q_NULLPTR));
        rsa2SgnButton->setText(QApplication::translate("MainWindow", "Signing and Authentication", Q_NULLPTR));
        label_3->setText(QString());
        label_4->setText(QString());
        pushButton_4->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        rsa1Img->setText(QString());
        rsa1Label->setText(QApplication::translate("MainWindow", "RSA-1", Q_NULLPTR));
        rsa1KeyButton->setText(QApplication::translate("MainWindow", "Key Options", Q_NULLPTR));
        rsa1EncButton->setText(QApplication::translate("MainWindow", "Encryption and Decryption", Q_NULLPTR));
        rsa1SgnButton->setText(QApplication::translate("MainWindow", "Signing and Authentication", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
