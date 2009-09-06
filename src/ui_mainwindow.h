/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Sep 6 18:01:54 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
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
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TMClass
{
public:
    QAction *actionAdd;
    QAction *actionAddChild;
    QAction *actionDel;
    QAction *actionExit;
    QAction *actionRestore;
    QAction *actionSave;
    QAction *actionSync;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QTabWidget *tabMain;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QCheckBox *cbHideDone;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lblCurrentActivity;
    QToolButton *btnToTasks;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *lblActivityStarted;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout;
    QLabel *lblInterrupts;
    QToolButton *btnAddInterrupt;
    QFrame *line_2;
    QLabel *label_6;
    QListView *lvActivities;
    QToolBox *toolBox;
    QWidget *page;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *Notes;
    QGridLayout *gridLayout_2;
    QLabel *label_12;
    QLabel *lblCreateTime;
    QCheckBox *cbStartedTime;
    QDateTimeEdit *teStartTime;
    QLabel *label_11;
    QLabel *lblFinishTime;
    QLabel *label_7;
    QLineEdit *lePlannedTime;
    QLabel *label_8;
    QLabel *lblRealTimeBrutto;
    QLabel *label_10;
    QLabel *lblRealTimeNetto;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QDateTimeEdit *teActivityStartTime;
    QToolButton *btnUpdateTime;
    QLabel *label_4;
    QLineEdit *leActivityName;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *rbActivityTask;
    QRadioButton *rbActivityOther;
    QPushButton *btnNewActivity;
    QFrame *line;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_5;
    QListView *lvLastActivities;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuTasks;

    void setupUi(QMainWindow *TMClass)
    {
        if (TMClass->objectName().isEmpty())
            TMClass->setObjectName(QString::fromUtf8("TMClass"));
        TMClass->resize(727, 482);
        actionAdd = new QAction(TMClass);
        actionAdd->setObjectName(QString::fromUtf8("actionAdd"));
        actionAddChild = new QAction(TMClass);
        actionAddChild->setObjectName(QString::fromUtf8("actionAddChild"));
        actionDel = new QAction(TMClass);
        actionDel->setObjectName(QString::fromUtf8("actionDel"));
        actionExit = new QAction(TMClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionRestore = new QAction(TMClass);
        actionRestore->setObjectName(QString::fromUtf8("actionRestore"));
        actionSave = new QAction(TMClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSync = new QAction(TMClass);
        actionSync->setObjectName(QString::fromUtf8("actionSync"));
        centralwidget = new QWidget(TMClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_5 = new QVBoxLayout(centralwidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tabMain = new QTabWidget(splitter);
        tabMain->setObjectName(QString::fromUtf8("tabMain"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeView = new QTreeView(tab);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setDragEnabled(true);
        treeView->setDragDropMode(QAbstractItemView::DragDrop);
        treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeView->setAnimated(true);

        verticalLayout->addWidget(treeView);

        cbHideDone = new QCheckBox(tab);
        cbHideDone->setObjectName(QString::fromUtf8("cbHideDone"));

        verticalLayout->addWidget(cbHideDone);

        tabMain->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_6 = new QVBoxLayout(tab_2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lblCurrentActivity = new QLabel(tab_2);
        lblCurrentActivity->setObjectName(QString::fromUtf8("lblCurrentActivity"));

        horizontalLayout_3->addWidget(lblCurrentActivity);

        btnToTasks = new QToolButton(tab_2);
        btnToTasks->setObjectName(QString::fromUtf8("btnToTasks"));

        horizontalLayout_3->addWidget(btnToTasks);


        verticalLayout_6->addLayout(horizontalLayout_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        lblActivityStarted = new QLabel(tab_2);
        lblActivityStarted->setObjectName(QString::fromUtf8("lblActivityStarted"));

        gridLayout_3->addWidget(lblActivityStarted, 0, 1, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lblInterrupts = new QLabel(tab_2);
        lblInterrupts->setObjectName(QString::fromUtf8("lblInterrupts"));

        horizontalLayout->addWidget(lblInterrupts);

        btnAddInterrupt = new QToolButton(tab_2);
        btnAddInterrupt->setObjectName(QString::fromUtf8("btnAddInterrupt"));

        horizontalLayout->addWidget(btnAddInterrupt);


        gridLayout_3->addLayout(horizontalLayout, 1, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_3);

        line_2 = new QFrame(tab_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(line_2);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_6);

        lvActivities = new QListView(tab_2);
        lvActivities->setObjectName(QString::fromUtf8("lvActivities"));

        verticalLayout_6->addWidget(lvActivities);

        tabMain->addTab(tab_2, QString());
        splitter->addWidget(tabMain);
        toolBox = new QToolBox(splitter);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 368, 337));
        verticalLayout_3 = new QVBoxLayout(page);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        Notes = new QTextEdit(page);
        Notes->setObjectName(QString::fromUtf8("Notes"));

        verticalLayout_3->addWidget(Notes);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_12 = new QLabel(page);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 0, 0, 1, 1);

        lblCreateTime = new QLabel(page);
        lblCreateTime->setObjectName(QString::fromUtf8("lblCreateTime"));

        gridLayout_2->addWidget(lblCreateTime, 0, 1, 1, 1);

        cbStartedTime = new QCheckBox(page);
        cbStartedTime->setObjectName(QString::fromUtf8("cbStartedTime"));

        gridLayout_2->addWidget(cbStartedTime, 1, 0, 1, 1);

        teStartTime = new QDateTimeEdit(page);
        teStartTime->setObjectName(QString::fromUtf8("teStartTime"));

        gridLayout_2->addWidget(teStartTime, 1, 1, 1, 1);

        label_11 = new QLabel(page);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 2, 0, 1, 1);

        lblFinishTime = new QLabel(page);
        lblFinishTime->setObjectName(QString::fromUtf8("lblFinishTime"));

        gridLayout_2->addWidget(lblFinishTime, 2, 1, 1, 1);

        label_7 = new QLabel(page);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

        lePlannedTime = new QLineEdit(page);
        lePlannedTime->setObjectName(QString::fromUtf8("lePlannedTime"));

        gridLayout_2->addWidget(lePlannedTime, 3, 1, 1, 1);

        label_8 = new QLabel(page);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 4, 0, 1, 1);

        lblRealTimeBrutto = new QLabel(page);
        lblRealTimeBrutto->setObjectName(QString::fromUtf8("lblRealTimeBrutto"));
        lblRealTimeBrutto->setAutoFillBackground(false);
        lblRealTimeBrutto->setScaledContents(false);

        gridLayout_2->addWidget(lblRealTimeBrutto, 4, 1, 1, 1);

        label_10 = new QLabel(page);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 5, 0, 1, 1);

        lblRealTimeNetto = new QLabel(page);
        lblRealTimeNetto->setObjectName(QString::fromUtf8("lblRealTimeNetto"));

        gridLayout_2->addWidget(lblRealTimeNetto, 5, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        toolBox->addItem(page, QString::fromUtf8("Notes"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setGeometry(QRect(0, 0, 368, 337));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        teActivityStartTime = new QDateTimeEdit(page_2);
        teActivityStartTime->setObjectName(QString::fromUtf8("teActivityStartTime"));

        gridLayout->addWidget(teActivityStartTime, 0, 1, 1, 1);

        btnUpdateTime = new QToolButton(page_2);
        btnUpdateTime->setObjectName(QString::fromUtf8("btnUpdateTime"));

        gridLayout->addWidget(btnUpdateTime, 0, 2, 1, 1);

        label_4 = new QLabel(page_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        leActivityName = new QLineEdit(page_2);
        leActivityName->setObjectName(QString::fromUtf8("leActivityName"));

        gridLayout->addWidget(leActivityName, 1, 1, 1, 2);


        verticalLayout_2->addLayout(gridLayout);

        groupBox = new QGroupBox(page_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFlat(false);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, 0, 5, -1);
        rbActivityTask = new QRadioButton(groupBox);
        rbActivityTask->setObjectName(QString::fromUtf8("rbActivityTask"));
        rbActivityTask->setChecked(true);

        horizontalLayout_2->addWidget(rbActivityTask);

        rbActivityOther = new QRadioButton(groupBox);
        rbActivityOther->setObjectName(QString::fromUtf8("rbActivityOther"));

        horizontalLayout_2->addWidget(rbActivityOther);


        verticalLayout_2->addWidget(groupBox);

        btnNewActivity = new QPushButton(page_2);
        btnNewActivity->setObjectName(QString::fromUtf8("btnNewActivity"));

        verticalLayout_2->addWidget(btnNewActivity);

        line = new QFrame(page_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_5);

        lvLastActivities = new QListView(page_2);
        lvLastActivities->setObjectName(QString::fromUtf8("lvLastActivities"));

        verticalLayout_4->addWidget(lvLastActivities);


        verticalLayout_2->addLayout(verticalLayout_4);

        toolBox->addItem(page_2, QString::fromUtf8("Add activity"));
        splitter->addWidget(toolBox);

        verticalLayout_5->addWidget(splitter);

        TMClass->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(TMClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TMClass->setStatusBar(statusbar);
        menubar = new QMenuBar(TMClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 727, 31));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTasks = new QMenu(menubar);
        menuTasks->setObjectName(QString::fromUtf8("menuTasks"));
        TMClass->setMenuBar(menubar);
        QWidget::setTabOrder(treeView, tabMain);
        QWidget::setTabOrder(tabMain, rbActivityTask);
        QWidget::setTabOrder(rbActivityTask, rbActivityOther);
        QWidget::setTabOrder(rbActivityOther, leActivityName);
        QWidget::setTabOrder(leActivityName, btnNewActivity);
        QWidget::setTabOrder(btnNewActivity, teActivityStartTime);
        QWidget::setTabOrder(teActivityStartTime, Notes);
        QWidget::setTabOrder(Notes, btnToTasks);
        QWidget::setTabOrder(btnToTasks, lvActivities);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTasks->menuAction());
        menuFile->addAction(actionRestore);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionSync);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTasks->addAction(actionAdd);
        menuTasks->addAction(actionAddChild);
        menuTasks->addAction(actionDel);
        menuTasks->addSeparator();

        retranslateUi(TMClass);
        QObject::connect(actionExit, SIGNAL(activated()), TMClass, SLOT(close()));
        QObject::connect(cbStartedTime, SIGNAL(toggled(bool)), TMClass, SLOT(slot_SetStartTime()));
        QObject::connect(btnAddInterrupt, SIGNAL(clicked()), TMClass, SLOT(slot_AddInterrupt()));
        QObject::connect(actionSync, SIGNAL(activated()), TMClass, SLOT(slot_Sync()));

        tabMain->setCurrentIndex(0);
        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(TMClass);
    } // setupUi

    void retranslateUi(QMainWindow *TMClass)
    {
        TMClass->setWindowTitle(QApplication::translate("TMClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionAdd->setText(QApplication::translate("TMClass", "Add", 0, QApplication::UnicodeUTF8));
        actionAddChild->setText(QApplication::translate("TMClass", "AddChild", 0, QApplication::UnicodeUTF8));
        actionDel->setText(QApplication::translate("TMClass", "Del", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("TMClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionRestore->setText(QApplication::translate("TMClass", "Restore", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("TMClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("TMClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSync->setText(QApplication::translate("TMClass", "Sync", 0, QApplication::UnicodeUTF8));
        cbHideDone->setText(QApplication::translate("TMClass", "Hide done", 0, QApplication::UnicodeUTF8));
        tabMain->setTabText(tabMain->indexOf(tab), QApplication::translate("TMClass", "Tasks", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TMClass", "Current activity", 0, QApplication::UnicodeUTF8));
        lblCurrentActivity->setText(QApplication::translate("TMClass", "No activity", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnToTasks->setToolTip(QApplication::translate("TMClass", "Jump to task in task list", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("TMClass", "Started", 0, QApplication::UnicodeUTF8));
        lblActivityStarted->setText(QApplication::translate("TMClass", "----.--.-- --:--", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("TMClass", "Interrupts", 0, QApplication::UnicodeUTF8));
        lblInterrupts->setText(QApplication::translate("TMClass", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnAddInterrupt->setToolTip(QApplication::translate("TMClass", "Add interrupt", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("TMClass", "Activities", 0, QApplication::UnicodeUTF8));
        tabMain->setTabText(tabMain->indexOf(tab_2), QApplication::translate("TMClass", "Day activities", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("TMClass", "Create time", 0, QApplication::UnicodeUTF8));
        lblCreateTime->setText(QApplication::translate("TMClass", "<no available>", 0, QApplication::UnicodeUTF8));
        cbStartedTime->setText(QApplication::translate("TMClass", "Start time", 0, QApplication::UnicodeUTF8));
        teStartTime->setDisplayFormat(QApplication::translate("TMClass", "yyyy.MM.dd H:mm", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("TMClass", "Finish time", 0, QApplication::UnicodeUTF8));
        lblFinishTime->setText(QApplication::translate("TMClass", "<not yet finished>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TMClass", "Planned time", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("TMClass", "Real time brutto", 0, QApplication::UnicodeUTF8));
        lblRealTimeBrutto->setText(QApplication::translate("TMClass", "<no available>", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("TMClass", "Real time netto", 0, QApplication::UnicodeUTF8));
        lblRealTimeNetto->setText(QApplication::translate("TMClass", "<no available>", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("TMClass", "Notes", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TMClass", "Start time", 0, QApplication::UnicodeUTF8));
        teActivityStartTime->setDisplayFormat(QApplication::translate("TMClass", "yyyy.MM.dd H:mm", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnUpdateTime->setToolTip(QApplication::translate("TMClass", "Change time to current", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("TMClass", "Name", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TMClass", "Type", 0, QApplication::UnicodeUTF8));
        rbActivityTask->setText(QApplication::translate("TMClass", "Task activity", 0, QApplication::UnicodeUTF8));
        rbActivityOther->setText(QApplication::translate("TMClass", "Separate activity", 0, QApplication::UnicodeUTF8));
        btnNewActivity->setText(QApplication::translate("TMClass", "New activity", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TMClass", "Last activities", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("TMClass", "Add activity", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("TMClass", "File", 0, QApplication::UnicodeUTF8));
        menuTasks->setTitle(QApplication::translate("TMClass", "Tasks", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TMClass: public Ui_TMClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
