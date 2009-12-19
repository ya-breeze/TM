/*
 * ChangableObject.h
 *
 *  Created on: 22.08.2009
 *      Author: breeze
 */

#ifndef CHANGABLEOBJECT_H_
#define CHANGABLEOBJECT_H_

/// \brief Класс для работы с изменяемыми объектами
class ChangableObject
{
	bool has_Changed;

public:
	ChangableObject()
		: has_Changed(false) {};
	void setChanged(bool _value = true )
	{
		has_Changed = _value;
	};
	bool hasChanged() const
	{
		return has_Changed;
	};
};

#endif /* CHANGABLEOBJECT_H_ */
