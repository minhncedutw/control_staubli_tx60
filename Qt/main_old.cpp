#include <QCoreApplication>
#include "TX60L.h"
#include <iostream>
#include <algorithm>
#include <QThread>
#include "tcpserver.h"
#include <QJsonObject>
#include <QJsonDocument>

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

bool loginRobot(TX60L *robot) {
    bool ret;
//    if (!(robot->IsLoggedIn()))
    {
        //    bool ret = (*robot).Login("http://127.0.0.1:5653/", "default", "");
        ret = (*robot).Login("http://172.31.0.3:5653/", "default", "");
    //    bool ret = (*robot).Login("http://192.168.0.100", "default", "");
        robot->ResetMotion();
        (*robot).Power(true);
        if (ret) {
            std::cout << "Login successfully." << std::endl;
        } else {
            std::cout << "Login FAIL!!" << std::endl;
        }
    }

    return ret;
}

bool logoffRobot(TX60L * robot) {
    (*robot).Power(false);
    (*robot).Logoff();

    std::cout << "LogOFF successfully." << std::endl;

    return true;
}

bool getRobotJoints(TX60L * robot, std::vector<double> * joint_pos) {
    (*joint_pos).resize(6);
    (*robot).GetRobotJoints((*joint_pos));

    std::cout << "Current joint pos: ";
    for (int i = 0; i < 6; i++) {
        std::cout << RADIAN_2_DEGREE((*joint_pos)[i]) << " ";
    }
    std::cout << std::endl;

    return true;
}

bool getRobotPoint(TX60L * robot, std::vector<double> * cartesian_pos) {
    (*cartesian_pos).resize(6);
    (*robot).GetRobotCartesianPosition((*cartesian_pos));

    std::cout << "Current cartesian pos: ";
    for (int i = 0; i < 6; i++) {
        if (i < 3) std::cout << M_2_MM((*cartesian_pos)[i]) << " ";
        else std::cout << RADIAN_2_DEGREE((*cartesian_pos)[i]) << " ";
    }
    std::cout << std::endl;

    return true;
}

bool getRobot(TX60L * robot, std::vector<int> * cartesian_pos) {
    (*cartesian_pos).resize(6);
    (*robot).GetRobots((*cartesian_pos));

    std::cout << "Current pos: ";
    for (int i = 0; i < 6; i++) {
        std::cout << ((*cartesian_pos)[i]) << " ";
    }
    std::cout << std::endl;

    return true;
}

bool moveRobotJoints(TX60L * robot, double J1, double J2, double J3, double J4, double J5, double J6) {
    std::vector<double> expect_pos;
    expect_pos.clear();

    std::cout << "Request to move to Joint: " << J1 << " " << J2 << " " << J3 << " " << J4 << " " << J5 << " " << J6 << std::endl;
    expect_pos.push_back(DEGREE_2_RADIAN(J1));
    expect_pos.push_back(DEGREE_2_RADIAN(J2));
    expect_pos.push_back(DEGREE_2_RADIAN(J3));
    expect_pos.push_back(DEGREE_2_RADIAN(J4));
    expect_pos.push_back(DEGREE_2_RADIAN(J5));
    expect_pos.push_back(DEGREE_2_RADIAN(J6));

    (*robot).MoveJoints(expect_pos);

    Sleeper::msleep(1000);

    return true;
}

bool checkArrival(TX60L * robot, double x, double y, double z, double rx, double ry, double rz) {
    std::vector<double> current_pos;
    getRobotPoint(robot, &current_pos);
    bool arrived = (abs(M_2_MM((current_pos)[0]) - x) < 1) &&
            (abs(M_2_MM((current_pos)[1]) - y) < 1) &&
            (abs(M_2_MM((current_pos)[2]) - z) < 1); // &&
//            (abs(M_2_MM((current_pos)[3]) - rx) < 1) &&
//            (abs(M_2_MM((current_pos)[4]) - ry) < 1) &&
//            (abs(M_2_MM((current_pos)[5]) - rz) < 1);
    return arrived;
}

bool moveRobotPoint(TX60L * robot, double x, double y, double z, double rx, double ry, double rz) {
    std::vector<double> expect_pos;
    expect_pos.clear();

    std::cout << "Request to move to Cartesion: " << x << " " << y << " " << z << " " << rx << " " << ry << " " << rz << std::endl;
    expect_pos.push_back(MM_2_M(x));
    expect_pos.push_back(MM_2_M(y));
    expect_pos.push_back(MM_2_M(z));
    expect_pos.push_back(DEGREE_2_RADIAN(rx));
    expect_pos.push_back(DEGREE_2_RADIAN(ry));
    expect_pos.push_back(DEGREE_2_RADIAN(rz));

    (*robot).Move(expect_pos);

//    Sleeper::msleep(1000);
    while (!checkArrival(robot, x, y, z, rx, ry, rz)) {}

    return true;
}

void test1(TX60L * robot) {
    qDebug() << "============================================";
    qDebug() << "This is test1";
    std::vector<double> current_pos, expect_pos;
    std::vector<int> current_pos2;

    getRobotJoints(robot, &current_pos); // Get robot joint-position

    // Move each joint with 45, 0, 0, 0, 0, 0
//    if (abs(RADIAN_2_DEGREE(current_pos[0]) - 90) < 1) {
//        moveRobotJoints(robot, 0, 90, 90, 0, 0, 180);
//    }
//    else {
//        moveRobotJoints(robot, 90, 90, 90, 0, 0, 180);
//    }

//    getRobotJoints(robot, &current_pos); // Get robot joint-position
}

void test2(TX60L * robot) {
    qDebug() << "============================================";
    qDebug() << "This is test2";
    std::vector<double> current_pos, expect_pos;
    std::vector<double> current_pos2;

    getRobotPoint(robot, &current_pos);

//    if (abs(M_2_MM(current_pos[1]) - 225) < 1) {
//        moveRobotPoint(robot, (225), (35), (-290), (-180), (0), (0));
//    }
//    else {
//        moveRobotPoint(robot, (-35), (225), (-290), (-180), (0), (-90));
//    }

//    getRobotPoint(robot, &current_pos);
}

TX60L mRobot;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPServer server;
    std::vector<double> current_pos, expect_pos;
    std::vector<int> current_pos2;

    qDebug() << "This is testing... \n";

    loginRobot(&mRobot); // Log in robot

//    test1(&mRobot);
    test2(&mRobot);
    Sleeper::sleep(1);

    logoffRobot(&mRobot); // Logoff robot

    return a.exec();
}

void TCPServer::newConnection() {
    qDebug() << "===========================================";
    QTcpSocket *socket = server->nextPendingConnection();

    socket->waitForReadyRead(3000);
    qDebug() << "Reading:" << socket->bytesAvailable();
    QString received_data = socket->readAll();
    qDebug() << received_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(received_data.toUtf8());
    QJsonObject json_obj = json_doc.object();
    QString cmd = json_obj.value("cmd").toString();
//    qDebug() << json_doc;
//    qDebug() << json_obj;
    qDebug() << cmd;

    if (cmd == "login") {
        loginRobot(&mRobot);
        test2(&mRobot);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    if (cmd == "logoff") {
        logoffRobot(&mRobot);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    if (cmd == "get_joints") {
        std::vector<double> current_pos;
        getRobotJoints(&mRobot, &current_pos);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        json_res_obj["J1"] = RADIAN_2_DEGREE(current_pos[0]);
        json_res_obj["J2"] = RADIAN_2_DEGREE(current_pos[1]);
        json_res_obj["J3"] = RADIAN_2_DEGREE(current_pos[2]);
        json_res_obj["J4"] = RADIAN_2_DEGREE(current_pos[3]);
        json_res_obj["J5"] = RADIAN_2_DEGREE(current_pos[4]);
        json_res_obj["J6"] = RADIAN_2_DEGREE(current_pos[5]);
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    if (cmd == "get_point") {
        std::vector<double> current_pos;
        getRobotPoint(&mRobot, &current_pos);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        json_res_obj["x"] = M_2_MM(current_pos[0]);
        json_res_obj["y"] = M_2_MM(current_pos[1]);
        json_res_obj["z"] = M_2_MM(current_pos[2]);
        json_res_obj["rx"] = RADIAN_2_DEGREE(current_pos[3]);
        json_res_obj["ry"] = RADIAN_2_DEGREE(current_pos[4]);
        json_res_obj["rz"] = RADIAN_2_DEGREE(current_pos[5]);
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    if (cmd == "move_joints") {
        double J1 = json_obj.value("J1").toDouble();
        double J2 = json_obj.value("J2").toDouble();
        double J3 = json_obj.value("J3").toDouble();
        double J4 = json_obj.value("J4").toDouble();
        double J5 = json_obj.value("J5").toDouble();
        double J6 = json_obj.value("J6").toDouble();

        std::vector<double> expect_pos;
        moveRobotJoints(&mRobot, J1, J2, J3, J4, J5, J6);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    if (cmd == "move_point") {
        double x = json_obj.value("x").toDouble();
        double y = json_obj.value("y").toDouble();
        double z = json_obj.value("z").toDouble();
        double rx = json_obj.value("rx").toDouble();
        double ry = json_obj.value("ry").toDouble();
        double rz = json_obj.value("rz").toDouble();

        moveRobotPoint(&mRobot, x, y, z, rx, ry, rz);

        QJsonObject json_res_obj;
        json_res_obj["ret"] = "OK";
        QJsonDocument res_doc(json_res_obj);
        QString res_str(res_doc.toJson(QJsonDocument::Compact));
        qDebug() << res_str;

        socket->write(res_str.toUtf8());
        socket->flush();
        socket->waitForBytesWritten(3000);
    }

    socket->close();
}

// -50 330 190 0 -180 0 / 92.6 30.93 43.08 180 -105.99 -87.4
