#ifndef BOOKBRIDGE_H
#define BOOKBRIDGE_H
#include <chrono>
#include <ctime>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <qpushbutton.h>
#include <obs.h>
#include <QFileDialog>
#include <QInputDialog>
#include <fstream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QTime>
#include <QTimeEdit>
#include <QStyle>
#include <QMessageBox>
class vbookbridge : public QWidget {
    Q_OBJECT
public:
    vbookbridge(QWidget* parent = nullptr);
    ~vbookbridge();
    void bookregis();
    void startimer();
    void saveb();
    bool ifgoing = false, ifsaved = false;
private:
    std::chrono::system_clock::time_point basetime;
    QFormLayout* booklay = new QFormLayout;
    QLabel* timestampsl = new QLabel("Time Stamps"), * namestampsl = new QLabel("Stamps Names");
    QWidget *parent = nullptr;
    QWidget* stamwid = new QWidget;
    QVBoxLayout* mlay = new QVBoxLayout;
private slots:
    void clearb();
	void delbr();
};
#endif