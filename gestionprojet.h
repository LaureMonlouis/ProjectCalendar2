#ifndef GESTIONPROJET
#define GESTIONPROJET
#include <QString>
#include <QDate>
#include <QFile>
#include <QTextStream>
//#include <QMessageBox>
#include <QtXml>
#include "timing.h"

const QDate DATE_MIN = QDate(0,1,1);
const QDate DATE_MAX = QDate(9999,1,1);
const QTime D_NON_PREEMPT = QTime (12,0);

class CalendarException{
public:
    CalendarException(const QString& message):info(message){
        //QMessageBox::warning(this, "Erreur", getInfo());
    }
    QString getInfo() const { return info; }

private:
    QString info;
};

class TacheUnitaire;
class TacheComposite;
class Projet;

class VisiteurTache{
public:
    VisiteurTache();
    virtual ~VisiteurTache();
    virtual void visiterTacheUnitaire (TacheUnitaire* tU)=0;
    virtual void visiterTacheComposite (TacheComposite* tC)=0;
    virtual void visiterProjet (Projet* p)=0;
};

class VisiteurSauvegarde:public VisiteurTache{
public :
    VisiteurSauvegarde(const QString& f):VisiteurTache(),file(f),newfile(f){
        if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
            throw CalendarException(QString("erreur sauvegarde t�ches : ouverture fichier xml"));
        stream = new QXmlStreamWriter (&newfile);
    }
    ~VisiteurSauvegarde(){}
    QString file;
    QFile newfile;
    QXmlStreamWriter* stream;
    void visiterTacheUnitaire (TacheUnitaire* tU);
    void visiterTacheComposite (TacheComposite* tC);
    void visiterProjet(Projet *p);
};

class TacheExplorer;

class Tache {
protected:
    QString identificateur;
    QString titre;
    QTime duree;
    QDate disponibilite;
    QDate echeance;
    bool programmee;
    Tache(const Tache& t);
    Tache& operator=(const Tache&);
    friend class TacheExplorer;
    TacheExplorer* tachesPrecedentesTraitement;
    TacheExplorer* tachesPrecedentesAffichage;
public:
    Tache(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline,bool program=false):
        identificateur(id),titre(t),duree(dur),disponibilite(dispo),echeance(deadline),programmee(program){}
    virtual ~Tache();
    virtual void ajouterTachePrecedenteA(Tache* u)=0;
    virtual void ajouterTacheUnitaire(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt)=0;
    virtual void ajouterTacheComposite(const QString& id, const QString& t)=0;
    virtual void accept(VisiteurTache* v)=0;
    virtual bool isUnitaire()=0;
    virtual bool isProjet(){return false;}
    QString getId() const { return identificateur; }
    void setId(const QString& str);
    QString getTitre() const { return titre; }
    void setTitre(const QString& str) { titre=str; }
    QTime getDuree() const { return duree; }
    QDate getDateDisponibilite() const {return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    bool getProgrammee()const{return programmee;}
    TacheExplorer* getTachesPrecedentesPourAffichage()const{return tachesPrecedentesAffichage;}
    TacheExplorer* getTachesPrecedentesPourTraitement()const{return tachesPrecedentesTraitement;}
};

class TacheExplorer {
protected:
    Tache** taches;
    unsigned int nb;
    unsigned int nbMax;
    Tache* trouverTache(const QString& id) const;
    void ajouterTacheUnitaire(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    TacheExplorer& operator=(const TacheExplorer& um);
public:
    TacheExplorer():nb(0),nbMax(0){
        taches = new Tache*[nb];
    }
    ~TacheExplorer();
    TacheExplorer(const TacheExplorer& um);
    Tache& getTache(const QString& id);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    //const Tache& getTache(const QString& code) const;
    void addItem(Tache* t);
    void supprimerTache(const QString& id);
    void concatSansRedondance(const TacheExplorer* tE);

    class Iterator{
        Iterator(unsigned int nb,Tache** u):nbRemain(nb), currentTache(u){}
        unsigned int nbRemain;
        Tache** currentTache;
        friend class TacheExplorer;
    public:
        Iterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentTache++;
        }
        Tache* current2() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return *currentTache;
        }
    };
    Iterator& getIterator()const{
        Iterator* it = new Iterator (this->nb,this->taches);
        return *it;
    }
    class iterator {
        Tache** current;
        iterator(Tache** u):current(u){}
        friend class TacheExplorer;
    public:
        iterator():current(0){}
        Tache& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current!=it.current; }
        iterator& operator++(){ ++current; return *this; }
    };
    iterator begin() { return iterator(taches); }
    iterator end() { return iterator(taches+nb); }
};

class TacheManager:public TacheExplorer {
private:

    QString file;
    TacheManager();
    ~TacheManager();
    TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
    struct Handler{
        TacheManager* instance;
        Handler():instance(0){}
        // destructeur appel� � la fin du programme
        ~Handler(){ if (instance) delete instance; }
    };
    static Handler handler;
public:
    void load(const QString& f);
    void save(const QString& f);
    TacheExplorer& getTachesNonProgrammees();
    static TacheManager& getInstance();
    static void libererInstance();

    class DisponibiliteFilterIterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        QDate dispo;
        DisponibiliteFilterIterator(Tache** u, unsigned nb, const QDate& d):currentTache(u),nbRemain(nb),dispo(d){
            while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite()){
                nbRemain--; currentTache++;
            }
        }
    public:
        DisponibiliteFilterIterator():currentTache(0), nbRemain(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            do {
                nbRemain--; currentTache++;
            }while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite());
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    DisponibiliteFilterIterator getDisponibiliteFilterIterator(const QDate& d) {
        return DisponibiliteFilterIterator(taches,nb,d);
    }
};

class TacheComposite:public Tache{
public:
    TacheComposite (const QString& id, const QString& t):
        Tache(id,t,QTime(0,0),QDate(0,0,0),QDate(0,0,0)){
        tachesPrecedentesTraitement = new TacheExplorer();
        tachesPrecedentesAffichage = new TacheExplorer();
        sousTaches = new TacheExplorer();
        updateAttributs();
    }
    virtual ~TacheComposite();
    TacheExplorer* sousTaches;
    virtual void accept(VisiteurTache* v){v->visiterTacheComposite(this);}
    virtual void ajouterTacheComposite(const QString& id, const QString& t);
    virtual void ajouterTacheUnitaire(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt);
    virtual bool isUnitaire (){return false;}
    virtual void ajouterTachePrecedenteA(Tache* u);
    QTime findDuree() const;
    QDate findDispo() const;
    QDate findEcheance() const;
    bool findAllProgrammed()const;
    void updateTachesPrecedentes();
    void updateAttributs();

    void ajouterTacheExistante(Tache* t);

};

class Projet:public TacheComposite{
public:
    Projet(const QString& id, const QString& t,const QString& des):
        TacheComposite(id,t),description(des){}
    virtual ~Projet();
private:
    QString description;
};

class TacheUnitaire:public Tache{
    bool preemptive;
    bool inTree;
public:
    TacheUnitaire (const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool program=false,bool preemp=false,bool iT=false):
        Tache(id,t,dur,dispo,deadline,program),preemptive(preemp),inTree(iT){
        tachesPrecedentesAffichage = new TacheExplorer();
        tachesPrecedentesTraitement = new TacheExplorer();
    }
    virtual ~TacheUnitaire();
    void accept(VisiteurTache* v){v->visiterTacheUnitaire(this);}
    void setDuree(const QTime& d) {
        if(!preemptive && d.minute()>D_NON_PREEMPT.minute()) throw CalendarException("Duree trop elevee (>12h) pour une tache non-preemptive");
        duree=d;
    }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tache : date echeance < date disponibilite");
        if (disp>DATE_MAX || disp<DATE_MIN) throw CalendarException("erreur Tache : disponibilite hors limites de temps");
        if (e>DATE_MAX || e<DATE_MIN) throw CalendarException("erreur Tache : echeance hors limites de temps");
        disponibilite=disp; echeance=e;}
    void ajouterTachePrecedenteA(Tache* u);
    void ajouterTachePrecedente(Tache* tP);
    virtual void ajouterTacheComposite(const QString&, const QString&){
        throw CalendarException("erreur Tache : impossible d'ajouter une tache à une tache unitaire");
    }
    virtual void ajouterTacheUnitaire(const QString&, const QString&, const QTime&, const QDate&, const QDate&, bool){
        throw CalendarException("erreur Tache : impossible d'ajouter une tache à une tache unitaire");
    }
    bool isUnitaire (){return true;}
    bool isPreemptive() const { return preemptive; }
    void setPreemptive() { preemptive=true; }
    void setNonPreemptive() { preemptive=false; }
    bool isInTree() const { return inTree; }
    void setInTree() { inTree=true; }
    void setNonInTree() { inTree=false; }
};



/*class FabriqueTaches
//plus tard
}*/

/*class FacadeGP {

}*/
#endif // GESTIONPROJET

