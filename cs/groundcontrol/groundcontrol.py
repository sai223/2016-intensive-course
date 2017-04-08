from PIL import Image
import serial
import time

img = Image.open("ajou.png")
print(img.size)

height = img.size[1]
width = img.size[0]

if(height > width):
    height = 50
    width = (int)(50 * (img.size[0] / img.size[1]))
elif(width > height):
    width = 50
    height = (int)(50 * (img.size[1] / img.size[0]))
else:
    width = 50
    height = 50

print(width, height)
ajou = img.resize( (width, height) )
print(ajou.size)

gray = ajou.convert("L")
#gray.show()

mtr = [[8 for col in range(50)]for row in range(height)]
for x in range(width):
    for y in range(height):
        if (gray.getpixel((x,y)) > 150):
            mtr[y][x] = 0
        else:
            mtr[y][x] = 1

packet = [[8 for col in range(51)]for row in range(height)]
frame = [height, width]

for x in range(height):
    for y in range(0, 51):
        if y == 0:
            packet[x][y] = x
        else:
            packet[x][y] = mtr[x][y-1]

for x in range(height):
    print("\n")
    for y in range(51):
        print(packet[x][y], end=" ")
print("\n")

ser = serial.Serial(
    port='COM12',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0.1
)

#ser.open()

s = input('Write down Mode : ')

def receive_ack():
    #ser.flushInput() #flush input buffer, discarding all its contents
    #ser.flushOutput()#flush output buffer, aborting current output

    #time.sleep(0.1)

    numOfLines = 0
    while True:
        response = ser.readline()
        numOfLines = numOfLines + 1

        if (numOfLines >= 5 or response == b'okay'):
            break

    if(response == b'okay'):
        print("ack received")
    else:
        print("ack isn't coming : try again")
        quit(0)

def send_packet():

    for x in range(height):
        #ser.flushInput()  # flush input buffer, discarding all its contents
        #ser.flushOutput()  # flush output buffer, aborting current output

        ser.write(packet[x])
        receive_ack()
        print(x, "  complete")


if ser.isOpen():

    ser.flushInput() #flush input buffer, discarding all its contents
    ser.flushOutput()#flush output buffer, aborting current output

    if(s == "maze"):
        ser.write(b'maze')
    elif(s == "draw"):
        ser.write(b'draw')
        receive_ack()
        ser.write(frame)
        receive_ack()
        send_packet()

    ser.close()
else:
    print("cannot open serial port")


