import sys
import os
import ROOT
from ROOT import TFile
from ROOT import TTree
from array import array
import lecroyparser

#lettura di variabili da linea di comando
if len(sys.argv) < 6:
    print("USAGE: [CD_number] [voltage] [trigger] [nfiles] [filename]")
    print("[nfiles]  : numbers of files to be read")
    print("[filename]: to be written without --numbers.trc at the end")
    print("")
    sys.exit()
    
#dichiarazioni necessarie
#se non sono definiti con array (quindi np non va bene!!!) riempie un tree folle
counter = 0 #to define the event number
CD_number = array('i',      [0])
voltage   = array('i',      [0])
dt        = array('f',      [0])
event     = array('i',      [0])
trigger   = array('f',      [0])
pshape    = array('f', 2502*[0]) #2502 lunghezza max pshape 

#parametri da linea di comando
CD_number[0] =   int(sys.argv[1])
voltage[0]   =   int(sys.argv[2])
trigger[0]   = float(sys.argv[3])
nfiles       =   int(sys.argv[4]) 
filename     =       sys.argv[5]

#creazione del Tree
tree = TTree('tree', 'treeraw')
tree.Branch('CD_number', CD_number,    'CD_number/I')
tree.Branch('voltage'  , voltage  ,      'voltage/I')
tree.Branch('dt'       , dt       ,           'dt/F')
tree.Branch('event'    , event    ,        'event/I')
tree.Branch('trigger'  , trigger  ,      'trigger/F')
tree.Branch('pshape'   , pshape   , 'pshape[2500]/F')

#se segmentato a 100 e più di 1000 file -> un tree per ogni 1000 file o non regge il TBuffer
for i in range(0, nfiles):
    if  (i in range (0,10)      ): zeros = '0000'
    elif(i in range (10,100)    ): zeros = '000'
    elif(i in range (100,1000)  ): zeros = '00'
    elif(i in range (1000,10000)): zeros = '0'
        
    path = os.path.expanduser("~/Downloads/%s V/%s--%s%s.trc" %(CD_number,int(voltage[0]),filename,zeros,i)) 
    data = lecroyparser.ScopeData(path)
    
    dt[0] = round(abs(data.x[1]-data.x[0]),12) 

    #controllo sulla segmentazione (numero di onde in ogni file)
    if  (len(data.y)%2501==0): points = 2501
    elif(len(data.y)%2502==0): points = 2502
    else: continue    

    segmenti = int(len(data.y)/points)  #numero di onde in ogni file
    for l in range (0, segmenti):
        for j in range (l*points,(l+1)*points):
            pshape[j-l*points] = data.y[j]
            
        event[0] = int(counter)
        counter+=1
        tree.Fill()

print("tree_raw entries:",tree.GetEntries())

# make a file and write the tree to it
outfile_name = ("CD%d_%dV_treeraw.root" %(CD_number[0],int(voltage[0])))
file = TFile("tree/%s" %(outfile_name), "RECREATE")
file.WriteObject(tree, "%sV_treeraw" %int(voltage[0]))
file.Close()

print("tree_raw saved in", outfile_name)
