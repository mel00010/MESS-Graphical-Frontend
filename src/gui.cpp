#include <iostream>
#include <string>
#include "gui.h"


Gui::Gui(std::string configFile)
: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_Button_Quit("Quit")
{
	TreeView *m_TreeView = new TreeView(configFile);
	set_title("Game Selection");
	set_border_width(5);
	set_default_size(815, 600);

	add(m_VBox);

	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
	m_ScrolledWindow.add(*m_TreeView);

	//Only show the scrollbars when they are necessary:
	m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_VBox.pack_start(m_ScrolledWindow);
	m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

	m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
	m_ButtonBox.set_border_width(5);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
	m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
			&Gui::on_button_quit) );

	show_all_children();
}

Gui::~Gui()
{
}

void Gui::on_button_quit()
{
	exit(0);
}

