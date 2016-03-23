#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

#include <gtkmm.h>
#include <json/json.h>
#include <string>
#include <stdlib.h>
class Gui : public Gtk::Window
{
public:
	Gui(Json::Value root);
	virtual ~Gui();

protected:
	//Signal handlers:
	void on_button_quit();
	void select_game();
	void on_row_activate(const Gtk::TreePath& treepath, Gtk::TreeViewColumn* column);
	//Tree model columns:
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:

		ModelColumns()
		{ add(m_col_id); add(m_col_name); add(m_col_system); }

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<Glib::ustring> m_col_system;
	};
	
	ModelColumns m_Columns;
	
	//Child widgets:
	Json::Value root;
	Gtk::Box m_VBox;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button m_Button_Select;
	Gtk::Button m_Button_Quit;
	
};

#endif //GTKMM_GUI_H
