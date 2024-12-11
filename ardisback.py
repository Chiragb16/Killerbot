import socket
import cv2 as cv
import time
import handtrackingmodule as htm

# Function to establish socket connection with retry mechanism
def connect_to_socket(esp_id, port, retries=5, delay=2):
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    for attempt in range(retries):
        try:
            soc.connect((esp_id, port))
            print("Connected to socket.")
            return soc
        except socket.error as e:
            print(f"Connection attempt {attempt + 1} failed: {e}")
            time.sleep(delay)  # Wait before retrying
    print("Failed to connect to socket after several attempts.")
    exit(1)

esp_id = "192.168.239.154"
port = 80
soc = connect_to_socket(esp_id, port)

cap = cv.VideoCapture(0)
ptime = 0
wCam, hCam = 640, 488
cap.set(3, wCam)
cap.set(4, hCam)

detector = htm.handDetector(detectionCon=0.75)

while True:
    success, img = cap.read()
    if not success:
        break

    img = detector.findHands(img)
    mylist = detector.position(img, draw=False)
    tip_ids = [4, 8, 12, 16, 20]

    if mylist:
        fingers = []

        # Thumb
        if mylist[tip_ids[0]][1] < mylist[tip_ids[0] - 1][1]:
            fingers.append(0)
        else:
            fingers.append(1)

        # Other fingers
        for id in range(1, 5):
            if mylist[tip_ids[id]][2] < mylist[tip_ids[id] - 2][2]:
                fingers.append(1)
            else:
                fingers.append(0)

        finger_count = fingers.count(1)
        print(finger_count)

        try:
            # Add newline character to the message
            message = f"{finger_count}\n".encode()
            soc.sendall(message)
            print(f"Sent command: {finger_count}")  # Debug print
        except socket.error as e:
            print(f"Error sending data: {e}")
            soc.close()
            soc = connect_to_socket(esp_id, port)

    ctime = time.time()
    fps = 1 / (ctime - ptime) if ptime else 0
    ptime = ctime
    cv.putText(img, f'FPS: {int(fps)}', (10, 50), cv.FONT_HERSHEY_PLAIN, 3, (255, 0, 0), 3)

    cv.imshow('Hand Tracking', img)
    if cv.waitKey(1) & 0xFF == ord('c'):
        break

cap.release()

# Close socket
try:
    soc.close()
except socket.error as e:
    print(f"Socket close error: {e}")

cv.destroyAllWindows()