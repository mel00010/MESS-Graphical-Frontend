#ifndef GTKMM_GUI_H
#define GTKMM_GUI_H

#include <gtkmm.h>
#include "treeview.h"

class Gui : public Gtk::Window
{
public:
	Gui(std::string configFile);
	virtual ~Gui();

protected:
	//Signal handlers:
	void on_button_quit();
	
 

	//Child widgets:
	Gtk::Box m_VBox;

	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::ButtonBox m_ButtonBox;
	Gtk::Button m_Button_Quit;
};

#endif //GTKMM_GUI_H
