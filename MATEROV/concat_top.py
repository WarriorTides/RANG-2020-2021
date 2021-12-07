# import required libraries
from vidgear.gears import NetGear
from vidgear.gears import VideoGear
import cv2
import numpy as np
#camera_index = [0, 4, 8, 19]
camera_index = [0]
streams = []
for i in range(0, len(camera_index)):
    streams.append(cv2.VideoCapture(camera_index[i]))
#for stream in streams:
#    stream.set(cv2.CAP_PROP_FPS, 3);
# define tweak flags
print("set all streams")
options = {"flag": 0, "copy": False, "track": False, "bidirectional_mode": True}

# Define Netgear Client at given IP address and define parameters 
# !!! change following IP address '192.168.x.xxx' with yours !!!
server = NetGear(
    address="10.78.18.6",
    port="5454",
    protocol="tcp",
    pattern=0,
    logging=True,
    **options
)

# loop over until KeyBoard Interrupted
while True:
    try:
        # read frames from stream
        print("reading from stream")
        for i in range(0, len(camera_index)):
            print("getting frame", i)
            (grabbed, frame) = streams[i].read()
            if frame is None:
               print("None")
               continue
            recv = server.send(frame, message="camera "+str(i))
            if not (recv is None):
                print(recv)
           


    except KeyboardInterrupt:
        break

# safely close video stream
for i in streams:
    i.release()
# safely close server
server.close()

