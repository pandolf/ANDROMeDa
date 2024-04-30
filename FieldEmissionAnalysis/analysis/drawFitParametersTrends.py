import sys
import numpy as np
import matplotlib 
import matplotlib.pyplot as plt
import math


#FUNCTIONS
#plotting parameters trend
def plot (x, y, y_err, label_y, axis, CD, choice, picco, name):
    plt.figure(dpi = 225)

    plt.errorbar(x, y, yerr = y_err, x_err = None, fmt = 'o', markersize = 5.5, capsize = 2.5, elinewidth = 1, 
                 color = 'black', ecolor = 'black')

    plt.grid(linestyle = '--')
    plt.xlabel('Voltage (V)', size=14)
    plt.ylabel(label_y, size=14)
    if   (np.amax(x)-np.amin(x) < 15): ticks = 1.0
    elif (np.amax(x)-np.amin(x) > 15): ticks = 2.0
    plt.xticks(np.arange(np.amin(x), np.amax(x)+1, ticks))
    plt.xticks(fontsize=11)
    plt.yticks(fontsize=11)
    if  (axis == 'free'): pass
    elif(axis == 'zero'): 
        index_max = np.argmax(y)
        ymax      = 1.1* y[index_max]+y_err[index_max]
        plt.ylim(0,ymax) 
    plt.tight_layout()

    out_path = "plots/CD %d/final plots/" %CD
    out_name = "%s_%spicco%d.png" %(name, choice, picco)
    plt.savefig("%s%s" %(out_path, out_name))
    print("plot saved in %s%s"%(out_path,out_name))

    
#definition of error propagation for sigma/mu
def err (mu,mu_err, sigma, sigma_err):
    d1 = np.power(sigma_err,2)/np.power(mu,2)
    d2 = np.power(sigma,2)*np.power(mu_err,2)/np.power(mu,4)
    return (np.sqrt(d1+d2))


#controllo su variabili da linea di comando
def init_ctrls():
    #lettura di variabili da linea di comando
    if len(sys.argv) < 4:
        print("USAGE: [CD_number] [amp/charge] [picco] [cruijff/gauss]")
        print("")
        sys.exit()
    if (sys.argv[2]!="amp" and sys.argv[2]!="charge"):
        print("variable accepted: 'amp' or 'charge'.")
        sys.exit()
    if (sys.argv[4]!="cruijff" and sys.argv[4]!="gauss"):
        print("fit accepted: 'cruijff' or 'gauss'.")
        sys.exit()


        
#"MAIN"
#controlli iniziali
init_ctrls()

#dichiarazioni necessarie
volt      = np.array([]) 
mu        = np.array([]) 
mu_err    = np.array([]) 
sigma     = np.array([]) 
sigma_err = np.array([]) 

#parametri da linea di comando
CD_number  = int(sys.argv[1])
choice     =     sys.argv[2]
picco      = int(sys.argv[3])
fit_fcn    =     sys.argv[4]

#lettura array da file
outfile_name = "CD%d_%s_picco%d_fit%s.txt"%(CD_number,choice,picco,fit_fcn)
with open("fit_results/%s"%outfile_name) as f:
    #salto le due righe iniziali di commenti
    for _ in range(2):
        next(f)
    #leggo dati aggiornando gli array
    for line in f:
        data      = line.split()
        volt      = np.append(volt     , int  (data[0]))
        mu        = np.append(mu       , float(data[1]))
        mu_err    = np.append(mu_err   , float(data[2]))
        sigma     = np.append(sigma    , float(data[3]))
        sigma_err = np.append(sigma_err, float(data[4]))

#plotting final graphs of mu, sigma/mu and resolution vs voltage
plot(volt,            mu,                             mu_err ,'μ (V)' , 'free', CD_number, choice, picco, "mu"         )
plot(volt,      sigma/mu,      err(mu,mu_err,sigma,sigma_err),'σ / μ' , 'zero', CD_number, choice, picco, "sigmamu"    )
plot(volt, volt*sigma/mu, volt*err(mu,mu_err,sigma,sigma_err),'σ (eV)', 'zero', CD_number, choice, picco, "risoluzione")

