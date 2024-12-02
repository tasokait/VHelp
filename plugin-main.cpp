/*
Plugin Name
Copyright (C) <Year> <Developer> <Email Address>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <plugin-support.h>
#include <obs-frontend-api.h>
#include <QWidget>
#include <QSharedMemory>
#include <QMessageBox>
#include <thread>
#include <iostream>
#include "MyServer.hpp"
#include "bookmark.hpp"
vbookbridge *cvbook;
MyServer* server;
void bookeventfilter(enum obs_frontend_event event, void *private_data){
    switch(event){
        case OBS_FRONTEND_EVENT_RECORDING_STARTED:
        case OBS_FRONTEND_EVENT_STREAMING_STARTED:
            cvbook->startimer();
			cvbook->ifgoing = true;
            break;
		case OBS_FRONTEND_EVENT_EXIT:{
		        QSharedMemory* sharedMemory;
    			sharedMemory = new QSharedMemory;
    			sharedMemory->setNativeKey("OBSifRemind");

    		// Attach to the shared memory segment
    		if (!sharedMemory->attach(QSharedMemory::ReadOnly)) {
        		return;
    		}

    		// Read the boolean value from shared memory
    		sharedMemory->lock();
    		bool* from = static_cast<bool*>(sharedMemory->data());
    		bool valueRead = *from;
    		sharedMemory->unlock();
    		sharedMemory->detach();
			if((cvbook->findChildren<QLineEdit*>().size() > 0) && valueRead && !(cvbook->ifsaved)){
				QMessageBox userbox = QMessageBox(QMessageBox::Warning, "TimeStamp Save", "You Marked TimeStamps and did not saved them, Do you want to save them?", QMessageBox::Save | QMessageBox::Cancel);
				userbox.setDefaultButton(QMessageBox::Save);
				int getuserin = userbox.exec();
				if(getuserin == QMessageBox::Save){
					cvbook->saveb();
				}
			}
		}
			break;
		case OBS_FRONTEND_EVENT_RECORDING_STOPPING:
        case OBS_FRONTEND_EVENT_STREAMING_STOPPING:
			cvbook->ifgoing = false;
			break;
    }
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")
bool obs_module_load(void){
	QWidget* main_window = (QWidget*)obs_frontend_get_main_window();
	cvbook = new vbookbridge(main_window);
	char vbid[] = "vbtime";
	char vbtitle[] = "VHelp Bookmark Timestamp";
	obs_frontend_add_dock_by_id(vbid,vbtitle,cvbook);
	obs_frontend_add_event_callback(bookeventfilter,cvbook);
    //std::thread(vbookthread);
    server = new MyServer;
    server->cvbook = cvbook;
    if (!server->listen(QHostAddress::LocalHost, 9998)) {
        obs_log(LOG_INFO, "Could not start server",PLUGIN_VERSION);
        return false;
    }
	obs_log(LOG_INFO, "plugin loaded successfully (version %s)",PLUGIN_VERSION);
	/*cvbook->bookregis();
	cvbook->bookregis();
	cvbook->bookregis();
	cvbook->bookregis();*/
	return true;
}
void obs_module_unload(void){
	obs_frontend_remove_event_callback(bookeventfilter,cvbook);
	delete server;
	obs_log(LOG_INFO, "plugin unloaded");
}