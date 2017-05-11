Count ={"AllNo":0,"AllYes":0, "No":{},"Yes":{}}
Prob = {"AllNo":0,"AllYes":0, "No":{},"Yes":{}}

def train():
    ##[Stage1] Test case counting
    f = open("playtennis.train","r")
    for tmp in f:
        tmp = tmp.rstrip().split(",")
        if tmp[0] == "No":
            Count["AllNo"] += 1
        else:
            Count["AllYes"] += 1

        for i in tmp[1:]:
            if tmp[0] == "No":
                if i not in Count["No"]:
                    Count["No"][i] = 1
                else:
                    Count["No"][i] += 1
            else:
                if i not in Count["Yes"]:
                    Count["Yes"][i] = 1
                else:


                    Count["Yes"][i] += 1
    f.close()
    print(Count)

    ##[Stage2] Calc Probility
    Prob["AllYes"] = Count["AllYes"]/(Count["AllYes"]+Count["AllNo"])
    Prob["AllNo"] = Count["AllNo"]/(Count["AllYes"]+Count["AllNo"])

    #Yes
    for i in Count["Yes"]:
        Prob["Yes"][i] = Count["Yes"][i] / Count["AllYes"]

    #No
    for i in Count["No"]:
        Prob["No"][i] = Count["No"][i] / Count["AllNo"]
    print(Prob)

def Check():



    f = open("playtennis.train","r")
    allcount = 0;
    match = 0
    for i in f:
        allcount += 1
        tmp = i.rstrip().split(",")
        target = tmp[0]
        result = ""
        YesProb = 1
        NoProb = 1
        for j in tmp[1:]:
            #Yes
            if j not in Prob["Yes"]:
                YesProb = YesProb * 0.0001
            else:
                YesProb = YesProb * Prob["Yes"][j]

            if j not in Prob["No"]:
                NoProb = NoProb * 0.0001
            else:
                NoProb = NoProb * Prob["No"][j]

        ResultYes = Prob["AllYes"]*YesProb
        ResultNo = Prob["AllNo"]*NoProb
        if ResultYes >= ResultNo:
            result = "Yes"
        else:
            result = "No"

        if target == result:
            match += 1

    print("Accuracy: %lf" %((match/allcount)*100))
    f.close()

def main():
    train()
    Check()

if __name__ == '__main__':

    main()