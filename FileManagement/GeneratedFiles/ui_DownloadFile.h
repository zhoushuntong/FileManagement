/********************************************************************************
** Form generated from reading UI file 'DownloadFile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOADFILE_H
#define UI_DOWNLOADFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DownloadFile
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *condition;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QTableView *downloadTable;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QProgressBar *progressBar;
    QLabel *downloadSpeedLabel;
    QSpacerItem *horizontalSpacer_4; 

    void setupUi(QWidget *DownloadFile)
    {
        if (DownloadFile->objectName().isEmpty())
            DownloadFile->setObjectName(QStringLiteral("DownloadFile"));
        DownloadFile->resize(811, 504);
        verticalLayout = new QVBoxLayout(DownloadFile);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(DownloadFile);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        condition = new QLineEdit(DownloadFile);
        condition->setObjectName(QStringLiteral("condition"));

        horizontalLayout->addWidget(condition);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        downloadTable = new QTableView(DownloadFile);
        downloadTable->setObjectName(QStringLiteral("downloadTable"));
        downloadTable->setMinimumSize(QSize(600, 400));

        horizontalLayout_3->addWidget(downloadTable);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        progressBar = new QProgressBar(DownloadFile);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);

        downloadSpeedLabel = new QLabel(DownloadFile);
        downloadSpeedLabel->setObjectName(QStringLiteral("downloadSpeedLabel"));

        verticalLayout_2->addWidget(downloadSpeedLabel);


        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(DownloadFile);

        QMetaObject::connectSlotsByName(DownloadFile);
    } // setupUi

    void retranslateUi(QWidget *DownloadFile)
    {
        DownloadFile->setWindowTitle(QApplication::translate("DownloadFile", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("DownloadFile", "\346\226\207\344\273\266\345\220\215\357\274\232", Q_NULLPTR));
        downloadSpeedLabel->setText(QApplication::translate("DownloadFile", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DownloadFile: public Ui_DownloadFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOADFILE_H
