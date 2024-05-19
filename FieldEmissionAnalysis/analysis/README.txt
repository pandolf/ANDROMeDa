ISTRUZIONI PROGRAMMI PER ELETTRONI - da aggiornare quando sistemo i problemi dati da wget

Scaricare le forme d'onda .trc dal drive () salvandole dei Downloads del computer divise 
per cartelle di voltaggio (non di cooldown). Nelle cartelle è anche presente un file .txt 
che è circa un'istogramma delle ampiezze. 

In analysis, creare una cartella "tree". Eseguendo prima makeRawTree.py e dopo makeTree.cpp 
si otterranno rispettivamente un tree raw con le pshapes complete (ed altre info come cooldown, 
voltaggio, numero di evento ecc) ed un tree con sia le variabili di alto livello (baseline, 
amppiezza, carica + info come nel tree raw) sia dei controlli applicati sulla forma d'onda, 
definiti in ../interface/pshape_functions.h. I controlli riguardano la saturazione, il punto 
in cui l'onda ha triggerato, la sua larghezza a metà ampiezza e quante volte vengono superate 
determinate soglie.

fitAmpChargeData.cpp permette di fittare i dati dopo aver scelto il loro cooldown, voltaggio, 
variabile (ampiezza/carica), fit (cruijff/gauss) e picco d'interesse (la carica può più di 
uno). Di volta in volta vanno cambiati a mano il binnaggio, il range grafico e il range di 
fit. Il grafico viene salvato nella cartella "analysis/plots/CD [numero del cooldown]/[voltaggio] V". 
Una volta individuati range e binnaggio definitivi, scriverli in CD_plot_details.h nelle 
apposite funzioni, per poter replicare i plot ed estrarne gli andamenti dei parametri rispetto 
al voltaggio.

drawAmpChargeFinalFits_single.cpp attinge alle informazioni contenute in 
../interface/CD_plot_details.h per fare il fit finale di un set di dati, una volta scelto il cooldown, 
il voltaggio, la variabile da fittare (amp/charge),la funzione di fit(cruijff/gauss), 
il picco d'interesse. Il grafico viene salvato in "analysis/plots/CD [numero del cooldown]/final plots". 
drawAmpChargeFinalFits_all.cpp fa lo stesso con tutti i voltaggi appartententi ad un CD dopo 
aver scelto le stesse variabili di prima; oltre a salvare tutti i plot, scrive su file 
(salvato in "analysis/fit_results") la media e la deviazioni std ricavati dai fit con i loro errori.

drawFitParametersTrends.py fa i plot finali di mu, sigma/mu e risoluzione vs voltaggio 
dei dati scritti prima su file e li salva in "analysis/plots/CD [numero del cooldown]/final plots". 
In ../interface/graphics.h sono contenute funzioni grafiche riguardanti gli istogrammi.
