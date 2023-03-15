import sys

def listMean(mylist):
    length = len(mylist)
    totalSum = 0
    for i in range(0, length):
        totalSum = totalSum + mylist[i] 

    mean = totalSum / length
    return mean 
    

def main():
    if len(sys.argv) < 2:
        print("enter a filename.")
        return

    filename = sys.argv[1]

    myfile = open(filename,  "r")
    lines = myfile.readlines()

    print(f"reading {filename}...")

    lineList = []

    # put lines into list
    for line in lines:
        intLine = int(line)
        lineList.append(intLine)

    # get the average
    mean = listMean(lineList)
        
    squaredDiffArray = []

    for line in lines:
        squareDiff = (int(line) - mean) ** 2 
        squaredDiffArray.append(squareDiff)

    # get the sd 
    sd = listMean(squaredDiffArray) ** (1/2)

    print("lines read: {}".format(len(lineList)))
    print("mean: {:.2f}".format(mean))
    print("standard deviation: {:.2f}".format(sd))

main()
