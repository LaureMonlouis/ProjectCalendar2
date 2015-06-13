#include "agenda.h"
#include "gestionprojet.h"

using namespace std;

//EXEMPLE: ui->lineEdit_titre->accessibleDescription();
//Pour recuperer la valeur entree je pense :)

/// ******************** Fonctions EvtManager ******************** ///

/// ********** Fonction pour ajouter une tache déjà existante ********** ///

void EvtManager::addEvt(const Tache * t, const QDate & date, const QTime & h, bool pre, unsigned int nbpre) {
    //evt -> tableau de tous les evt existants (taches et RDV)
    //evt[] pointe sur un evenement particulier et evt[][] est l'evenement en lui meme
    //evt[].getTruc pour les acceder aux donnes de l'evt

    if (nb == nbMax)
    {
        Evt ** newtab = new Evt * [nbMax + 10];
        nbMax += 10;
        for(unsigned int i = 0; i<nb; i++)
        {
            newtab[i]=evt[i];
        }
        Evt ** old = evt;
        evt = newtab;
        delete[] old;
    }

    if (pre == false)
    {
        for (unsigned int i = 0; i<nb; i++)
        {
            if (evt[i]->getDate() == date)
            {
                QTime finEvt =  QTime (0,0,evt[i]->getDebut().second() + evt[i]->getDuree().second());
                if((evt[i]->getDebut() < h) && (h < finEvt))
                {
                    cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
                    return;
                }
                QTime finT =  QTime (0,0,h.second() + t->getDuree().second());
                if((evt[i]->getDebut() < finT) && (finT < finEvt))
                {
                    cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
                    return;
                }
            }
        }
        if (date < t->getDateDisponibilite())
        {
            cout<<"Erreur, la date demandée est antérieure à la date de disponibilité de la tache"<<endl;
            return;
        }
        if (date > t->getDateEcheance())
        {
            cout<<"Erreur, la date demandée est postérieure à la date d'échéance de la tache"<<endl;
            return;
        }

        TacheExplorer::Iterator i = t->getTachesPrecedentesPourTraitement()->getIterator();
        while(!i.isDone())
        {
            //if (!isEvtExistant(i.current2())) isEvtExistant existant doit avoir pour paramètre evt*, la fonction est donc à définir
            //getEvt(tache.tachesprecedente.current2()).getDebut <  evtTache1.debut;

            if (!isEvtExistant(i.current2()))//en attendant
            {
                cout<<"Erreur de précédence !"<<endl;
                return;
            }
            else
            {
                if (date < trouverEvt(i.current2()->getId())->getDate())
                {
                    cout<<"Erreur de précédence !"<<endl;
                    return;
                }
            }
            i.next();
        }

        EvtTache * task = new EvtTache(date, h, t);
        evt[nb] = task;

        evt[nb] = new EvtTache(date, h, t);

        nb++;
    }
    else
    {
        for(unsigned int x = 0; x<nbpre; x++)
        {
            cout<<"Veuillez entrer les caracteristiques de la preemption numéro "<<x<<" :"<<endl;
            cout<<"\t1) Date"<<endl<<"\t2) Heure de debut"<<endl<<"\t3) Duree"<<endl;
            //Recuperer les donnes a partir de l'interface ...
            //addTachePre(date,hDebut,duree);
        }
    }
}

/// ********** Fonction auxiliaire pour créer une tache préemptée ********** ///

void EvtManager::addTachePre(const QDate & date, const QTime & h, const QTime & dur)
{
    for (unsigned int i = 0; i<nb; i++)
    {
        if (evt[i]->getDate() == date)
        {
            QTime finEvt =  QTime (0,0,evt[i]->getDebut().second() + evt[i]->getDuree().second());
            if((evt[i]->getDebut() < h) && (h < finEvt))
            {
                cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
                return;
            }
            QTime finT =  QTime (0,0,h.second() + dur.second());
            if((evt[i]->getDebut() < finT) && (finT < finEvt))
            {
                cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
                return;
            }
        }
    }
}



/// ********** Fonction pour créer puis ajouter une tache ou un RDV ********** ///

void EvtManager::addNewEvt(const QDate & d, const QString & s, const QTime & deb, const QTime & dur, const QString & l, const QString & pers)
{
    EvtRDV * rdv = new EvtRDV(d,s,deb,dur,l,pers);

    for (unsigned int i = 0; i<nb; i++)
    {
        if (evt[i]->getDate() == rdv->getDate())
        {
            QTime finEvt =  QTime (0,0,evt[i]->getDebut().second() + evt[i]->getDuree().second());
            if((evt[i]->getDebut() < rdv->getDebut()) && (rdv->getDebut() < finEvt))
            {
                cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
            }
            QTime finT =  QTime (0,0,deb.second() + dur.second());
            if((evt[i]->getDebut() < finT) && (finT < finEvt))
            {
                cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
                return;
            }
        }
    }

    if (nb == nbMax)
    {
        Evt ** newtab = new Evt * [nbMax + 10];
        nbMax += 10;
        for(unsigned int i = 0; i<nb; i++)
        {
            newtab[i]=evt[i];
        }
        Evt ** old = evt;
        evt = newtab;
        delete[] old;
    }
    evt[nb] = rdv;
    nb++;
}

/// ********** Fonction pour supprimer un evenement ********** ///

void EvtManager::supprimerEvt(const QString & s)
{
    Evt* tmp = trouverEvt(s);
    delete tmp;
    nb--;
}

/// ********** Fonction pour trouver l'evenement correspondant a un RDV ou a une tache ********** ///

Evt * EvtManager::trouverEvt(const QString & s)
{
    for(unsigned int i=0; i<nb; i++)
    {
        if (evt[i]->getDescripteur() == s)
        {
            return evt[i];
        }
    }
    return NULL;
}









