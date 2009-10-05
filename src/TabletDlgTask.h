/*
 * TabletDlgTask.h
 *
 *  Created on: 05.10.2009
 *      Author: breeze
 */

#ifndef TABLETDLGTASK_H_
#define TABLETDLGTASK_H_

#include <QDialog>

#include "ui_TabletDlgTask.h"

class TabletDlgTask : public QDialog
{
	Q_OBJECT

public:
	TabletDlgTask( QWidget *parent = NULL );

protected:
	Ui::TabletDlgTask	ui;
};

#endif /* TABLETDLGTASK_H_ */
