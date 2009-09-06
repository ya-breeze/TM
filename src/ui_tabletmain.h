/********************************************************************************
** Form generated from reading ui file 'tabletmain.ui'
**
** Created: Sun Sep 6 18:01:54 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TABLETMAIN_H
#define UI_TABLETMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabletMain
{
public:
    QAction *actionRestore;
    QAction *actionSave;
    QAction *actionExit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *btnTasks;
    QPushButton *btnChrono;
    QPushButton *btnAddActivity;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QTreeView *treeView;
    QTextEdit *teNotes;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnAddChild;
    QPushButton *btnAddSibling;
    QPushButton *btnDeleteTask;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lblCurrentActivity;
    QToolButton *btnToTasks;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QLabel *lblActivityStarted;
    QFrame *line_2;
    QLabel *label_7;
    QListView *lvActivities;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDateTimeEdit *teActivityStartTime;
    QToolButton *btnUpdateTime;
    QLabel *label_4;
    QLineEdit *leActivityName;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *rbActivityTask;
    QRadioButton *rbActivityOther;
    QPushButton *btnNewActivity;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_5;
    QListView *lvLastActivities;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TabletMain)
    {
        if (TabletMain->objectName().isEmpty())
            TabletMain->setObjectName(QString::fromUtf8("TabletMain"));
        TabletMain->resize(640, 480);
        actionRestore = new QAction(TabletMain);
        actionRestore->setObjectName(QString::fromUtf8("actionRestore"));
        actionSave = new QAction(TabletMain);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(TabletMain);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(TabletMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btnTasks = new QPushButton(centralwidget);
        btnTasks->setObjectName(QString::fromUtf8("btnTasks"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnTasks->sizePolicy().hasHeightForWidth());
        btnTasks->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(btnTasks);

        btnChrono = new QPushButton(centralwidget);
        btnChrono->setObjectName(QString::fromUtf8("btnChrono"));
        sizePolicy.setHeightForWidth(btnChrono->sizePolicy().hasHeightForWidth());
        btnChrono->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(btnChrono);

        btnAddActivity = new QPushButton(centralwidget);
        btnAddActivity->setObjectName(QString::fromUtf8("btnAddActivity"));
        sizePolicy.setHeightForWidth(btnAddActivity->sizePolicy().hasHeightForWidth());
        btnAddActivity->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(btnAddActivity);


        horizontalLayout->addLayout(verticalLayout);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        horizontalLayout_2 = new QHBoxLayout(page);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        treeView = new QTreeView(page);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(3);
        sizePolicy1.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(treeView);

        teNotes = new QTextEdit(page);
        teNotes->setObjectName(QString::fromUtf8("teNotes"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(teNotes->sizePolicy().hasHeightForWidth());
        teNotes->setSizePolicy(sizePolicy2);

        verticalLayout_3->addWidget(teNotes);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        btnAddChild = new QPushButton(page);
        btnAddChild->setObjectName(QString::fromUtf8("btnAddChild"));
        sizePolicy.setHeightForWidth(btnAddChild->sizePolicy().hasHeightForWidth());
        btnAddChild->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(btnAddChild);

        btnAddSibling = new QPushButton(page);
        btnAddSibling->setObjectName(QString::fromUtf8("btnAddSibling"));
        sizePolicy.setHeightForWidth(btnAddSibling->sizePolicy().hasHeightForWidth());
        btnAddSibling->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(btnAddSibling);

        btnDeleteTask = new QPushButton(page);
        btnDeleteTask->setObjectName(QString::fromUtf8("btnDeleteTask"));
        sizePolicy.setHeightForWidth(btnDeleteTask->sizePolicy().hasHeightForWidth());
        btnDeleteTask->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(btnDeleteTask);


        horizontalLayout_2->addLayout(verticalLayout_2);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        verticalLayout_6 = new QVBoxLayout(page_2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(page_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lblCurrentActivity = new QLabel(page_2);
        lblCurrentActivity->setObjectName(QString::fromUtf8("lblCurrentActivity"));

        horizontalLayout_4->addWidget(lblCurrentActivity);

        btnToTasks = new QToolButton(page_2);
        btnToTasks->setObjectName(QString::fromUtf8("btnToTasks"));

        horizontalLayout_4->addWidget(btnToTasks);


        verticalLayout_6->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_5->addWidget(label_6);

        lblActivityStarted = new QLabel(page_2);
        lblActivityStarted->setObjectName(QString::fromUtf8("lblActivityStarted"));

        horizontalLayout_5->addWidget(lblActivityStarted);


        verticalLayout_6->addLayout(horizontalLayout_5);

        line_2 = new QFrame(page_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_2);

        label_7 = new QLabel(page_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_7);

        lvActivities = new QListView(page_2);
        lvActivities->setObjectName(QString::fromUtf8("lvActivities"));

        verticalLayout_6->addWidget(lvActivities);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        verticalLayout_5 = new QVBoxLayout(page_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(page_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        teActivityStartTime = new QDateTimeEdit(page_3);
        teActivityStartTime->setObjectName(QString::fromUtf8("teActivityStartTime"));

        gridLayout->addWidget(teActivityStartTime, 0, 1, 1, 1);

        btnUpdateTime = new QToolButton(page_3);
        btnUpdateTime->setObjectName(QString::fromUtf8("btnUpdateTime"));

        gridLayout->addWidget(btnUpdateTime, 0, 2, 1, 1);

        label_4 = new QLabel(page_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        leActivityName = new QLineEdit(page_3);
        leActivityName->setObjectName(QString::fromUtf8("leActivityName"));

        gridLayout->addWidget(leActivityName, 1, 1, 1, 2);


        verticalLayout_5->addLayout(gridLayout);

        groupBox = new QGroupBox(page_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 0, 5, -1);
        rbActivityTask = new QRadioButton(groupBox);
        rbActivityTask->setObjectName(QString::fromUtf8("rbActivityTask"));
        rbActivityTask->setChecked(true);

        horizontalLayout_3->addWidget(rbActivityTask);

        rbActivityOther = new QRadioButton(groupBox);
        rbActivityOther->setObjectName(QString::fromUtf8("rbActivityOther"));

        horizontalLayout_3->addWidget(rbActivityOther);


        verticalLayout_5->addWidget(groupBox);

        btnNewActivity = new QPushButton(page_3);
        btnNewActivity->setObjectName(QString::fromUtf8("btnNewActivity"));

        verticalLayout_5->addWidget(btnNewActivity);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_5 = new QLabel(page_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_5);

        lvLastActivities = new QListView(page_3);
        lvLastActivities->setObjectName(QString::fromUtf8("lvLastActivities"));

        verticalLayout_4->addWidget(lvLastActivities);


        verticalLayout_5->addLayout(verticalLayout_4);

        stackedWidget->addWidget(page_3);

        horizontalLayout->addWidget(stackedWidget);

        TabletMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TabletMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 31));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        TabletMain->setMenuBar(menubar);
        statusbar = new QStatusBar(TabletMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TabletMain->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionRestore);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);

        retranslateUi(TabletMain);
        QObject::connect(btnAddActivity, SIGNAL(clicked()), TabletMain, SLOT(slot_SetFocusAddActivity()));
        QObject::connect(btnChrono, SIGNAL(clicked()), TabletMain, SLOT(slot_SetFocusChrono()));
        QObject::connect(btnTasks, SIGNAL(clicked()), TabletMain, SLOT(slot_SetFocusTasks()));
        QObject::connect(btnAddChild, SIGNAL(clicked()), TabletMain, SLOT(slot_TaskAddChild()));
        QObject::connect(btnAddSibling, SIGNAL(clicked()), TabletMain, SLOT(slot_TaskAddSibling()));
        QObject::connect(btnDeleteTask, SIGNAL(clicked()), TabletMain, SLOT(slot_TaskDelete()));
        QObject::connect(actionRestore, SIGNAL(activated()), TabletMain, SLOT(slot_Restore()));
        QObject::connect(actionSave, SIGNAL(activated()), TabletMain, SLOT(slot_Save()));
        QObject::connect(actionExit, SIGNAL(activated()), TabletMain, SLOT(close()));

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TabletMain);
    } // setupUi

    void retranslateUi(QMainWindow *TabletMain)
    {
        TabletMain->setWindowTitle(QApplication::translate("TabletMain", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionRestore->setText(QApplication::translate("TabletMain", "Restore", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("TabletMain", "Save", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("TabletMain", "Exit", 0, QApplication::UnicodeUTF8));
        btnTasks->setText(QApplication::translate("TabletMain", "Tasks", 0, QApplication::UnicodeUTF8));
        btnChrono->setText(QApplication::translate("TabletMain", "Chrono", 0, QApplication::UnicodeUTF8));
        btnAddActivity->setText(QApplication::translate("TabletMain", "Add activity", 0, QApplication::UnicodeUTF8));
        btnAddChild->setText(QApplication::translate("TabletMain", "Child", 0, QApplication::UnicodeUTF8));
        btnAddSibling->setText(QApplication::translate("TabletMain", "Sibling", 0, QApplication::UnicodeUTF8));
        btnDeleteTask->setText(QApplication::translate("TabletMain", "Delete", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TabletMain", "Current activity", 0, QApplication::UnicodeUTF8));
        lblCurrentActivity->setText(QApplication::translate("TabletMain", "No activity", 0, QApplication::UnicodeUTF8));
        btnToTasks->setText(QApplication::translate("TabletMain", "...", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TabletMain", "Started", 0, QApplication::UnicodeUTF8));
        lblActivityStarted->setText(QApplication::translate("TabletMain", "----.--.-- --:--", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TabletMain", "Activities", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TabletMain", "Start time", 0, QApplication::UnicodeUTF8));
        teActivityStartTime->setDisplayFormat(QApplication::translate("TabletMain", "yyyy.MM.dd H:mm", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TabletMain", "Name", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TabletMain", "Type", 0, QApplication::UnicodeUTF8));
        rbActivityTask->setText(QApplication::translate("TabletMain", "Task activity", 0, QApplication::UnicodeUTF8));
        rbActivityOther->setText(QApplication::translate("TabletMain", "Separate activity", 0, QApplication::UnicodeUTF8));
        btnNewActivity->setText(QApplication::translate("TabletMain", "New activity", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TabletMain", "Last activities", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("TabletMain", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TabletMain: public Ui_TabletMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLETMAIN_H
