import cv2
import mediapipe as mp
import time
import serial

mp_drawing = mp.solutions.drawing_utils
mp_face_mesh = mp.solutions.face_mesh
drawing_spec = mp_drawing.DrawingSpec(thickness=1, circle_radius=1)

ser = serial.Serial('COM4', 9600)


def getLandmarks(image):
    face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)
    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    image.flags.writeable = False
    results = face_mesh.process(image)
    #landmarks = results.multi_face_landmarks[0].landmark
    return  results.multi_face_landmarks

camera = cv2.VideoCapture(0) #Laptop Camera

success, image = camera.read()



while(True):
    
    ButtonP = False
    rec = True

    while not ButtonP:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
            ButtonP = True
            rec = False
   

    while (not rec):
        x_str = ""
        y_str = ""
        z_str = ""
        if not success:
            #cv2.imshow('frame', image)
            continue
        #cv2.imshow('frame', image)
        success, image = camera.read()
        res = getLandmarks(image)
        if res is None:
            continue
        else:
            # Calculate average x, y, and z coordinates of all landmarks
            x = []
            y = []
            z = []
            for face_landmarks in res:
                for landmark in face_landmarks.landmark:
                    x.append(landmark.x)
                    y.append(landmark.y)
                    z.append(landmark.z)
            x_mean = sum(x) / len(x)
            y_mean = sum(y) / len(y)
            z_mean = sum(z) / len(z)

            # Remove outliers
            x_filtered = []
            y_filtered = []
            z_filtered = []
            for i in range(len(x)):
                if abs(x[i] - x_mean) < 0.1 and abs(y[i] - y_mean) < 0.1 and abs(z[i] - z_mean) < 0.1:
                    x_filtered.append(x[i])
                    y_filtered.append(y[i])
                    z_filtered.append(z[i])
            x_mean_filtered = sum(x_filtered) / len(x_filtered)
            y_mean_filtered = sum(y_filtered) / len(y_filtered)
            z_mean_filtered = sum(z_filtered) / len(z_filtered)

            # Store final x, y, and z values as strings
            x_str = str(x_mean_filtered)[:6]
            y_str = str(y_mean_filtered)[:6]
            z_str = str(z_mean_filtered)[:6]
            #SEND HERE
            
            combStr = x_str + ',' + y_str + ',' + z_str
            print(combStr)
            data = combStr.encode()
        
            ser.write(data)

            start_time = time.monotonic()

            while (not rec) and (time.monotonic() - start_time < 5):
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
                    rec = True

        
        time.sleep(1)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break





camera.release()
# Destroy all the windows
cv2.destroyAllWindows()