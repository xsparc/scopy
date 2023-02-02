/*
 * Copyright (c) 2019 Analog Devices Inc.
 *
 * This file is part of Scopy
 * (see http://www.github.com/analogdevicesinc/scopy).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "toolmenu.h"

#include <QDebug>

using namespace adiscope;

ToolMenu::ToolMenu(QWidget *parent):
	BaseMenu(parent)
{
	buttonGroup = new QButtonGroup(this);

	connect(this, &ToolMenu::itemMovedFromTo,
		this, &ToolMenu::_updateToolList);
}


ToolMenuItem* ToolMenu::getToolMenuItemFor(QString toolId)
{
	for(auto &&tool:tools) {
		if(tool->getId() == toolId)
			return tool;
	}
	return nullptr;
}
ToolMenuItem* ToolMenu::addTool(QString id, QString name, QString icon, int position)
{
	ToolMenuItem *t = new ToolMenuItem(id,name,icon,this);
	insertMenuItem(t,position);
	if(position == -1) {
		tools.append(t);
	} else {
		tools.insert(position, t);
	}
	buttonGroup->addButton(t->getToolBtn());

	connect(t->getToolBtn(), &QPushButton::clicked, this, [=](){
		Q_EMIT toolSelected(t->getId());
	});

	connect(t, &ToolMenuItem::detach, this, [=](){
		Q_EMIT detach(t->getId());
	});


	connect(t, &ToolMenuItem::toggleButtonGroup, this, [=](bool detached) {
		if (detached) {
			buttonGroup->removeButton(t->getToolBtn());
		} else {
			buttonGroup->addButton(t->getToolBtn());
		}
	});

	return t;
}

bool ToolMenu::removeTool(QString id)
{
	for(int i = 0;i<tools.size();i++) {
		if(tools[i]->getId() == id) {
			delete tools[i];
			tools.remove(i);
			return true;
		}
	}
	return false;
}

bool ToolMenu::removeTool(ToolMenuItem *tmi)
{
	for(int i = 0;i<tools.size();i++) {
		if(tools[i] == tmi) {
			delete tools[i];
			tools.remove(i);
			return true;
		}
	}
	return false;
}

const QVector<ToolMenuItem *> &ToolMenu::getTools() const
{
	return tools;
}

QButtonGroup *ToolMenu::getButtonGroup() const
{
	return buttonGroup;
}

void ToolMenu::_updateToolList(short from, short to)
{
	if (d_items == tools.size()) {
		auto toMove = tools[from];
		tools.remove(from);
		tools.insert(to, toMove);
	}
}

ToolMenu::~ToolMenu()
{
	_saveState();
}

void ToolMenu::_saveState()
{
//	QSettings settings;

//	settings.beginWriteArray("toolMenu/pos");
//	for (int i = 0; i < d_tools.size(); ++i) {
//		settings.setArrayIndex(i);
//		settings.setValue("idx", QVariant(d_tools[i].second));
//	}
//	settings.endArray();
}

void ToolMenu::_loadState()
{
//	QSettings settings;

//	int n = settings.beginReadArray("toolMenu/pos");
//	for (int i = 0; i < n; ++i) {
//		settings.setArrayIndex(i);
//		d_positions.push_back(settings.value("idx").value<int>());
//	}
//	settings.endArray();
}



