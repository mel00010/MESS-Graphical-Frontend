#ifndef GTKMM_TREEVIEW_H
#define GTKMM_TREEVIEW_H

#include <gtkmm.h>
#include "SNES.h"
#include <json/json.h>
#include <string>
#include <stdlib.h>

class TreeView : public Gtk::TreeView
{
public:
	TreeView();
	virtual ~TreeView();

protected:
	// Override Signal handler:
	// Alternatively, use signal_button_press_event().connect_notify()
	bool on_button_press_event(GdkEventButton* button_event) override;
	bool on_key_release_event(GdkEventKey* event) override;

	std::string getFileContents(const char *filename);
	Json::Value root;
	Json::Reader reader;

	SNES snes;
	
	//Tree model columns:
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ModelColumns()
		{ add(m_col_id); add(m_col_name);/* add(m_col_system); */}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		//~ Gtk::TreeModelColumn<Glib::ustring> m_col_system;
	};
	
	ModelColumns m_Columns;

	//The Tree model:
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

#endif //GTKMM_TREEVIEW_H
