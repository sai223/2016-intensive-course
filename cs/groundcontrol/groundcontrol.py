from PIL import Image
import serial

img = Image.open("ajou.png")
print(img.size)

height = img.size[1]
width = img.size[0]

MAX_FRAME_SIZE = 30

if(height > width):
    height = MAX_FRAME_SIZE
    width = (int)(MAX_FRAME_SIZE * (img.size[0] / img.size[1]))
elif(width > height):
    width = MAX_FRAME_SIZE
    height = (int)(MAX_FRAME_SIZE * (img.size[1] / img.size[0]))
else:
    width = MAX_FRAME_SIZE
    height = MAX_FRAME_SIZE

print(width, height)
ajou = img.resize( (width, height) )
print(ajou.size)

gray = ajou.convert("L")
#gray.show()

mtr = [[8 for col in range(MAX_FRAME_SIZE)]for row in range(height)]
for x in range(width):
    for y in range(height):
        if (gray.getpixel((x,y)) > 150):
            mtr[y][x] = 0
        else:
            mtr[y][x] = 1

packet = [[8 for col in range(MAX_FRAME_SIZE + 1)]for row in range(height)]
frame = [height, width]

for x in range(height):
    for y in range(0, MAX_FRAME_SIZE + 1):
        if y == 0:
            packet[x][y] = x
        else:
            packet[x][y] = mtr[x][y-1]

for x in range(height):
    print("\n")
    for y in range(MAX_FRAME_SIZE + 1):
        print(packet[x][y], end=" ")
print("\n")

ser = serial.Serial(
    port='COM9',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0.1
)

def receive_ack():

    numOfLines = 0


    while True:
        response = ser.readline()
        numOfLines = numOfLines + 1

        if (numOfLines >= 5 or response == b'okay\0'):
            break

    if(response == b'okay\0'):
        print("ack received")
    else:
        print("ack isn't coming : try again")
        quit(0)


def send_packet():

    for x in range(height):

        ser.write(packet[x])
        receive_ack()
        print(x, "  complete")

while(True):
    s = input('Write down Mode : ')
    if ser.isOpen():

        ser.flushInput() #flush input buffer, discarding all its contents
        ser.flushOutput()#flush output buffer, aborting current output

        if(s == "wait"):
            ser.write(b'wait\0')
            receive_ack()
        elif(s == "maze"):
            ser.write(b'maze\0')
            receive_ack()
        elif(s == "draw"):
            ser.write(b'draw\0')
            receive_ack()
            ser.write(frame)
            receive_ack()
            send_packet()
        elif(s == "quit"):
            ser.close()
            break
    else:
        print("cannot open serial port")



