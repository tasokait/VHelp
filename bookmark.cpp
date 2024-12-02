#include "bookmark.hpp"
#include <obs-module.h>
#include <obs-frontend-api.h>
vbookbridge::vbookbridge(QWidget* parent) : QWidget(parent){
    this->parent = parent;
    QScrollArea* stamparea = new QScrollArea;
    QHBoxLayout* blay = new QHBoxLayout;
    QGroupBox* cwidget = new QGroupBox;
    QPushButton* txtexp = new QPushButton("Export to subtitle"),*clrbt = new QPushButton("clear all timestamps");
    stamparea->setWidgetResizable(true);
    booklay->addRow(timestampsl,namestampsl);
    booklay->addWidget(txtexp);
    stamwid->setLayout(booklay);
    stamparea->setWidget(stamwid);
    blay->addWidget(txtexp);
    blay->addWidget(clrbt);
    cwidget->setLayout(blay);
    mlay->addWidget(stamparea);
    mlay->addWidget(cwidget);
    this->setLayout(mlay);
    connect(txtexp, &QPushButton::released, this, &vbookbridge::saveb);
    connect(clrbt, &QPushButton::released, this, &vbookbridge::clearb);
    this->setVisible(false);
}
void vbookbridge::startimer(){
    basetime = std::chrono::system_clock::now();
}
void vbookbridge::clearb(){
	QMessageBox userbox = QMessageBox(QMessageBox::Warning, "All timestamps about to be cleared", "You're about to clear all timestamps you've marked, Are you sure?", QMessageBox::Yes | QMessageBox::No);
	userbox.setDefaultButton(QMessageBox::No);
	int getuserin = userbox.exec();
	if(getuserin == QMessageBox::Yes){
		QList<QTimeEdit*> alltimes = stamwid->findChildren<QTimeEdit*>();
		for(int i = 0; i<alltimes.size(); i++){
        		delete alltimes[i];
    		}
		QList<QLineEdit*> alllines = stamwid->findChildren<QLineEdit*>();
		for(int i = 0; i<alllines.size(); i++){
			delete alllines[i];
		}
		QList<QPushButton*> allbuttons = stamwid->findChildren<QPushButton*>();
		for(int i = 0; i<allbuttons.size(); i++){
			delete allbuttons[i];
		}
    		ifsaved = true;
	}
}
void vbookbridge::delbr(){
	QMessageBox userbox = QMessageBox(QMessageBox::Warning, "one timestamp is about to be cleared", "You're about to clear a timestamp, Are you sure?", QMessageBox::Yes | QMessageBox::No);
	userbox.setDefaultButton(QMessageBox::No);
	int getuserin = userbox.exec();
	if(getuserin == QMessageBox::Yes){
		QPushButton* delb = qobject_cast<QPushButton*>(sender());
		QList<QHBoxLayout*> MT = stamwid->findChildren<QHBoxLayout*>();
		QList<QTimeEdit*> QTA = stamwid->findChildren<QTimeEdit*>();
		for(int i = 0; i<QTA.size(); i++){
			if(QTA[i]->property("order").toInt() == delb->property("order").toInt()){
				delete QTA[i];
				break;
			}
		}
		QList<QLineEdit*> QLA = stamwid->findChildren<QLineEdit*>();
		for(int i = 0; i<QLA.size(); i++){
			if(QLA[i]->property("order").toInt() == delb->property("order").toInt()){
				delete QLA[i];
				break;
			}
		}
		for(int i = 0; i<MT.size(); i++){
			if(MT[i]->property("order").toInt() == delb->property("order").toInt()){
				delete MT[i];
				break;
			}
		}
		delete delb;
	}
}
void vbookbridge::bookregis(){
    QTimeEdit* ctstamp = new QTimeEdit;
    QLineEdit* cnstamp = new QLineEdit;
	QPushButton* delb = new QPushButton();
	QHBoxLayout* ctimelay = new QHBoxLayout;
    int64_t hash = (intptr_t)delb;
    std::chrono::system_clock::time_point nowstamp = std::chrono::system_clock::now();
    delb->setIcon(style()->standardIcon(QStyle::SP_TrashIcon).pixmap(64, 64));
    auto difftime = nowstamp - basetime;
    int hours = std::chrono::duration_cast<std::chrono::hours>(difftime).count();
    int minutes = std::chrono::duration_cast<std::chrono::minutes>(difftime % std::chrono::hours(1)).count();
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(difftime % std::chrono::minutes(1)).count();
    delb->setProperty("order", hash);
    cnstamp->setProperty("order", hash);
    ctstamp->setProperty("order", hash);
    ctstamp->setDisplayFormat("hh:mm:ss");
    ctstamp->setTime(QTime(hours,minutes,seconds));
	ctimelay->addWidget(ctstamp);
	ctimelay->addWidget(cnstamp);
	ctimelay->addWidget(delb);
    booklay->insertRow(booklay->rowCount()-1, ctimelay);
    ctimelay->setProperty("order", hash);
    ifsaved = false;
    connect(delb, &QPushButton::released, this, &vbookbridge::delbr);
}
void vbookbridge::saveb(){
    QList<QLineEdit*> alllines = this->findChildren<QLineEdit*>();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Address"), "", tr("subtitle format (*.srt);;youtube subtitle format (*.sbv)"));
    bool isdurok = false;
	int dur;
	if(filename != ""){
    	dur = QInputDialog::getInt(this, "Set clip duration after the time stamp", "Duration: ",2,-2147483647,2147483647,1,&isdurok);
	}
    if(!isdurok){
        return;
    }
    std::ofstream subfile (filename.toStdString());
	QString fileformat = filename.last(3);
	if(fileformat == "sbv"){
    	for(int i =0; i< alllines.size();i++){
        	if((i%2)==0){
            	subfile << QTime::fromString(alllines[i]->text(),"hh:mm:ss").addSecs(-dur).toString("H:mm:ss").toStdString() + ".000," << QTime::fromString(alllines[i]->text(),"hh:mm:ss").addSecs(dur).toString("H:mm:ss").toStdString() + ".000"<< std::endl;
            	continue;
        	}
            subfile << alllines[i]->text().toStdString() << std::endl << std::endl;
	    }
	}else if(fileformat == "srt"){
    	for(int i =0; i< alllines.size();i++){
        	if((i%2)==0){
            	subfile << std::to_string((i/2)+1)<< std::endl << QTime::fromString(alllines[i]->text(),"hh:mm:ss").addSecs(-dur).toString("hh:mm:ss").toStdString() + ",000 --> " << QTime::fromString(alllines[i]->text(),"hh:mm:ss").addSecs(dur).toString("hh:mm:ss").toStdString() + ",000"<< std::endl;
            	continue;
        	}
        	subfile << alllines[i]->text().toStdString() << std::endl << std::endl;
	    }
	}
    subfile.close();
    ifsaved = true;
}
vbookbridge::~vbookbridge(){}