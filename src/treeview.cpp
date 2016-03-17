#include "treeview.h"
#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <stdlib.h>

TreeView::TreeView()
{
	//Create the Tree model:
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	set_model(m_refTreeModel);

	//Fill the TreeView's model
	reader.parse(getFileContents("/home/mel/roms/config.json"),root);
	Json::Value games_ = root["games"];
	
	int i = 1;
	Gtk::TreeModel::Row row;
	for (Json::Value::iterator itr = games_.begin(); itr != games_.end(); itr++)
	{
		Json::Value game = (*itr);
		row = *(m_refTreeModel->append());
		row[m_Columns.m_col_id] = i;
		row[m_Columns.m_col_name] = game["name"].asString();
		//~ row[m_Columns.m_col_system] = game["system"].asString();
		i++;
	}
	//Add the TreeView's view columns:
	append_column("ID", m_Columns.m_col_id);
	append_column("Name", m_Columns.m_col_name);
	//~ append_column("System", m_Columns.m_col_system);
	//~ m_refTreeModel[m_Columns.m_col_system].set_fixed_width(50);

}

TreeView::~TreeView()
{
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
		auto refSelection = get_selection();
		if(refSelection)
		{
			Gtk::TreeModel::iterator iter = refSelection->get_selected();
			if(iter)
			{
				int id = (*iter)[m_Columns.m_col_id] -1;
				snes.load(root["games"][id]["save_path"].asString(),root["games"][id]["system"].asString());
				snes.run(root["games"][id]["path"].asString(), root["games"][id]["system"].asString());
				snes.save(root["games"][id]["save_path"].asString(), root["games"][id]["system"].asString());
				//~ std::cout << id << std::endl;
			}
		}
	}
	return return_value;
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
		auto refSelection = get_selection();
		if(refSelection)
		{
			Gtk::TreeModel::iterator iter = refSelection->get_selected();
			if(iter)
			{
				int id = (*iter)[m_Columns.m_col_id] -1;
				snes.load(root["games"][id]["save_path"].asString(),root["games"][id]["system"].asString());
				snes.run(root["games"][id]["path"].asString(), root["games"][id]["system"].asString());
				snes.save(root["games"][id]["save_path"].asString(), root["games"][id]["system"].asString());
				//~ std::cout << id << std::endl;
			}
		}
	}
	if (event->keyval == GDK_KEY_Escape)
	{
		exit(0);
	}
	return return_value;
}
std::string TreeView::getFileContents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}
