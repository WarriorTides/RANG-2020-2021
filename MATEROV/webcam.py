#!/usr/bin/env python
import PySimpleGUI as sg
import cv2
import numpy as np

"""
Demo program that displays a webcam using OpenCV
"""
#from vidgear.gears import NetGear
import cv2
stream = cv2.VideoCapture(0)
# define tweak flags
#options = {"flag": 0, "copy": False, "track": False, "bidirectional_mode": True}

# Define Netgear Client at given IP address and define parameters
# !!! change following IP address '192.168.x.xxx' with yours !!!
# client = NetGear(
#     address="10.78.18.6",
#     port="5454",
#     protocol="tcp",
#     pattern=0,
#     receive_mode=True,
#     logging=True,
#     **options
# )

def main():

    sg.theme('Black')

    # define the window layout
    layout = [[sg.Text('GUI', size=(40, 1), justification='center', font='Helvetica 20')],
    [
    sg.Slider(
        (25, 66),
        0,
        1,
        orientation="h",
        size=(40, 15),
        key="-SIZE SLIDER-",
    )],
     [sg.Button('4 Cameras', size=(10, 1), font='Helvetica 14'),
               sg.Button('Main Camera', size=(10, 1), font='Any 14'),
               sg.Button('Exit', size=(10, 1), font='Helvetica 14'), ],
              [sg.Image(filename='', key='image'),sg.Image(filename='', key='image2'),sg.Image(filename='', key='large')],
              [sg.Image(filename='', key='image3'),sg.Image(filename='', key='image4'),sg.Image(filename='', key='mini1'),sg.Image(filename='', key='mini2'),sg.Image(filename='', key='mini3')],
             ]

    # create the window and show it without the plot
    window = sg.Window('MATE ROV',
                       layout, location=(0, 0))
  
    # ---===--- Event LOOP Read and display frames, operate the GUI --- #
    
    recording = False
    recording_large = False
   
    while True:
        ret, img = stream.read()
        #ret, img = client.recv(return_data="yo")
        event, values = window.read(timeout=0)
        if event == 'Exit' or event == sg.WIN_CLOSED:
            return

        elif event == '4 Cameras':
            recording = True
            recording_large = False

        elif event == 'Main Camera':
            recording = False
            recording_large = True
            
            
        if recording_large:
            
            scale_percent = values["-SIZE SLIDER-"] # percent of original size
            if scale_percent >= 25 and scale_percent <= 66:
                width = int(img.shape[1] * scale_percent / 100)
                height = int(img.shape[0] * scale_percent / 100)
                dim = (width, height)
                dim2 = (width * 3, height * 3)
                resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
                resized2 = cv2.resize(img, dim2, interpolation = cv2.INTER_AREA)
                imgbytes = cv2.imencode('.png', resized)[1].tobytes()  # ditto
                imgbytes_large = cv2.imencode('.png', resized2)[1].tobytes() 
                window['image'].update("")
                window['image2'].update("")
                window['image3'].update("")
                window['image4'].update("")
                window['large'].update(data=imgbytes_large)
                window['mini1'].update(data=imgbytes)
                window['mini2'].update(data=imgbytes)
                window['mini3'].update(data=imgbytes)
        if recording:
            scale_percent = 60 # percent of original size
            width = int(img.shape[1] * scale_percent / 100)
            height = int(img.shape[0] * scale_percent / 100)
            dim = (width, height)
            
            # resize image
            resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
            imgbytes = cv2.imencode('.png', resized)[1].tobytes()  # ditto
            window['image'].update(data=imgbytes)
            window['image2'].update(data=imgbytes)
            window['image3'].update(data=imgbytes)
            window['image4'].update(data=imgbytes)
            window['large'].update("")
            window['mini1'].update("")
            window['mini2'].update("")
            window['mini3'].update("")


main()