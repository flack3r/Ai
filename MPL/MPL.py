import random
import math

class Network():
    #HiddenNumber is 1 hidden layer's node number
    def __init__(self,HiddenNumber):
        self.LearningWeight = 0.04
        #HiddenNeuran Init
        self.HiddenNeuranLayer = []
        for i in range(HiddenNumber):
            self.HiddenNeuranLayer.append(Neuran(10))

        # 64 bit input vector
        self.InputNeuranLayer = []
        for i in range(64):
            self.InputNeuranLayer.append(Neuran(HiddenNumber))

    #BackPropagation Tarining
    def BackProTrain(self, train_exam, iter_num):
        for i in range(iter_num):
            print("[%d]iter" %(i+1))
            for exam in train_exam:
                HiddenSigResult = []
                TargetSigResult = []
                ResultDelta = []
                HiddenDelta = []
                input = list(map(lambda x: int(x),exam[:-1]))
                target = int(exam[-1])
                target = [1 if i==target else 0 for i in range(10)]

                ###Stage1 Propagate sigmoid value###
                self.Propagate(input, HiddenSigResult, TargetSigResult)
                ###Stage2 calc target delta###
                for idx, Targetsig in enumerate(TargetSigResult):
                    delta = Targetsig * (1-Targetsig) * (target[idx] - Targetsig)
                    ResultDelta.append(delta)

                ####Stage3 calc hidden delta###
                for hindex, hsig in enumerate(HiddenSigResult):
                    Sum = 0
                    hiddenN = self.HiddenNeuranLayer[hindex]
                    WeightSize = hiddenN.GetWeightSize()
                    for i in range(WeightSize):
                        Sum += hiddenN.GetWeightValue(i)*ResultDelta[i]
                    delta = hsig * (1 - hsig) * Sum
                    HiddenDelta.append(delta)

                ###Stage4 Update weight###
                #stage4-1 update hidden neuran weight
                for hindex, hiddenN in enumerate(self.HiddenNeuranLayer):
                    for Rdeltaindex, Rdelta in enumerate(ResultDelta):
                        tmp = self.LearningWeight * Rdelta * HiddenSigResult[hindex]
                        tmp += hiddenN.GetWeightValue(Rdeltaindex)
                        hiddenN.SetWeight(Rdeltaindex,tmp)

                test = []
                for hiddenN in self.HiddenNeuranLayer:
                    test.append(hiddenN.Inspect(0))
                #stage4-2 update input neuran weight
                for inputIndex, inputN in enumerate(self.InputNeuranLayer):
                    for hiddenindex, Hdelta in enumerate(HiddenDelta):
                        tmp = self.LearningWeight * Hdelta * input[inputIndex]
                        tmp += inputN.GetWeightValue(hiddenindex)
                        inputN.SetWeight(hiddenindex,tmp)

    #Get Propagation
    def Propagate(self,input, HiddenSigResult, TargetSigResult):
        #Stage1
        #Send sigmoid value to Hidden
        for i in range(len(self.HiddenNeuranLayer)):
            product = 0
            for idx, input_neu in enumerate(self.InputNeuranLayer):
                product += input_neu.GetWeightValue(i)*input[idx]
            sig = self.calcSigmoid(product)
            HiddenSigResult.append(sig)

        #print(HiddenSigResult)
        #Stage2
        #Send Hidden to Target
        for i in range(10):
            product = 0
            for idx, hidden_neu in enumerate(self.HiddenNeuranLayer):
                product += hidden_neu.GetWeightValue(i)*HiddenSigResult[idx]
            sig = self.calcSigmoid(product)
            TargetSigResult.append(sig)

        #print(TargetSigResult)
    def calcSigmoid(self,error):
        tmp = 1 + math.exp(-error)
        return 1 / (tmp)

    def DecideResult(self, input):
        Result = []
        HiddenSigResult = []
        input = list(map(lambda x: int(x), input))
        self.Propagate(input,HiddenSigResult,Result)
        # for i in range(len(Result)):
        #     print("%.4f " %(Result[i]), end="")
        # print("")
        return Result.index(max(Result))

#Have Weight Information
class Neuran():
    # Weights is multi list
    def __init__(self,N_out):
        self.Weights = []
        self.InitWeight(N_out)

    def GetWeightValue(self,idx):
        return self.Weights[idx]

    def SetWeight(self, idx, value):
        self.Weights[idx] = value

    def InitWeight(self,OutputNum):
        for i in range(OutputNum):
            tmp = random.uniform(0.01,0.03)
            self.Weights.append(tmp)
            
    def GetWeightSize(self):
        return len(self.Weights)

    def Inspect(self,idx):
        return self.Weights[idx]