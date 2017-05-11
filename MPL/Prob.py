import MPL
import time

def main():
    #Training data

    start = time.time()
    exam = []
    f = open("optdigits.tra")
    for buf in f:
        exam.append(buf.rstrip().split(","))
    f.close()

    # Train Machine
    Machine = MPL.Network(30)
    Machine.BackProTrain(exam, 15)
    f.close()
    # test
    f = open("optdigits.tes")
    count = 0
    all = 0
    for buf in f:
        all += 1
        test = buf.rstrip().split(",")
        RealResult = int(test[-1])
        #print(RealResult)
        Result = Machine.DecideResult(test[:-1])
        # print(Result)
        if RealResult == Result:
            count += 1
    f.close()
    print("Accurate: %lf" %((count/all)*100))
    print("time : %.2f" %(time.time()-start))

if __name__ == '__main__':
    main()