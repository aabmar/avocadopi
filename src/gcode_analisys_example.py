import re

filename="test.gcode"


'''
Finds all lines where we start on the next layer for prusa gcode files
'''
def find_raise_with_lift():

    with open(filename, "r") as fil:
        counter = 0
        first = True
        tall1 = 0
        tall2 = 0
        pairCounter = 0
        for line in fil:
            counter+=1
            if not line.startswith(";"):
                # print(line, end="")
                r1 = re.findall(r"Z(\d+.\d+)", line)
                if len(r1) != 0:
                    if first:
                        first = not first
                        print("Skipping first")
                        continue
                    curTall = float(r1[0])
                    # print(curTall)
                    if pairCounter == 0:
                        tall1 = curTall
                        pairCounter+=1
                        continue
                    if pairCounter == 1:
                        if(tall2<curTall):
                            print("Nest hakk at", counter)
                        # print("Pair: %.3f og %.3f" %(tall1, curTall))
                        pairCounter = 0
                        tall2 = curTall

'''
Finds all lines where we start on the next layer for cura gcode files
'''
def find_raise_normal():
    with open(filename, "r") as fil:
        counter = 0
        for line in fil:
            counter+=1
            if not line.startswith(";"):
                # print(line, end="")
                r1 = re.findall(r"Z(\d+.\d+)", line)
                if len(r1) != 0:
                    # print(line)
                    print("Neste hakk at", counter)


def detect_mode():
    with open(filename, "r") as fil:
        prevtall=None
        for line in fil:
            if not line.startswith(";"):
                # print(line, end="")
                r1 = re.findall(r"Z(\d+.\d+)", line)
                if len(r1) != 0:
                    #hvis vi har 2 tall etter hverandre med forskjell på 1
                    curTall = float(r1[0])
                    if prevtall is None:
                        prevtall = curTall
                        continue
                    if abs(curTall-prevtall)==1:
                        # print("Den lifter")
                        return True
    return False


'''
test
'''
def find_raises():
    if(detect_mode()):
        # print("lift")
        find_raise_with_lift()
    else:
        # print("normal")
        find_raise_normal()

find_raises()