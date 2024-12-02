#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include "bookmark.hpp"
class MyServer : public QTcpServer {
    Q_OBJECT

public:
    MyServer(QObject *parent = nullptr) : QTcpServer(parent) {}
    vbookbridge *cvbook;
protected:
    void incomingConnection(qintptr socketDescriptor) override {
        QTcpSocket *clientSocket = new QTcpSocket(this);
        clientSocket->setSocketDescriptor(socketDescriptor);
        connect(clientSocket, &QTcpSocket::readyRead, [this, clientSocket]() {
            QDataStream in(clientSocket);
            QString request;
            in >> request;
            switch(request.toInt()){
                case 1:{
                    if(cvbook->ifgoing){cvbook->bookregis();};
                }
                break;
            }
            clientSocket->disconnectFromHost();
        });

        connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    }
};