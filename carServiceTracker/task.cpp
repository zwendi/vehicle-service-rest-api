#include "stdafx.h"
#include "task.h"

#include "restColumn.h"

//task json return key


bool task::setName(string& name)
{
	if (name.size() < 1)
		return false;
	taskName = name;
	return true;
}


bool task::setPrice(double price)
{
	if (price < 0)
		return false;
	taskPrice = price;
	return true;

}

bool task::setDescription(string& description)
{
	if (description.size() < 1)
		return false;
	taskDescription = description;
	return true;
}

json::value task::toJson()
{
	json::value object;
	if (taskName.size() < 1 || taskPrice <0)
		return object;
	object[jsonName] = web::json::value(utility::conversions::to_utf16string(getName()));
	object[jsonDescription] = web::json::value(utility::conversions::to_utf16string(getDescription()));
	object[jsonPrice] = web::json::value(getPrice());
	return object;
}
