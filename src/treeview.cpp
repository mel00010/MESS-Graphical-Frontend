#include "treeview.h"
#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <stdlib.h>

TreeView::TreeView(Json::Value json)
{
	root = json;
	//Create the Tree model:
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	set_model(m_refTreeModel);
	//Fill the TreeView's model
	
	int i = 1;
	Gtk::TreeModel::Row row;
	for (Json::Value::iterator itr = root["games"].begin(); itr != root["games"].end(); itr++)
	{
		Json::Value game = (*itr);
		row = *(m_refTreeModel->append());
		row[m_Columns.m_col_id] = i;
		row[m_Columns.m_col_name] = game["name"].asString();
		//~ row[m_Columns.m_col_system] = game["system"].asString();
		i++;
	}
	//~ set_headers_clickable(true);
	//Add the TreeView's view columns:
	append_column("ID", m_Columns.m_col_id);
	append_column("Name", m_Columns.m_col_name);
	auto pColumn = get_column(0);
	if(pColumn)
	{
		pColumn->set_sort_column(m_Columns.m_col_id);
	}
	auto qColumn = get_column(1);
	if(qColumn)
	{
		qColumn->set_sort_column(m_Columns.m_col_name);
	}
	//~ append_column("System", m_Columns.m_col_system);
	//~ m_refTreeModel[m_Columns.m_col_system].set_fixed_width(50);

}
bool TreeView::on_button_press_event(GdkEventButton* button_event)
{
	bool return_value = false;

	//Call base class, to allow normal handling,
	//such as allowing the row to be selected by the right-click:
	return_value = Gtk::TreeView::on_button_press_event(button_event);

	//Then do our custom stuff:
	if (button_event->type == GDK_2BUTTON_PRESS) 
	{
		select_game();
	}
	return return_value;
}
void TreeView::select_game()
{
	auto refSelection = get_selection();
	if(refSelection)
	{
		Gtk::TreeModel::iterator iter = refSelection->get_selected();
		if(iter)
		{
			int id = (*iter)[m_Columns.m_col_id] -1;
			SNES snes(root, id);
			//~ std::cout << id << std::endl;
		}
	}	
}
bool TreeView::on_key_release_event(GdkEventKey* event)
{
	bool return_value = false;

	//Call base class, to allow normal handling,
	//such as allowing the row to be selected by the right-click:
	//~ return_value Gtk::TreeView::on_key_release_event(event);

	//Then do our custom stuff:
	if (event->keyval == GDK_KEY_Return) 
	{
		select_game();
	}
	if (event->keyval == GDK_KEY_Escape)
	{
		exit(0);
	}
	return return_value;
}
TreeView::~TreeView()
{
}
