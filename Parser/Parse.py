def Parse(allLength,input):
    GrammerList = {"S":[["NP","VP"],["Vst","NP"],["S","PP"]], "VP":[["V","NP"],["VP","PP"],["flies"]],"NP":[["Det","N"],["NP","PP"],["NP","NP"],["time"],["flies"]],
                   "PP":[["P","NP"]],"Vst":[["time"]],"P":[["like"]],"V":[["like"]],"Det":[["an"]],"N":[["arrow"]]}

    ## ParseTable Init
    ParseTable = []
    for i in range(0,allLength+1):
        ParseTable.append([])
        for j in range(0,allLength+1):
            ParseTable[i].append([])

    ## Fill Table
    for y in range(1,allLength+1):
        for x in range(allLength-y+1):
            #Fill the blank located in bottom of table
            if y == 1:
                for GList in GrammerList:
                    #if grammer find
                    if [input[x]] in GrammerList[GList]:
                        ParseTable[x][y].append(GList)
            for i in range(1,y):
                for GList in GrammerList:
                    for l_list in ParseTable[x][i]:
                        for r_list in ParseTable[x+i][y-i]:
                            if [l_list]+[r_list] in GrammerList[GList]:
                                ParseTable[x][y].append(GList)

    if len(ParseTable[0][allLength]) != 0:
        print("Grammer에 맞습니다")
    else:
        print("Grammer에 없는 문장입니다")

def main():
    f = open("input.txt","r")
    buf = f.read()
    buf = buf.split(" ")
    print(buf)
    f.close()

    Parse(len(buf),buf)

if __name__ == '__main__':
    main()