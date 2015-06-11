#include "agenda.h"

using namespace std;




/// ******************** Fonctions EvtManager ******************** ///

/// ********** Fonction pour ajouter une tache déjà existante ********** ///

void EvtManager::addEvt(const evt* t, const QDate & date, const QTime & h, bool pre) {
	//evt -> tableau de tous les evt existants (taches et RDV)
	//evt[] pointe sur un evenement particulier et evt[][] est l'evenement en lui meme
	//evt[].getTruc pour les données

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
			if (evt[i].getDate() == date)
			{
				unsigned int fin = evt[i].getHoraire() + evt[i].getDuree;		// L'opérateur + doit pas etre defini entre QTime et Duree
				if((evt[i].getHoraire() < h) && (h < fin))
				{
					cout<<"Erreur, la date et l'horaire démandés sont déjà occupés par un autre evenement !"<<endl;
					return;
				}
			}
		}
		if (date < t.getDateDisponibilite())
		{
			cout<<"Erreur, la date demandée est antérieure à la date de disponibilité de la tache"<<endl;
			return;
		}
		if (date > t.getDateEcheance())
		{
			cout<<"Erreur, la date demandée est postérieure à la date d'échéance de la tache"<<endl;
			return;
		}
		
		iterator i = getTachesPrecedentesPourTraitement().getIterator();
		while(!i.isDone())
		{
			if (!isEvtExistant(i.current2()))
			{
                cout<<"Erreur de précédence !"<<endl;
				return;
			}
			else
			{
                if (date < trouverEvt(i.current2().getID()).getDate())
				{
					cout<<"Erreur de précédence !"<<endl;
					return;
				}
			}
			i.next();
		}
        evt[nb]=t;
        nb++;
	}
    else
    {
        //Appel a une autre fonction addTachePre (pour tache preemptee), voir juste en dessous
    }
}

/// ********** Fonction auxiliere pour créer une tache préemptée ********** ///

//void addTachePre(unsigned int nbPre, )
//Fonction compliquée car quel parametres rentrer : une date ? ou plusieurs ? et les horaires ?
//Il faut faire une fonction qui va demander tout ca a l'utilisateur en fonction du nbr de preemption qu'il veut


/// ********** Fonction pour créer puis ajouter une tache ou un RDV ********** ///

virtual Evt & addNewEvt(const QDate & d, const QString & s, const QTime & deb, const Duree & dur, const QString & l, const QString & pers)
{
    //Appel constructeur du RDV + le mettre en tache (on ne peux pas creer une tache ici)
    EvtRDV * rdv = EvtRDV(d,s,deb,dur,l,pers);

    for (unsigned int i = 0; i<nb; i++)
    {
        if (evt[i].getDate() == rdv->getDate())
        {
            unsigned int fin = evt[i].getHoraire() + evt[i].getDuree;		// L'opérateur + doit pas etre defini entre QTime et Duree
            if((evt[i].getHoraire() < rdv->getDebut()) && (rdv->getDebut() < fin))
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

virtual void EvtManager::supprimerEvt(const QString & s)
{
    Evt * tmp = & trouverEvt(s);
    delete tmp;
    nb--;
}

/// ********** Fonction pour trouver l'evenement correspondant a un RDV ou a une tache ********** ///

Evt * EvtManager::trouverEvt(const QString & s)
{
	for(unsigned int i=0; i<nb; i++)
	{
		if (evt[i].getSujet() == s)
		{
			return evt[i];
		}
	}
	return NULL;
}










