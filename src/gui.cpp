#include "gui.h"
#include "SNES.h"
#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <stdlib.h>

Gui::Gui(Json::Value json)
: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_Button_Select("Select"),
	m_Button_Quit("Quit")
	
{
	root = json;
	
	set_title("Game Selection");
	set_border_width(5);
	set_default_size(815, 600);

	add(m_VBox);

	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
	m_ScrolledWindow.add(m_TreeView);

	//Only show the scrollbars when they are necessary:
	m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_VBox.pack_start(m_ScrolledWindow);
	m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
	m_ButtonBox.set_spacing(5);
	m_ButtonBox.pack_start(m_Button_Select, Gtk::PACK_SHRINK);
	m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
	m_ButtonBox.set_border_width(5);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
	m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
			&Gui::on_button_quit) );
	m_Button_Select.signal_clicked().connect( sigc::mem_fun(*this,
			&Gui::select_game) );
	//Create the Tree model:
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	m_TreeView.set_model(m_refTreeModel);
	//Fill the TreeView's model
	
	int i = 1;
	Gtk::TreeModel::Row row;
	for (Json::Value::iterator itr = root["games"].begin(); itr != root["games"].end(); itr++)
	{
		Json::Value game = (*itr);
		row = *(m_refTreeModel->append());
		row[m_Columns.m_col_id] = i;
		row[m_Columns.m_col_name] = game["name"].asString();
		row[m_Columns.m_col_system] = game["system"].asString();
		i++;
	}
	//~ set_headers_clickable(true);
	//Add the TreeView's view columns:
	m_TreeView.append_column("ID", m_Columns.m_col_id);
	m_TreeView.append_column("Name", m_Columns.m_col_name);
	m_TreeView.append_column("System", m_Columns.m_col_system);
	m_TreeView.get_column(1)->set_expand(true);

	m_TreeView.signal_row_activated().connect(sigc::mem_fun(*this, 
        &Gui::on_row_activate) );
	for (int i = 0; i < m_refTreeModel->get_n_columns(); i++) {
		auto pColumn = m_TreeView.get_column(i);
		if(pColumn)
		{
			pColumn->set_sort_column(i);
		}
	}
	
	//~ append_column("System", m_Columns.m_col_system);
	//~ m_refTreeModel[m_Columns.m_col_system].set_fixed_width(50);
	show_all_children();
}

Gui::~Gui()
{
}

void Gui::on_button_quit()
{
	hide();
}
void Gui::select_game() 
{
	auto refSelection = m_TreeView.get_selection();
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
void Gui::on_row_activate(const Gtk::TreePath& treepath, Gtk::TreeViewColumn* column)
{
	select_game();
}

