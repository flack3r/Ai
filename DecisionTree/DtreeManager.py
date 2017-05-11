import math
import operator

class TreeMaker():
    def __init__(self):
        self.tree = {}
        self.TargetAttr= []
        self.Attr = []
        self.AttributeState = {}
        self.ex = []
        self.ResultTree = {}

    def ParseFile(self, filename):
        f = open(filename,"r")
        self.TargetAttr = f.readline().rstrip().split(",")
        #self.ExamAttr -> Exmaple Colum name
        self.ExamAttr = f.readline().rstrip().split(",")
        #self.Attr -> All Attribute name
        self.Attr = self.ExamAttr[1:]

        #Classify Attribute's state
        for i in self.Attr:
            self.AttributeState[i] = []

        for tmp in f:
            buf = tmp.rstrip().split(",")
            tmp_exam = {}
            for i,j in zip(self.ExamAttr,buf):
                tmp_exam[i] = j
            self.ex.append(tmp_exam)
            for a,b in zip(self.Attr,buf[1:]):
                self.AttributeState[a].append(b)

        #Erase Multiple value
        for i in self.Attr:
            self.AttributeState[i] = set(self.AttributeState[i])
        f.close()

    def MakeTree(self):
        self.ResultTree = self.Id3(self.ex,self.TargetAttr,self.Attr)
        return self.ResultTree

    def Id3(self,Examples, Target_attribute, Attributes):
        #Create root node
        DTree = {}
        RootAttribute, EndCondition = self.CalcRoot(Examples,Target_attribute,Attributes)
        RootAttrName = RootAttribute[0][0]

        #if all examples are positive return Yes
        if abs(EndCondition[0]) == len(Examples):
            return self.TargetAttr[0]
        #if all examples are negative return No
        elif abs(EndCondition[1]) == len(Examples):
            return self.TargetAttr[1]
        #if Attributes is empty, return most common value of Target_attribute
        elif len(Attributes) == 0:
            if abs(EndCondition[0]) >= abs(EndCondition[1]):
                return self.TargetAttr[0]
            else:
                return self.TargetAttr[1]

        DTree[RootAttrName] = {}
        for i in self.AttributeState[RootAttrName]:
            #Add a new tree branch below root.
            DTree[RootAttrName][i] = {}
            #Let ExampleW, be the subset of Examples
            ExampleW = self.FilteringExam(Examples,i,RootAttrName)
            if len(ExampleW) == 0:
                DTree[RootAttrName][i] = self.TargetAttr[0] if abs(EndCondition[0]) >= abs(EndCondition[1]) else self.TargetAttr[1]
            else:
                TmpAttribute = Attributes[:]
                TmpAttribute.remove(RootAttrName)
                DTree[RootAttrName][i] = self.Id3(ExampleW, Target_attribute, TmpAttribute)
                TmpAttribute.append(RootAttrName)
        return DTree


    def CalcRoot(self,Examples,Target_attribute, Attributes):
        ### stage1 ###
        # Create Attribute
        EntropyData = {}
        EntropyDataAll = [0,0]
        for i in Attributes:
            EntropyData[i] = {}

        ### stage2 ###
        # Classify according to Target_attribute
        for i in Examples:
           #YES
           if i[self.ExamAttr[0]] == Target_attribute[0]:
               EntropyDataAll[0] += 1
               for j in i:
                   if j != self.ExamAttr[0] and j !=self.ExamAttr[0]:
                       State = i[j]
                       if State not in EntropyData[j].keys():
                           EntropyData[j][State] = [1,0]
                       else:
                           EntropyData[j][State][0] += 1
           #NO
           elif i[self.ExamAttr[0]] == Target_attribute[1]:
               EntropyDataAll[1] += -1
               for j in i:
                   if j != self.ExamAttr[0] and j !=self.ExamAttr[0]:
                       State = i[j]
                       if State not in EntropyData[j].keys():
                           EntropyData[j][State] = [0,-1]
                       else:
                           EntropyData[j][State][1] += -1

        # print(EntropyData)
        # print(EntropyDataAll)
        ### stage3 ###
        # Calculate Gain value
        GainResult= self.CalcGain(EntropyData,EntropyDataAll)

        # return Root Attribute Name
        return (GainResult,EntropyDataAll)

    def CalcGain(self,EntropyData, EntropyDataAll):
        AllEntropy = self.CalcEntropy(EntropyDataAll)
        AllNum = abs(EntropyDataAll[0])+abs(EntropyDataAll[1])
        #print(AllEntropy)
        GainResult = {}
        for i in EntropyData:
            SumAttributeEntropy = 0
            for j in EntropyData[i]:
                AttriButeNum = abs(EntropyData[i][j][0])+abs(EntropyData[i][j][1])
                tmp = (AttriButeNum / AllNum)*self.CalcEntropy(EntropyData[i][j])
                SumAttributeEntropy += tmp
            GainResult[i] = AllEntropy - SumAttributeEntropy
        GainResult = sorted(GainResult.items(), key = operator.itemgetter(1), reverse = True)
        #print(GainResult)
        return GainResult

    def CalcEntropy(self,Data):
        Yes = abs(Data[0])
        No = abs(Data[1])
        Sum = Yes+No
        if Yes==0 or No==0:
            return 0
        left = -((Yes)/Sum)*math.log((Yes/Sum),2)
        right = -((No)/Sum)*math.log((No/Sum),2)
        return left+right

    def FilteringExam(self, Example, State, AttributeValue):
        tmp = Example[:]
        Result = []
        for i in tmp:
            if i[AttributeValue] == State:
                i.pop(AttributeValue)
                Result.append(i)
        return Result

    #Decide whether condition is Yes or Not
    def ConcludeResult(self,Test,tree):
        TmpData = {}
        for a,b in zip(self.Attr,Test):
            TmpData[a] = b
        return self.DecideOnTree(TmpData,tree)

    #Traversal tree to Conclude Result
    def DecideOnTree(self,Test,TreeCurrent):
        if TreeCurrent == self.TargetAttr[0]:
            return self.TargetAttr[0]
        elif TreeCurrent == self.TargetAttr[1]:
            return self.TargetAttr[1]

        key = list(TreeCurrent.keys())
        key = key[0]
        DataValue = Test[key]
        return self.DecideOnTree(Test,TreeCurrent[key][DataValue])

    def Modeling(self,TreeCurrent):
        self.ModelingTree(TreeCurrent,0)

    def ModelingTree(self,TreeCurrent,padding):
        if TreeCurrent == self.TargetAttr[0]:
            print("-"*padding+self.TargetAttr[0])
            return
        elif TreeCurrent == self.TargetAttr[1]:
            print("-"*padding+self.TargetAttr[1])
            return

        key = list(TreeCurrent.keys())
        key = key[0]
        print("-"*padding + key)
        for i in TreeCurrent[key].keys():
            print("-"*(padding+2)+i)
            self.ModelingTree(TreeCurrent[key][i],padding+5)