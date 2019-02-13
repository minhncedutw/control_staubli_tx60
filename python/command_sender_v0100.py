import socket
import json
import numpy as np
from time import sleep
from enum import Enum

PI = 3.14
def DEGREE_2_RADIAN(X):
    return (X * PI / 180)
def RADIAN_2_DEGREE(X):
    return (X*180/PI)
def METER_2_MILIMETER(X):
    return (X*1000)
def MILIMETER_2_METER(X):
    return (X/1000.0)

# eThreeFingerModes = Enum(['BASIC', 'PINCH', 'WIDE', 'SCISSOR'])
class ThreeFingerModes(Enum):
    BASIC = 0
    PINCH = 1
    WIDE = 2
    SCISSOR = 3

host = socket.gethostname()
port = 1234                   # The same port as used by the server

host_3finger = socket.gethostname()
port_3finger = 2345                   # The same port as used by the server

def send_command(command):
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(command)
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    return response


def communicate(command):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    return response

def ROBOT_enable():
    command = {"cmd": "enable"}
    print('Sent: ', command)
    # sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock.connect((host, port))
    # sock.sendall(json.dumps(command).encode('utf-8'))
    # response = sock.recv(1024)
    # sock.close()
    response = communicate(command=command)
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status


def ROBOT_disable():
    command = {"cmd": "disable"}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status


def ROBOT_move_point(x, y, z, rx, ry, rz):
    command = {"cmd": "move_point", "x": x, "y": y, "z": z, "rx": rx, "ry": ry, "rz": rz}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status


def move_to_joints(J1, J2, J3, J4, J5, J6):
    command = {"cmd": "move_joints", "J1": J1, "J2": J2, "J3": J3, "J4": J4, "J5": J5, "J6": J6}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status


def get_joints():
    command = {"cmd": "get_joints"}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    J1 = json.loads(response.decode('utf-8'))['J1']
    J2 = json.loads(response.decode('utf-8'))['J2']
    J3 = json.loads(response.decode('utf-8'))['J3']
    J4 = json.loads(response.decode('utf-8'))['J4']
    J5 = json.loads(response.decode('utf-8'))['J5']
    J6 = json.loads(response.decode('utf-8'))['J6']
    return response_status, [J1, J2, J3, J4, J5, J6]


def ROBOT_get_point():
    command = {"cmd": "get_point"}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    x = json.loads(response.decode('utf-8'))['x']
    y = json.loads(response.decode('utf-8'))['y']
    z = json.loads(response.decode('utf-8'))['z']
    rx = json.loads(response.decode('utf-8'))['rx']
    ry = json.loads(response.decode('utf-8'))['ry']
    rz = json.loads(response.decode('utf-8'))['rz']
    return response_status, [x, y, z, rx, ry, rz]

def rotate(original_point, rotated_center=[0, 0], angle_deg=None, angle_rad=None):
    # input angle_deg is still not correct calculating
    if angle_rad is not None:
        _angle = angle_rad
    else:
        if angle_deg is not None:
            _angle = angle_deg / 180.0 * np.pi
        else:
            return False, []
    origin = np.array([original_point[0], original_point[1], 1])
    first = np.array([[1, 0, -rotated_center[0]], [0, 1, -rotated_center[1]], [0, 0, 1]])
    second = np.array([[np.cos(_angle), -np.sin(_angle), 0], [np.sin(_angle), np.cos(_angle), 0], [0, 0, 1]])
    third = np.array([[1, 0, rotated_center[0]], [0, 1, rotated_center[1]], [0, 0, 1]])
    # rotated_point = np.dot(np.dot(np.dot(third, second), first), origin)
    rotated_point = np.dot(third, np.dot(second, np.dot(first, origin)))
    return True, [rotated_point[0], rotated_point[1]]

def interpolate(column, row):
    _regulated_coord = rotate([column, row], [320, 240], angle_rad=-np.arctan((10.0 / 218.5)))[1]
    x, y = (_regulated_coord - np.array([187.745, 149.952])) * np.array([-1, 1]) * \
                 np.array([(303.5 - 75) / (452.209 - 187.745), (339 - 191) / (329.049 - 149.952)]) + \
                 np.array([303.5, 191])
    return 'OK', [x, y]

def ROBOT_reset_point():
    _, [_, _, _, _rx, _ry, _rz] = ROBOT_get_point()
    ROBOT_move_point(-200, 345, 60, _rx, _ry, _rz)
    sleep(1)
    ROBOT_move_point(-200, 345, -60, _rx, _ry, _rz)
    return 'OK', []

def ROBOT_move_dimension(x=None, y=None, z=None, rx=None, ry=None, rz=None):
    if x is None and y is None and z is None and rx is None and ry is None and rz is None:
        return None, []
    elif z is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(_x, _y, z, _rx, _ry, _rz)
    elif x is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(x, _y, _z, _rx, _ry, _rz)
    elif y is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(_x, y, _z, _rx, _ry, _rz)
    elif rx is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(_x, _y, _z, rx, _ry, _rz)
    elif ry is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(_x, _y, _z, _rx, ry, _rz)
    elif rz is not None:
        _, [_x, _y, _z, _rx, _ry, _rz] = ROBOT_get_point()
        ROBOT_move_point(_x, _y, _z, _rx, _ry, rz)
    return 'OK', []

def THREEFINGER_enable():
    command = {"cmd": "enable"}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host_3finger, port_3finger))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status

def THREEFINGER_disable():
    command = {"cmd": "disable"}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host_3finger, port_3finger))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status

def THREEFINGER_mode(mode=ThreeFingerModes.BASIC):
    command = {"cmd": "mode", "mode": mode}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host_3finger, port_3finger))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status

def THREEFINGER_move(position, speed=22, force=15):
    command = {"cmd": "move", "position": position, "speed": speed, "force": force}
    print('Sent: ', command)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host_3finger, port_3finger))
    sock.sendall(json.dumps(command).encode('utf-8'))
    response = sock.recv(1024)
    sock.close()
    print('Received: ', response)
    response_status = json.loads(response.decode('utf-8'))['ret']
    return response_status

res = ROBOT_enable()

# moving point equation
# (np.array([320, 192]) - np.array([192, 335])) * np.array([-1, 1]) * np.array([214/64/4, 162/48/4]) + np.array([294, 332])
# (np.array([448, 335]) - np.array([192, 144])) * np.array([-1, 1]) * np.array([(303.5-75)/64/4, (339-191)/48/4]) + np.array([303.5, 191])


# original = [0; 2; 1];
# first = [1,0, -320; 0, 1, -240; 0, 0, 1];
# angle = -atan(10/218.5)
# second = [cos(angle), -sin(angle),0; sin(angle), cos(angle), 0; 0, 0, 1];
# third = [1,0, 320; 0, 1, 240; 0, 0, 1];
# rotated = third * second * first * original

# (np.array([443.477, 138.248]) - np.array([187.745, 149.952])) * np.array([-1, 1]) * np.array([(303.5-75)/(452.209-187.745), (339-191)/(329.049-149.952)]) + np.array([303.5, 191])
# (rotate([448, 144], [320, 240], angle_rad=-np.arctan((10.0/218.5)))[1] - np.array([187.745, 149.952])) * np.array([-1, 1]) * np.array([(303.5-75)/(452.209-187.745), (339-191)/(329.049-149.952)]) + np.array([303.5, 191])

