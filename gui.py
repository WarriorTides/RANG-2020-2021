import pygame
import serial
global fine
import time


def startup():
    
    # Define some colors.
    BLACK = pygame.Color('black')
    WHITE = pygame.Color('white')
    moving1 = False
    moving2 = False
    moving3 = False
    moving4 = False
    fine = False
    

    
    ser = serial.Serial('COM5')  # open serial port()
    # time.sleep(5)
    for i in range(5):
        # ser.write("  x".encode())
        val = ser.readline()
        val = str(val[0:len(val)].decode("utf-8"))
        print(val)

    def message(msg):
        # if ser.is_open == False:
        #      ser.open()
        ser.write(str(msg).encode())  # send the serial message
        
        # ser.close()
        print("Sending " + msg)
        if ser.in_waiting:
            val = ser.readline()
            val = str(val[0:len(val)].decode("utf-8"))
            print("Recieving " + val)
            
        #pass
        



        


    # This is a simple class that will help us print to the screen.
    # It has nothing to do with the joysticks, just outputting the
    # information.
    class TextPrint(object):
        def __init__(self):
            self.reset()
            self.font = pygame.font.Font(None, 20)

        def tprint(self, screen, textString):
            textBitmap = self.font.render(textString, True, BLACK)
            screen.blit(textBitmap, (self.x, self.y))
            self.y += self.line_height

        def reset(self):
            self.x = 10
            self.y = 10
            self.line_height = 15

        def indent(self):
            self.x += 10

        def unindent(self):
            self.x -= 10


    pygame.init()
    user_textM = ""
    base_font = pygame.font.Font(None,18)
    input_rectM = pygame.Rect(1000,200,140,32)
    activeM = False
    
    color = pygame.Color('lightskyblue3')

    # Set the width and height of the screen (width, height).
    screen = pygame.display.set_mode((1200, 800))

    pygame.display.set_caption("My Game")
    
    # Loop until the user clicks the close button.
    done = False

    # Used to manage how fast the screen updates.
    clock = pygame.time.Clock()

    # Initialize the joysticks.
    pygame.joystick.init()

    # Get ready to print.
    textPrint = TextPrint()
    #print("ready")
    #print("wait for arduino")
    fineMode = False
    fineMultiplier = 0.35
    controlling_cam = 1
    controlling_camera_one = True
    claw_locked = False
    claw_send = 0
    clawR_locked = False
    clawR_send = 0
    # while True:
    #     if ser.in_waiting:
    #         break
    #print("arduino ready")
    stopped = [False] * 10
    # -------- Main Program Loop -----------

                
    
    while not done:
        
        #
        # EVENT PROCESSING STEP
        #
        # Possible joystick actions: JOYAXISMOTION, JOYBALLMOTION, JOYBUTTONDOWN,
        # JOYBUTTONUP, JOYHATMOTION
        basicfont = pygame.font.SysFont(None, 20)
        ExitButton = pygame.Rect(1000,600,100,50)
        CalcMButton = pygame.Rect(950,240,200,50)
        TLButton = pygame.Rect(950,340,200,50)
        SSButton = pygame.Rect(950,440,200,50)
        pygame.draw.rect(screen, (255,0,0), (150,550,100,50))
        for event in pygame.event.get(): # User did something.
            if event.type == pygame.QUIT: # If user clicked close.
                done = True # Flag that we are done so we exit this loop.
            elif event.type == pygame.JOYBUTTONDOWN:
                #print("Joystick button pressed.")
                pass
            elif event.type == pygame.JOYBUTTONUP:
                #print("Joystick button released.")
                pass

            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouse_pos = event.pos  # gets mouse position
                if ExitButton.collidepoint(mouse_pos):
                    # prints current location of mouse
                    print('button was pressed at {0}'.format(mouse_pos))
                    pygame.quit()
                if TLButton.collidepoint(mouse_pos):
                    print("Run Transect")
                if SSButton.collidepoint(mouse_pos):
                    print("Screenshot Camera Feed")
                if CalcMButton.collidepoint(mouse_pos):
                    try:
                        vals = str(user_textM).split(",")
                        dim1 = float(vals[0])
                        dim2 = float(vals[1])
                        numMussels = float(vals[2])
                        filtrationRate = float(vals[3])
                        totalNumMussels = dim1 * dim2 / 0.25  * numMussels
                        filtration = filtrationRate * totalNumMussels
                        print("Total number of mussels in the bed is " + str(int(totalNumMussels)))
                        print("Total amount of water filtered by the mussel bed in one hour is " + str(round(filtration, 2)))
                        user_textM = ""
                    except:
                        print("Error")
                        user_textM = ""
                    
                elif input_rectM.collidepoint(event.pos):
                    activeM = True
                elif input_rectM.collidepoint(event.pos) == False:
                    activeM = False
            elif event.type == pygame.KEYDOWN:
                if activeM == True:
                    if event.key == pygame.K_BACKSPACE:
                        user_textM = user_textM[:-1]
                    else:
                        user_textM += event.unicode


        #
        # DRAWING STEP
        #
        # First, clear the screen to white. Don't put other drawing commands
        # above this, or they will be erased with this command.
        screen.fill(WHITE)
        textPrint.reset()
        pygame.draw.rect(screen, [255, 0, 0], ExitButton)
        pygame.draw.rect(screen, [200, 200, 200], CalcMButton)
        pygame.draw.rect(screen, [200, 200, 200], TLButton)
        pygame.draw.rect(screen, [200, 200, 200], SSButton)
        textExit = basicfont.render("Exit", False, (0, 0, 0), None)
        
        textrectExit = textExit.get_rect()
        textrectExit.center = (1050, 625)
        screen.blit(textExit,textrectExit)
        textCalcM = basicfont.render("Calculate Mussels/fRate", True, (0, 0, 0), None)
        
        textRectCalcM = textCalcM.get_rect()
        textRectCalcM.center = (1050, 260)
        screen.blit(textCalcM,textRectCalcM)
        textCalcTL = basicfont.render("Run Transect Line", True, (0, 0, 0), None)
        
        textRectCalcTL = textCalcTL.get_rect()
        textRectCalcTL.center = (1050, 360)
        screen.blit(textCalcTL,textRectCalcTL)
        textCalcSS = basicfont.render("Screenshot Camera Feed", True, (0, 0, 0), None)
        
        textRectCalcSS = textCalcSS.get_rect()
        textRectCalcSS.center = (1050, 460)
        screen.blit(textCalcSS,textRectCalcSS)
        # Get count of joysticks.
        joystick_count = pygame.joystick.get_count()
        text_surface = base_font.render(user_textM, True,(0,0,0))
        pygame.draw.rect(screen,color,input_rectM,2)
        screen.blit(text_surface,(input_rectM.x + 5,input_rectM.y + 5))
        input_rectM.w = text_surface.get_width() + 10
        textCalc = basicfont.render("Enter vals (ex: dimX,dimY,numMussels,fRate)", True, (0, 0, 0), None)
        textrectCalc = textCalc.get_rect()
        textrectCalc.center = (1050, 175)
        screen.blit(textCalc,textrectCalc)
        textPrint.tprint(screen, "Number of joysticks: {}".format(joystick_count))
        textPrint.indent()
        # For each joystick:
        
        textTL = basicfont.render("Top Left Motor", True, (0, 0, 0), None)
        textrectTL = textTL.get_rect()
        textrectTL.center = (290, 260)
        screen.blit(textTL,textrectTL)
        textTR = basicfont.render("Top Right Motor", True, (0, 0, 0), None)
        textrectTR = textTR.get_rect()
        textrectTR.center = (440, 260)
        screen.blit(textTR,textrectTR)
        textML = basicfont.render("Middle Left Motors", True, (0, 0, 0), None)
        textrectML = textML.get_rect()
        textrectML.center = (290, 460)
        screen.blit(textML,textrectML)
        textMR = basicfont.render("Middle Right Motors", True, (0, 0, 0), None)
        textrectMR = textMR.get_rect()
        textrectMR.center = (440, 460)
        screen.blit(textMR,textrectMR)
        textBL = basicfont.render("Bottom Left Motor", True, (0, 0, 0), None)
        textrectBL = textBL.get_rect()
        textrectBL.center = (290, 660)
        screen.blit(textBL,textrectBL)
        textBR = basicfont.render("Bottom Right Motor", True, (0, 0, 0), None)
        textrectBR = textBR.get_rect()
        textrectBR.center = (440, 660)
        screen.blit(textBR,textrectBR)

        pygame.draw.rect(screen, (0, 0, 0), (250, 100, 80, 150), 3)
        pygame.draw.rect(screen, (0, 0, 0), (400, 100, 80, 150), 3)
        pygame.draw.rect(screen, (0, 0, 0), (250, 300, 80, 150), 3)
        pygame.draw.rect(screen, (0, 0, 0), (400, 300, 80, 150), 3)
        pygame.draw.rect(screen, (0, 0, 0), (250, 500, 80, 150), 3)
        pygame.draw.rect(screen, (0, 0, 0), (400, 500, 80, 150), 3)
        
        #pygame.draw.rect(screen, (0, 153, 255), (251, 103, 77, 146))

        
        for i in range(joystick_count):
            joystick = pygame.joystick.Joystick(i)
            joystick.init()

            textPrint.tprint(screen, "Joystick {}".format(i))
            textPrint.indent()

            # Get the name from the OS for the controller/joystick.
            name = joystick.get_name()
            textPrint.tprint(screen, "Joystick name: {}".format(name))

            # Usually axis run in pairs, up/down for one, and left/right for
            # the other.
            axes = joystick.get_numaxes()
            textPrint.tprint(screen, "Number of axes: {}".format(axes))
            textPrint.indent()
            
            for i in range(axes):
                
                axis = joystick.get_axis(i)

                
                #textPrint.tprint(screen, "Axis {} value: {:>6.3f}".format(i, axis))
                #textPrint.tprint(screen, "[{} {:>6.3f}]".format(i, axis))
                if axis > -0.1 and axis < 0.1 and i == 1:
                    moving1 = False
                if axis > -0.1 and axis < 0.1 and i == 0:
                    moving0 = False
                if axis > -0.1 and axis < 0.1 and i == 4:
                    moving4 = False
                if axis > -0.1 and axis < 0.1 and i == 3:
                    moving3 = False
                if axis < -0.1 or axis > 0.1:
                    if i == 1:
                        
                        topRight = axis * -1
                        topLeft = axis * -1
                        botRight = axis * -1
                        botLeft = axis * -1
                        moving1 = True  
                        varHeightTR = -73 - (73 * axis * -1)   
                        varHeightTL = -73 - (73 * axis * -1)
                        varHeightBR = -73 - (73 * axis * -1)
                        varHeightBL = -73 - (73 * axis * -1)
                    if i == 0:
                        topRight = axis
                        topLeft = axis * -1
                        botRight = axis
                        botLeft = axis * -1
                        moving0 = True  
                        
                        varHeightTR = -73 - (73 * axis * -1)   
                        varHeightTL = -73 - (73 * axis)
                        varHeightBR = -73 - (73 * axis * -1)
                        varHeightBL = -73 - (73 * axis)
                    if i == 3:
                        midRight = axis * -1
                        midLeft = axis * -1
                        varHeightMR = -73 - (73 * axis * -1)   
                        varHeightML = -73 - (73 * axis * -1)
                        
                        moving3 = True
                    if i == 2:
                        topRight = axis * -1
                        topLeft = axis
                        botRight = axis
                        botLeft = axis * -1
                        moving4 = True
                        varHeightTR = -73 - (73 * axis)   
                        varHeightTL = -73 - (73 * axis * -1)
                        varHeightBR = -73 - (73 * axis * -1)
                        varHeightBL = -73 - (73 * axis)

                if moving1 == False and moving0 == False and moving4 == False:
                    varHeightTR = -73
                    varHeightTL = -73
                    varHeightBR = -73
                    varHeightBL = -73 
                    topRight = 0
                    topLeft = 0
                    botRight = 0
                    botLeft = 0
                if moving3 == False:
                    varHeightMR = -73
                    varHeightML = -73
                    midRight = 0
                    midLeft = 0
                
                if not fine:
                    
                    pygame.draw.rect(screen, (0, 153, 255), (251, 102, 77, abs(varHeightTR)))
                    pygame.draw.rect(screen, (0, 153, 255), (401, 102, 77, abs(varHeightTL)))
                    pygame.draw.rect(screen, (0, 153, 255), (251, 302, 77, abs(varHeightMR)))
                    pygame.draw.rect(screen, (0, 153, 255), (401, 302, 77, abs(varHeightML)))
                    pygame.draw.rect(screen, (0, 153, 255), (251, 502, 77, abs(varHeightBR)))
                    pygame.draw.rect(screen, (0, 153, 255), (401, 502, 77, abs(varHeightBL)))
                else:
                   
                    pygame.draw.rect(screen, (255, 0, 0), (251, 102, 77, abs(varHeightTR)))
                    pygame.draw.rect(screen, (255, 0, 0), (401, 102, 77, abs(varHeightTL)))
                    pygame.draw.rect(screen, (255, 0, 0), (251, 302, 77, abs(varHeightMR)))
                    pygame.draw.rect(screen, (255, 0, 0), (401, 302, 77, abs(varHeightML)))
                    pygame.draw.rect(screen, (255, 0, 0), (251, 502, 77, abs(varHeightBR)))
                    pygame.draw.rect(screen, (255, 0, 0), (401, 502, 77, abs(varHeightBL)))
                textPrint.tprint(screen, "Axis {} value: {:>6.3f}".format(i, axis))
                if i != 4 and i != 5:
                    if 0.3 > axis and axis > -0.3:
                        if not stopped[i]:
                            stopped[i] = True
                            message(str(i) + " 0" + "x")
                    else:
                        if i == 1 or i == 3:
                            axis *= -1
                        if fineMode:
                            axis *= fineMultiplier
                        axis = round(axis, 3)
                        message(str(i) + " " + str(axis) + "x")
                        stopped[i] = False
                if stopped == ([True] * 10):
                    message("9 0.0x")
                
                # if i == 4:
                #     if claw_locked:
                #         #print("send " + str(claw_send))
                #         axis = claw_send
                #     else:
                #         if axis < -0.9:
                #             claw_send = 0.0
                #             axis = 0.0
                #         else:
                #             claw_send = axis
                        #print("assigned " + str((axis + 1) / 2))
                        # axis = (axis + 1) / 2
                        
                # if i == 5:
                #     if clawR_locked:
                #         axis = 0.0
                #     else:
                #         if axis < -0.9:
                #             clawR_send = 0.0
                #             axis = 0.0
                #         else:
                #             clawR_send = axis
                #         print(axis)
                        #print("assigned " + str((axis + 1) / 2))
                
                
            textPrint.tprint(screen, "3x2:".format())
            textPrint.tprint(screen, "[{:>6.3f}] [{:>6.3f}]".format(topLeft, topRight))
            textPrint.tprint(screen, "[{:>6.3f}] [{:>6.3f}]".format(midLeft, midRight))
            textPrint.tprint(screen, "[{:>6.3f}] [{:>6.3f}]".format(botLeft, botRight))
            textPrint.unindent()
            #textPrint.tprint(screen, "2x2: {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f} {:>6.3f}".format(axis0, axis1, axis2, axis3, axis4))



            buttons = joystick.get_numbuttons()
            # ClawLockButton = 4
            # ClawRotateLockButton = 5
            if joystick.get_button(4) == 1:
                # claw_locked = not claw_locked
                message("4 1.07x")
                #print("Claw Locked")
            elif joystick.get_button(5) == 1:
                message("4 -1.07x")
                # clawR_locked = not clawR_locked
                #print("Claw Wrist Locked")
            elif joystick.get_button(1) == 1:
                message("5 1.0x")
                print("Claw Rotate Flip")
            elif joystick.get_button(0) == 1:
                message("5 -1.0x")
                print("Claw Rotate Flip")
            elif joystick.get_button(2) == 1:
                
                fineMode = not fineMode
                fine = not fine
                print('Fine Mode: ' + str(fine))
            # elif joystick.get_button(openClawButton) == 0 and joystick.get_button(closeClawButton) == 0:
            #     #print("Claw Stop")
            #     message("9" + " " + "0.0" + "x")
            textPrint.tprint(screen, "Number of buttons: {}".format(buttons))
            textPrint.indent()

            for i in range(buttons):
                button = joystick.get_button(i)
                textPrint.tprint(screen,
                                "Button {:>2} value: {}".format(i, button))
                
            textPrint.unindent()

            hats = joystick.get_numhats()
            textPrint.tprint(screen, "Number of hats: {}".format(hats))
            textPrint.indent()

            # Hat position. All or nothing for direction, not a float like
            # get_axis(). Position is a tuple of int values (x, y).
            for i in range(hats):
                hat = joystick.get_hat(i)
                if hat == (-1,0):
                    
                    print("Camera is moved to left")
                    message(str(5 + int(controlling_cam)) + " -1.0x")
                elif hat == (1,0):
                    print("Camera is moved to right")
                    message(str(5 + int(controlling_cam)) + " 1.875x")
                elif hat == (0,1):
                    print("Camera is moved to normal position")
                    message(str(5 + int(controlling_cam)) + " 0.0x")
                elif hat == (0,-1):
                    print("Switching camera control")
                    controlling_camera_one = not controlling_camera_one
                    if controlling_camera_one:
                        print("Controlling Cam 1")
                        controlling_cam = 1
                    else:
                        print("Controlling Cam 2")
                        controlling_cam = 2
                        
                textPrint.tprint(screen, "Hat {} value: {}".format(i, str(hat)))
                textPrint.unindent()
            textPrint.tprint(screen, "Controlling Camera: {}".format(str(controlling_cam)))
            # textPrint.tprint(screen, "Claw Locked: {}".format(str(claw_locked)))
            # textPrint.tprint(screen, "Claw Wrist Locked: {}".format(str(clawR_locked)))
            textPrint.unindent()
            textPrint.unindent()

            textPrint.unindent()
       
        

       
        
        # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT
        #

        # Go ahead and update the screen with what we've drawn.
        pygame.display.flip()

        # Limit to 20 frames per second.
        clock.tick(20)

    # Close the window and quit.
    # If you forget this line, the program will 'hang'
    # on exit if running from IDLE.
    pygame.quit()