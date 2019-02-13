#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

#include <QJsonObject>
#include <QJsonDocument>

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();
    void sendJsonObj(QTcpSocket * socket, QJsonObject json_res_obj);
    void receiveJsonObj(QTcpSocket * socket, QJsonObject * json_req_obj);

private:
    QTcpServer *server;
};

#endif // TCPSERVER_H
