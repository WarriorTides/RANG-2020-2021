import pygame
import serial

# Define some colors.
BLACK = pygame.Color('black')
WHITE = pygame.Color('white')

Axis1 = 0
Axis0 = 0
Axis4 = 0
#ser = serial.Serial('COM28')  # open serial port()


def message(msg):
    # if ser.is_open == False:
    #      ser.open()
    # ser.write(str(msg).encode())  # send the serial message
    # ser.close()
    #print(msg)
    pass

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

# Set the width and height of the screen (width, height).
screen = pygame.display.set_mode((500, 700))

pygame.display.set_caption("My Game")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates.
clock = pygame.time.Clock()

# Initialize the joysticks.
pygame.joystick.init()

# Get ready to print.
textPrint = TextPrint()
print("ready")
print("wait for arduino")
fineMode = False
fineMultiplier = 0.5
# while True:
#     if ser.in_waiting:
#         break
print("arduino ready")
stopped = [True] * 10
# -------- Main Program Loop -----------
while not done:
    # Possible joystick actions: JOYAXISMOTION, JOYBALLMOTION, JOYBUTTONDOWN,
    # JOYBUTTONUP, JOYHATMOTION
    for event in pygame.event.get():  # User did something.
        # print("event happened")
        # print(event.type)
        if event.type == pygame.QUIT:  # If user clicked close.
            done = True  # Flag that we are done so we exit this loop.

        # elif event.type == pygame.JO YBUTTONDOWN:
        #     print("Joystick button pressed.")
        # elif event.type == pygame.JOYBUTTONUP:
        #     print("Joystick button released.")

        screen.fill(WHITE)
        textPrint.reset()
        # print(pygame.joystick)
        # Get count of joysticks.
        joystick_count = pygame.joystick.get_count()
        # print(joystick_count)
        textPrint.tprint(screen, "Number of joysticks: {}".format(joystick_count))
        textPrint.indent()

        # For each joystick:
        for i in range(joystick_count):
            # print("joystick")
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

            for j in range(axes):
                # while ser.in_waiting:
                #     print(ser.readline())
                axis = joystick.get_axis(j)
                textPrint.tprint(screen, "Axis {} value: {:>6.3f}".format(j, axis))
                if 0.1 > axis and axis > -0.1:
                    if not stopped[j]:
                        stopped[j] = True
                        message(str(j) + " 0" + "x")
                    continue
                stopped[j] = False
                if fineMode:
                    j *= fineMultiplier
                if j == 4 or j == 5:
                    axis += 1.0
                message(str(j) + " " + str(axis) + "x")
            textPrint.unindent()
            openClawButton = 3
            closeClawButton = 1
            # fineButton = 3
            buttons = joystick.get_numbuttons()

            # if i == 1:
            if joystick.get_button(openClawButton) == 1:
                print("Claw Open")  # 66
                message("9" + " " + "1.0" + "x")
            elif joystick.get_button(closeClawButton) == 1:
                print("Claw Close")  # 86
                message("8" + " " + "1.0" + "x")
            elif joystick.get_button(2) == 1:
                print('Fine Mode: TRUE')
                fineMode = True
            elif joystick.get_button(0) == 1:
                print('Fine Mode: FALSE')
                fineMode = False
            elif joystick.get_button(openClawButton) == 0 and joystick.get_button(closeClawButton) == 0:
                #print("Claw Stop")
                message("9" + " " + "0.0" + "x")
                

    pygame.display.flip()

    # Limit to 20 frames per second.
    clock.tick(20)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit()
