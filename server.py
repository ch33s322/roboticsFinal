#dakota Deets

import socket
import json
import time
import uuid
import os
import queue
import threading

width = 20
height = 20

empty = " . "
path = " * "
robot = " @ "

robotX = 0
robotY = 0



visited = {(robotX, robotY)}

def clear():
    os.system("cls" if os.name == "nt" else "clear")

def updateRobotPos(x, y):
    global robotX, robotY
    robotX += x
    robotY += y
    visited.add((robotX, robotY))

def drawGrid():
    #clear()
    global robotX, robotY, empty, path, robot, width, height

    print(robotX)
    print(robotY)

    for row in range(height):
        
        line = ""

        for col in range(width):
            if (col, row) == (robotX, robotY):
                line += robot
            elif (col, row) in visited:
                line += path
            else:
                line += empty

        print(line)

class message:
    def __init__(self, version, msgType, command, params, data):
        self.version = version
        self.msgID = str(uuid.uuid4())
        self.timestamp = int(time.time())
        self.msgType = msgType
        self.command = command
        self.params = params
        self.data = data

def JSONMessage(m):
    msg = {
    "version": m.version,
    "msgID": m.msgID,
    "timestamp": m.timestamp,
    "type": m.msgType,
    "command": m.command,
    "params": m.params
    }
    return msg

ROBOT_IP = "10.32.11.204"  #Arduino IP
PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(2)

command_queue = queue.Queue()

def input_thread():
    while True:
        cmd = input(">>> ").strip().lower()
        command_queue.put(cmd)

# Start input thread
threading.Thread(target=input_thread, daemon=True).start()

running = True
while running:
    while not command_queue.empty():
        command = command_queue.get()

        if command == "exit":
            print("Goodbye!")
            running = False
            break

        elif command in ["task1", "task2", "task3", "task4", "task5", "task6", "start", "stop", "w", "s", "a", "d"]:

            if(command == "stop"):
                visited.clear()
                robotX = 1
                robotY = 1

            m = message("1.0", "command", command, "", "")

            print(f"Sending: {command}")

            sock.sendto(
                json.dumps(JSONMessage(m)).encode(),
                (ROBOT_IP, PORT)
            )

        elif command == "":
            continue

        else:
            print("Unknown command:", command)

    # -------------------------
    # NON-BLOCKING RECEIVE
    # -------------------------
    try:
        data, _ = sock.recvfrom(1024)

        response = json.loads(data.decode())

        print("\nReceive:")
        print(json.dumps(response, indent=4))

        #if update recieved, update map
        if(response["type"] == "update"):
            updateRobotPos(response["data"]["dx"], response["data"]["dy"])
            drawGrid()
        
        

    except socket.timeout:
        pass

    # Prevent 100% CPU usage
    time.sleep(0.01)