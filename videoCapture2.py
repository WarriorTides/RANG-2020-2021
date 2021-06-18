import cv2

# frameWidth = 640
# frameHeight = 480
cap = cv2.VideoCapture(0)
# cap.set(3, frameWidth)
# cap.set(4, frameHeight)
# cap.set(10,150)
counter = 0
path = '/Users/sherylhsu/PycharmProjects/Coral/videoCaptureImages/'
while cap.isOpened():
    success, img = cap.read()
    height, width, channels = img.shape
    print(height, width)
    if success:
        cv2.imshow("Result", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        key = cv2.waitKey(1)
        # print(key)
        if key == ord('s'):
            counter += 1
            cropped = img[0:int(height/2), 0:int(width/2)]
            cv2.imwrite(filename=path+'image-' + str(counter) + '.jpg', img=cropped)
            print("Saved Image-" + str(counter) + "!")
            print("Processing image...")
            # img_ = cv2.imread('saved_img.jpg', cv2.IMREAD_ANYCOLOR)
            # print("Converting RGB image to grayscale...")
            # gray = cv2.cvtColor(img_, cv2.COLOR_BGR2GRAY)
            # print("Converted RGB image to grayscale...")
            # print("Resizing image to 28x28 scale...")
            # img_ = cv2.resize(gray,(28,28))
            # print("Resized...")
            # print(cv2.imwrite(filename=str(counter)+".jpg", img=img))
            print("Image saved! path ", str(counter)+".jpg")
            # cap.open(0, cv2.CAP_DSHOW)

        elif key == ord('q'):
            cap.release()
            cv2.destroyAllWindows()
            break
