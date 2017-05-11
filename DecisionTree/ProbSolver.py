import DtreeManager

def main():
    TreeM = DtreeManager.TreeMaker()
    #Input file Parsing
    #TreeM.ParseFile("Train.txt")
    TreeM.ParseFile("playtennis.train")
    tree = TreeM.MakeTree()
    print(tree)
    TreeM.Modeling(tree)

    #f = open("Test.txt","r")

if __name__ == '__main__':
    main()