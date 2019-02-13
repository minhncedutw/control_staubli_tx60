'''
    File name: robot-grasping
    Author: minhnc
    Date created(MM/DD/YYYY): 3/16/2018
    Last modified(MM/DD/YYYY HH:MM): 3/16/2018 3:20 PM
    Python Version: 3.5
    Other modules: [tensorflow-gpu 1.3.0]

    Copyright = Copyright (C) 2017 of NGUYEN CONG MINH
    Credits = [None] # people who reported bug fixes, made suggestions, etc. but did not actually write the code
    License = None
    Version = 0.9.0.1
    Maintainer = [None]
    Email = minhnc.edu.tw@gmail.com
    Status = Prototype # "Prototype", "Development", or "Production"
    Code Style: http://web.archive.org/web/20111010053227/http://jaynes.colorado.edu/PythonGuidelines.html#module_formatting
'''

#==============================================================================
# Imported Modules
#==============================================================================
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import os.path
import sys
import time

import socket
import json
import numpy as np
from time import sleep
from enum import Enum

#==============================================================================
# Constant Definitions
#==============================================================================
PI = 3.14
def DEGREE_2_RADIAN(X):
    return (X * PI / 180)
def RADIAN_2_DEGREE(X):
    return (X*180/PI)
def METER_2_MILIMETER(X):
    return (X*1000)
def MILIMETER_2_METER(X):
    return (X/1000.0)


class TX60():
    """"""

    def __init__(self, host=socket.gethostname(), port=1234):
        """Constructor for TX60"""
        self.host = host
        self.port = port

    def _communicate(self, command):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((self.host, self.port))
        sock.sendall(json.dumps(command).encode('utf-8'))
        response = sock.recv(1024)
        sock.close()
        return response

    # @classmethod
    def enable(self):
        command = {"cmd": "enable"}
        print('Sent: ', command)
        response = self._communicate(command)
        print('Received: ', response)
        response_status = json.loads(response.decode('utf-8'))['ret']
        return response_status, []

    # @classmethod
    def disable(self):
        command = {"cmd": "disable"}
        print('Sent: ', command)
        response = self._communicate(command)
        print('Received: ', response)
        response_status = json.loads(response.decode('utf-8'))['ret']
        return response_status, []

    # @classmethod
    def move_point(self, x, y, z, rx, ry, rz):
        command = {"cmd": "move_point", "x": x, "y": y, "z": z, "rx": rx, "ry": ry, "rz": rz}
        print('Sent: ', command)
        response = self._communicate(command)
        print('Received: ', response)
        response_status = json.loads(response.decode('utf-8'))['ret']
        return response_status, []

    # @classmethod
    def get_point(self):
        command = {"cmd": "get_point"}
        print('Sent: ', command)
        response = self._communicate(command)
        print('Received: ', response)
        response_status = json.loads(response.decode('utf-8'))['ret']
        x = json.loads(response.decode('utf-8'))['x']
        y = json.loads(response.decode('utf-8'))['y']
        z = json.loads(response.decode('utf-8'))['z']
        rx = json.loads(response.decode('utf-8'))['rx']
        ry = json.loads(response.decode('utf-8'))['ry']
        rz = json.loads(response.decode('utf-8'))['rz']
        return response_status, [x, y, z, rx, ry, rz]

    # @classmethod
    def reset_point(self):
        _, [_, _, _, _rx, _ry, _rz] = self.get_point()
        self.move_dimension(z=60)
        self.move_dimension(x=-200)
        self.move_dimension(y=345)
        # self.move_dimension(z=-60)
        # self.move_point(-200, 345, 60, _rx, _ry, _rz)
        self.move_point(-200, 345, -60, -180, 0, 0)
        return 'OK', []

    # @classmethod
    def move_dimension(self, x=None, y=None, z=None, rx=None, ry=None, rz=None):
        if x is None and y is None and z is None and rx is None and ry is None and rz is None:
            return None, []
        elif z is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(_x, _y, z, _rx, _ry, _rz)
        elif x is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(x, _y, _z, _rx, _ry, _rz)
        elif y is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(_x, y, _z, _rx, _ry, _rz)
        elif rx is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(_x, _y, _z, rx, _ry, _rz)
        elif ry is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(_x, _y, _z, _rx, ry, _rz)
        elif rz is not None:
            _, [_x, _y, _z, _rx, _ry, _rz] = self.get_point()
            self.move_point(_x, _y, _z, _rx, _ry, rz)
        return 'OK', []

#==============================================================================
# Function Definitions
#==============================================================================

#==============================================================================
# Main function
#==============================================================================
def main(argv=None):
    print('Hello! This is TX60 Command Sender for Robot TX60 Program')

    tx60 = TX60()
    print(tx60.enable())
    print(tx60.get_point())
    print(tx60.move_dimension(z=60))
    print(tx60.get_point())
    print(tx60.move_dimension(z=-60))
    print(tx60.disable())


if __name__ == '__main__':
    main()
