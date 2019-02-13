#include "tcpserver.h"

TCPServer::TCPServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "\nServer could not start!!";
    } else {
        qDebug() << "\nServer started.\nReady to receive commands...";
    }
}

void TCPServer::receiveJsonObj(QTcpSocket * socket, QJsonObject * json_obj_request)
{
    socket->waitForReadyRead(3000);
    qDebug() << "Reading:" << socket->bytesAvailable();
    QString received_data = socket->readAll();
    qDebug() << received_data;

    QJsonDocument json_doc_request = QJsonDocument::fromJson(received_data.toUtf8());
    *json_obj_request = json_doc_request.object();
}

void TCPServer::sendJsonObj(QTcpSocket * socket, QJsonObject json_res_obj)
{
    QJsonDocument res_doc(json_res_obj);
    QString res_str(res_doc.toJson(QJsonDocument::Compact));
    qDebug() << res_str;

    socket->write(res_str.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(3000);
}
