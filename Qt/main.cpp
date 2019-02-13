#include <QCoreApplication>
#include "TX60L.h"
#include <string>
#include <fstream>
#include <iostream>

#include <QThread>
#include "tcpserver.h"

#define PI 3.14159265
#define SQR(x) ((x)*(x))
#define SUM(x, y, z, r, s, t) ((x) + (y) + (z) + (r) + (s) + (t))

using namespace std;

class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};

int enable(TX60L * robot);
bool disable(TX60L * robot);
bool isEnabled();
void showPoint(std::vector<double> position);
bool getPoint(TX60L * robot, std::vector<double> * position);
bool movePoint(TX60L * robot, double x, double y, double z, double rx, double ry, double rz);
bool reset(TX60L * robot);

TX60L * mRobot;
bool mRobotEnabled = false;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    mRobot = new TX60L;

    int ret;
    ret = enable(mRobot);
    if (ret <= 0) return -1;

    // Test Robot
    std::vector<double> position;
    std::vector<int> idontknow;
    getPoint(mRobot, &position);
    showPoint(position);
    movePoint(mRobot, -400, 40, 60, -180, 0, 0);
    getPoint(mRobot, &position);
    showPoint(position);
    disable(mRobot);

    // Start the Server for communication
    TCPServer server;

    return a.exec();
}

void TCPServer::newConnection()
{
    qDebug() << "\n===========================================";
    QTcpSocket * socket = server->nextPendingConnection();

    // Get request json
    QJsonObject json_obj_request;
    receiveJsonObj(socket, &json_obj_request);

    // Analyse json to get command
    QString cmd = json_obj_request.value("cmd").toString();
    qDebug() << "Command:" << cmd;

    if (cmd == "enable") {
        // Execute command
        enable(mRobot);
//        movePoint(mRobot, -200, 300, 60, -180, 0, 0);
//        movePoint(mRobot, -200, 300, -60, -180, 0, 0);

        // Create response command
        QJsonObject json_obj_response;
        json_obj_response["ret"] = "OK";
        // Send command
        sendJsonObj(socket, json_obj_response);

    } else if (cmd == "disable") {
        // Execute command
        disable(mRobot);

        // Create response command
        QJsonObject json_obj_response;
        json_obj_response["ret"] = "OK";
        // Send command
        sendJsonObj(socket, json_obj_response);

    } else if (cmd == "get_point") {
        // Execute command
        std::vector<double> current_pos;
        getPoint(mRobot, &current_pos);

        // Create response command
        QJsonObject json_obj_response;
        json_obj_response["ret"] = "OK";
        json_obj_response["x"] = METER_2_MILIMETER(current_pos[0]);
        json_obj_response["y"] = METER_2_MILIMETER(current_pos[1]);
        json_obj_response["z"] = METER_2_MILIMETER(current_pos[2]);
        json_obj_response["rx"] = RADIAN_2_DEGREE(current_pos[3]);
        json_obj_response["ry"] = RADIAN_2_DEGREE(current_pos[4]);
        json_obj_response["rz"] = RADIAN_2_DEGREE(current_pos[5]);
        // Send command
        sendJsonObj(socket, json_obj_response);

    } else if (cmd == "move_point") {
        // Execute command
        double x = json_obj_request.value("x").toDouble();
        double y = json_obj_request.value("y").toDouble();
        double z = json_obj_request.value("z").toDouble();
        double rx = json_obj_request.value("rx").toDouble();
        double ry = json_obj_request.value("ry").toDouble();
        double rz = json_obj_request.value("rz").toDouble();

        movePoint(mRobot, x, y, z, rx, ry, rz);

        // Create response command
        QJsonObject json_obj_response;
        json_obj_response["ret"] = "OK";
        // Send command
        sendJsonObj(socket, json_obj_response);

    }
    else { // If uncorrect command, respond Wrong
        // Create response command
        QJsonObject json_obj_response;
        json_obj_response["ret"] = "Uncorrect Command";
        // Send command
        sendJsonObj(socket, json_obj_response);
    }
}

int enable(TX60L * robot)
{
    int ret;
//    if (!robot->IsLoggedIn())
    if (!isEnabled())
    {
//        ret = robot->Login("http://127.0.0.1:5653/", "default", "");
//        ret = robot->Login("http://172.31.0.3:5653/", "default", ""); // config ip address: 172.31.0.10; subnet mask: 255.255.0.0
        ret = robot->Login("http://192.168.0.252:5653/", "default", ""); // config ip address: 192.168.0.254; subnet mask: 255.255.0.0
//        ret = robot->Login("http://10.123.0.250/", "default", "");
        if (ret > 0)
        {
            Sleeper::sleep(1);
            robot->ResetMotion();
            robot->Power(true);
            mRobotEnabled = true;
        }
    }

    return ret;
}

bool disable(TX60L * robot)
{
    if (!isEnabled()) return false;

    robot->Power(false);
    robot->Logoff();
    mRobotEnabled = false;

    return true;
}

bool isEnabled()
{
    return mRobotEnabled;
}

void showPoint(std::vector<double> position)
{
    std::cout << "\n Current cartesian pos: ";
    for (int i = 0; i <6; i++){
        if (i < 3) std::cout << METER_2_MILIMETER((position)[i]) << " ";
        else std::cout << RADIAN_2_DEGREE((position)[i]) << " ";
    }
}

bool getPoint(TX60L * robot, std::vector<double> * position)
{
    if (!isEnabled()) return false;

    robot->GetRobotCartesianPosition((*position));

    return true;
}

bool isArrived(TX60L * robot, double x, double y, double z, double rx, double ry, double rz) {
    std::vector<double> current_pos;
    getPoint(robot, &current_pos);
    bool arrived = (abs(METER_2_MILIMETER((current_pos)[0]) - x) < 1) &&
            (abs(METER_2_MILIMETER((current_pos)[1]) - y) < 1) &&
            (abs(METER_2_MILIMETER((current_pos)[2]) - z) < 1); // &&
            (abs(RADIAN_2_DEGREE((current_pos)[3]) - rx) < 1) &&
            (abs(RADIAN_2_DEGREE((current_pos)[4]) - ry) < 1) &&
            (abs(RADIAN_2_DEGREE((current_pos)[5]) - rz) < 1);

    return arrived;
}

//void waitUntilFinishMoving(TX60L * robot, double x, double y, double z, double rx, double ry, double rz)
//{
//    std::vector<double> current_pos;

//    double last_error=1000000, current_error;
//    do {
//        getPoint(robot, &current_pos);
//        current_error = SUM((SQR(METER_2_MILIMETER(current_pos[0]) - x)),
//                SQR(METER_2_MILIMETER(current_pos[1]) - y),
//                SQR(METER_2_MILIMETER(current_pos[2]) - z),
//                SQR(RADIAN_2_DEGREE(current_pos[3]) - rx),
//                SQR(RADIAN_2_DEGREE(current_pos[4]) - ry),
//                SQR(RADIAN_2_DEGREE(current_pos[5]) - rz));
//        printf("ERROR: %f %f", last_error, current_error);
//        if (last_error < current_error) last_error = current_error;
//        else break;
//    } while (1);
//}

bool movePoint(TX60L * robot, double x, double y, double z, double rx, double ry, double rz)
{
    if (!isEnabled()) return false;

    // Configure for moving
    std::vector<double> position_request;
    position_request.clear();
    position_request.push_back(MILIMETER_2_METER(x));
    position_request.push_back(MILIMETER_2_METER(y));
    position_request.push_back(MILIMETER_2_METER(z));
    position_request.push_back(DEGREE_2_RADIAN(rx));
    position_request.push_back(DEGREE_2_RADIAN(ry));
    position_request.push_back(DEGREE_2_RADIAN(rz));
    robot->Move(position_request);
    // Wait until finish moving
    while (!isArrived(robot, x, y, z, rx, ry, rz)) {}
//    waitUntilFinishMoving(robot, x, y, z, rx, ry, rz);

    return true;
}

bool reset(TX60L * robot)
{
    if (!isEnabled()) return false;

    robot->ResetMotion();

    return true;
}
