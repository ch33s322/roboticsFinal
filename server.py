#dakota Deets

import socket
import json
import time
import uuid


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

ROBOT_IP = "192.168.254.253"  #Arduino IP
PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(2)

while True:
    command = input(">>> ").strip().lower()

    if command == "exit":
        print("Goodbye!")
        break

    elif command == "task1":
            m = message("1.0", "command", "task1", "", "")
            
            # Send message
            print("Sending: task1")
            sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))

        

    elif command == ("task2"):
        m = message("1.0", "command", "task2", "", "")
            
        # Send message
        print("Sending: task2")
        sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))
    elif command == ("task3"):
        m = message("1.0", "command", "task3", "", "")
            
        # Send message
        print("Sending: task3")
        sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))
    elif command == ("task4"):
        m = message("1.0", "command", "task4", "", "")
            
        # Send message
        print("Sending: task4")
        sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))
    
    elif command == ("start"):
        m = message("1.0", "command", "start", "", "")
            
        # Send message
        print("Sending: start")
        sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))

    elif command == ("stop"):
        m = message("1.0", "command", "stop", "", "")
            
        # Send message
        print("Sending: stop")
        sock.sendto(json.dumps(JSONMessage(m)).encode(), (ROBOT_IP, PORT))

    elif command == "":
        continue  # ignore empty input

    else:
        print("Unknown command:", command)

    #receive 
    try:
        data, _ = sock.recvfrom(1024)
        response = json.loads(data.decode())

        print("Receive:")
        print(json.dumps(response, indent=4))

    except socket.timeout:
        print("No response from robot")