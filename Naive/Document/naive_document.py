from functools import reduce

Count ={}
Prob = {}
Attribute = set()

#if lable name is in Count or Prob array
def CheckName(name):
    All = "All"+name
    #Check All Counting label
    if All not in Count:
        Count[All] = 0
    if All not in Prob:
        Prob[All] = 0

    #Check class name
    if name not in Count:
        Count[name] = {}
    if name not in Prob:
        Prob[name] = {}

def train():
    ##[Stage1] Test case counting
    f = open("r8-train-all-terms.txt","r")
    #f = open("train","r")
    train_number = 0
    for tmp in f:
        tmp = tmp.rstrip().split("\t")
        class_name = tmp[0]
        tmp = tmp[1].rstrip().split(" ")
        CheckName(class_name)

        #[Stage2] All number counter
        Count["All"+class_name] += 1
        train_number += 1

        #[Stage3] Attribute number counter
        for i in tmp:
            Attribute.update([i])
            if i not in Count[class_name]:
                Count[class_name][i] = 1
            else:
                Count[class_name][i] += 1
    f.close()

    ##[Stage2] Calc Probility
    for name in Count:
        if name[:3] == "All":
            Prob[name] = Count[name]/train_number
        else:
            TestAlphaNum = reduce(lambda x,y: x+y, Count[name].values())
            #print(TestAlphaNum)
            for i in Attribute:
                if i not in Count[name]:
                    Prob[name][i] = (0+1) / (TestAlphaNum+len(Attribute))
                else:
                    Prob[name][i] = (Count[name][i]+1) / (TestAlphaNum+len(Attribute))


def Check():
    #f = open("train","r")
    f = open("r8-test-all-terms.txt","r")
    allcount = 0
    match = 0
    for i in f:
        allcount += 1
        tmp = i.rstrip().split("\t")
        target = tmp[0]

        tmp = tmp[1].rstrip().split(" ")
        CalcProb = {}
        for i in Prob:
            if i[:3] != 'All':
                CalcProb[i] = 1

        for i in CalcProb:
            for j in tmp:
                if j not in Prob[i]:
                    Prob[i][j] = 0.001
                CalcProb[i] = (CalcProb[i]) * (Prob[i][j])

        ProbResult = {}
        for i in CalcProb:
            ProbResult[i] = Prob["All"+target]*CalcProb[i]

        #calc max attribute
        result = 0
        for i in ProbResult:
            if ProbResult[i] >= result:
                result = ProbResult[i]
        #find result
        result = [v for v in ProbResult if ProbResult[v] == result]

        if target == result[0]:
            match += 1

    print("Accuracy: %lf" %((match/allcount)*100))
    f.close()

def main():
    train()
    Check()

if __name__ == '__main__':
    main()