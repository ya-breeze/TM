#include "diagrammdialog.h"
#include "ui_diagrammdialog.h"

#include "utils.h"
#include "diagramm/ConvertorFromSeconds.h"

DiagrammDialog::DiagrammDialog(Activities &_acts, const TaskTree& _tasks, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagrammDialog),
    m_Acts(_acts),
    m_Tasks(_tasks),
    p_LastActs(new LastActs(m_Tasks, m_Acts, this))
{
    ui->setupUi(this);
    p_Diagramm = new Diagramm(this);
    p_Diagramm->setConvertor( new ConvertorFromSeconds() );
    ui->horizontalLayout->addWidget(p_Diagramm);
    p_Diagramm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(ui->calendar, SIGNAL(selectionChanged()), this, SLOT(onDateSelected()));

	// Activities
	ui->listView->setModel( p_LastActs );

	onDateSelected();
}

DiagrammDialog::~DiagrammDialog()
{
    delete ui;
}

void DiagrammDialog::onDateSelected() {
    QDate day = ui->calendar->selectedDate();
    DEBUG("Date selected: " + day.toString());

    DayActivities acts = m_Acts.getDay( day );
    if( !acts.count() ) {
        DEBUG("There are no any activities");
        return;
    }

    DEBUG("There are " << acts.count() << " activities");
    DiagrammItemList list;
    typedef QMap<QUuid, DiagrammItem> ItemMap;
    ItemMap items;

    for(size_t i=0; i<acts.count(); ++i) {
        // Посчитаем длительность. Для последней активности длительность неопределена
        Activity act = acts.getActivity(i);
        int condur = -1;
        if( i!=acts.count()-1 ) {
            Activity next = acts.getActivity(i+1);
            condur = act.getStartTime().secsTo( next.getStartTime() );
        }
        DEBUG(i << " - " << act.getName() << ":" << condur);

        if( condur>0 ) {
            Task *task = m_Tasks.getItem(act.getAssignedTask());
            ItemMap::iterator it = items.find(task->getId());
            if( it==items.end() ) {
                DiagrammItem item(task->getName(), task->getId(), condur);
                item.setParent(task->getParentId());
                items[task->getId()] = item;

                // Add parents
                Task *curTask = task;
                while( !curTask->getParentId().isNull() ) {
                    curTask = m_Tasks.getItem(curTask->getParentId());

                    if( items.find(curTask->getId())==items.end() ) {
                        DiagrammItem item(curTask->getName(), curTask->getId(), 0);
                        item.setParent(curTask->getParentId());
                        items[curTask->getId()] = item;
                    }
                }
            } else {
                it->setValue(it->getValue() + condur);
            }
        }
    }

    foreach (DiagrammItem item, items.values())
        list.append(item);

    p_Diagramm->setItems(list);

    p_LastActs->todayChanged(day);


//    int total = 0;
//    QMap< QPair<QString, QString/*deep*/>, int/*condur*/ > times;
//    std::cout << "---------------- " << cld.dateTime().date().toString("yyyy.MM.dd") << " ------------------" << std::endl;
//    for(size_t i=0; i<acts.count(); ++i) {
//    // Посчитаем длительность. Для последней активности длительность неопределена
//    Activity act = acts.getActivity(i);
//    int condur = -1;
//    if( i!=acts.count()-1 ) {
//        Activity next = acts.getActivity(i+1);
//        condur = act.getStartTime().secsTo( next.getStartTime() );
//    }

//	// Получим полное название задачи - включая родителей
//	Task *task = m_Tasks.getItem(act.getAssignedTask());
//	QString fullTaskName;
//	while( task ) {
//		if( !fullTaskName.isEmpty() )
//		fullTaskName = ":" + fullTaskName;
//		fullTaskName = task->getName() + fullTaskName;

//            // Длительность родительских задач
//            QPair<QString, QString> key = qMakePair(fullName(task), deep2Spaces(task));
//            //QString name = deep2Spaces(task) + task->getName();
//            //QString name = deep2Spaces(task);// + fullTaskName;
//            if( !times.contains(key) )
//        times[ key ] = 0;
//            if( condur!=-1 )
//        times[key] += condur;

//		if( !task->getParentId().isNull() )
//		task = m_Tasks.getItem( task->getParentId() );
//		else
//		task = NULL;
//	}
}
