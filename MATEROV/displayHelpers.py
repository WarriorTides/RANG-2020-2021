import cv2
def list_ports():
    is_working = True
    dev_port = 0
    working_ports = []
    while is_working:
        camera = cv2.VideoCapture(dev_port)
        if not camera.isOpened():
            is_working = False
        else:
            is_reading, img = camera.read()
            w = camera.get(3)
            h = camera.get(4)
            if is_reading:
                working_ports.append(dev_port)
        dev_port +=1
    return working_ports

def list_portsNew():
    cams = [1,2,3]
    ret = []
    for i in cams:
        ret.append("http://rpi" + str(i) + ".local:8000/stream.mjpg")
    return ret

if __name__ == "__main__":
    print(list_portsNew())