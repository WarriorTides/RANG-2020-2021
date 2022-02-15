import PySimpleGUI as sg
import cv2
import numpy as np
import displayHelpers as dh
import partlen as pl

streams = []
vidStorage = []
vidNames = []
streamS = dh.list_portsNew()
numStreams = len(streamS)
cap = cv2.VideoCapture(-1)
for k, i in enumerate(streamS):
    cap = cv2.VideoCapture(i)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 10)
    streams.append(cap)
    #streams[k].set(CV_CAP_PROP_BUFFERSIZE, 3)
    vidStorage.append(0)
    vidNames.append("vid" + str(k))


def main():

    sg.theme('Black')

    # define the window layout
    layout = [[sg.Text('GUI', size=(40, 1), justification='center', font='Helvetica 20')],
    
     [      sg.Button('Exit', size=(10, 1), font='Helvetica 14'), sg.Button('Screenshot', size=(10, 1), font='Helvetica 14'), sg.Button('PartLen', size=(10, 1), font='Helvetica 14')],
              [sg.Image(filename='', key='vid0'),sg.Image(filename='', key='vid1')],
              [sg.Image(filename='', key='vid2'),sg.Image(filename='', key='vid3')],
             ]

    # create the window and show it without the plot
    window = sg.Window('MATE ROV',
                       layout, location=(0, 0))
  
    # ---===--- Event LOOP Read and display frames, operate the GUI --- #

    numPictures = 0
   
    while True:
        
        for numStream in range(0, numStreams):
            ret, vidStorage[numStream] = streams[numStream].read()
        event, values = window.read(timeout=0)
        
        for numStream in range(0, numStreams):
            scale_percent = 33 # percent of original size
            width = int(vidStorage[numStream].shape[1] * scale_percent / 100)
            height = int(vidStorage[numStream].shape[0] * scale_percent / 100)
            dim = (width, height)
            
            # resize image
            resized = cv2.resize(vidStorage[numStream], dim, interpolation = cv2.INTER_AREA)
            imgbytes = cv2.imencode('.png', resized)[1].tobytes()  # ditto
            window[vidNames[numStream]].update(data=imgbytes)
        if event == 'Exit' or event == sg.WIN_CLOSED:
            return
        if event == 'Screenshot':
            for i in range(0, numStreams):
                cv2.imwrite("Cam #" + str(i) + "Image #" + str(numPictures) + ".jpg", vidStorage[i])
                numPictures += 1
        #if event == 'PartLen':
            # for i in range(0, numStreams):
            #     cv2.imwrite("Cam #" + str(i) + "Image #" + str(numPictures) + ".jpg", vidStorage[i])
            #     numPictures += 1
            #     pl.part_len_main("C:/Users/ISS/Documents/MATEROV/Cam #" + str(i) + "Image #" + str(numPictures) + ".jpg")
 
main()