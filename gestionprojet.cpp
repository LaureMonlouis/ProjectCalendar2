#include "gestionprojet.h"
#include <QTextCodec>
#include <iostream>

using namespace std;
//#include <QMessageBox>



/*void Tache::setId(const QString& str){
  if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id d�j� existante");
  identificateur=str;
}
*/

/*
///On définit la division d'une duree par un entier. La duree obtenue doit être multiple de 15.
Duree Duree::div(unsigned int n) const{
    Duree dur=*this;
    unsigned int u=dur.getDureeEnMinutes()/n;
    if (u%15<8) u=u-u%15;
    if (u%15>7) u=u+(15-u%15);
    dur.setDuree(u);
    return dur;

}
*/

QTextStream& operator<<(QTextStream& fout, const Tache& t){
    fout<<t.getId()<<"\n";
    fout<<t.getTitre()<<"\n";
    fout<<t.getDuree().toString()<<"\n";
    fout<<t.getDateDisponibilite().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
    return fout;
}

VisiteurTache::VisiteurTache(){}
VisiteurTache::~VisiteurTache(){}

void TacheManager::VisiteurSauvegarde::visiterProjet(Projet* p){
    std::cout<<"Sauvegarde commence";
    stream->setAutoFormatting(true);
    stream->writeStartDocument();
    stream->writeStartElement("Projet");
    stream->writeTextElement("identificateur",p->getId());
    stream->writeTextElement("titre",p->getTitre());
    stream->writeTextElement("description",p->getDescription());
    TacheExplorer::Iterator it = p->sousTaches->getIterator();
    while(!it.isDone()){
        it.current2()->accept(this);
        it.next();
    }
    stream->writeEndElement();
    stream->writeEndDocument();
    newfile.close();
}

void TacheManager::VisiteurSauvegarde::visiterTacheUnitaire (TacheUnitaire* tU)
{
    stream->writeStartElement("Tache unitaire");
    //stream->writeAttribute("preemptive", (taches[i.current2()]->isPreemptive())?"true":"false");
    stream->writeTextElement("identificateur",tU->getId());
    stream->writeTextElement("titre",tU->getTitre());
    stream->writeTextElement("disponibilite",tU->getDateDisponibilite().toString(Qt::ISODate));
    stream->writeTextElement("echeance",tU->getDateEcheance().toString(Qt::ISODate));
    QString str;
    str.setNum(tU->getDuree().minute());
    stream->writeTextElement("duree",str);
    if (tU->getProgrammee())
        stream->writeTextElement("programmee","1");
    else
        stream->writeTextElement("programmee","0");
    stream->writeEndElement();

}

void TacheManager::VisiteurSauvegarde::visiterTacheComposite (TacheComposite* tC)
{
    stream->writeStartElement("Tache composite");
    stream->writeTextElement("identificateur",tC->getId());
    stream->writeTextElement("titre",tC->getTitre());
    TacheExplorer::Iterator it = tC->sousTaches->getIterator();
    while(!it.isDone()){
        it.current2()->accept(this);
        it.next();
    }
    stream->writeEndElement();
}

Tache::~Tache(){
}
TacheUnitaire::~TacheUnitaire(){}
TacheComposite::~TacheComposite(){
    delete sousTaches;
}
Projet::~Projet(){
}
/**
 * @brief TacheExplorer::concatSansRedondance
 * @param tE
 * Permet d'ajouter une liste de tache dans un TacheExplorer à la liste de Tache du TacheExplorer courant.
 * On n'autorise pas ici l'ajout d'une tache deja existante dans le TacheExplorer.
 */
void TacheExplorer::concatSansRedondance(const TacheExplorer* tE){
    Iterator it = tE->getIterator();
    while(!it.isDone()){
        if (!isTacheExistante(it.current2()->getId())){
            addItem(it.current2());
        }
        it.next();
    }
}

void TacheExplorer::addItem(Tache* t){
    if (nb==nbMax){
        Tache** newtab=new Tache*[nbMax+10];
        for(unsigned int i=0; i<nb; i++) newtab[i]=taches[i];
        // ou memcpy(newtab,taches,nb*sizeof(Tache*));
        nbMax+=10;
        Tache** old=taches;
        taches=newtab;
        delete[] old;
    }
    taches[nb++]=t;
}

Tache* TacheExplorer::trouverTache(const QString& id)const{
    for(unsigned int i=0; i<nb; i++)
        if (id==taches[i]->getId()) return taches[i];
    return 0;
}

void TacheExplorer::ajouterTacheUnitaire(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (trouverTache(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheUnitaire* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt,false);
    addItem(newt);
}

void TacheExplorer::supprimerTache(const QString& id){
    Tache* t=&getTache(id);
    delete t;
}
Tache& TacheExplorer::getTache(const QString& id){
    Tache* t=trouverTache(id);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}


/*const Tache& TacheManager::getTache(const QString& id)const{
    return const_cast<TacheManager*>(this)->getTache(id);
}
*/

TacheExplorer::~TacheExplorer(){
    //if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    //file="";
}

TacheManager::TacheManager():TacheExplorer(){}
TacheManager::~TacheManager(){
    //if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    file="";
}


TacheExplorer& TacheManager::getTachesNonProgrammees(){
    TacheExplorer* tENonProg= new TacheExplorer();
    TacheExplorer::Iterator i = getInstance().getIterator();
    while (!i.isDone()){
        if (i.current2()->isUnitaire()&& !i.current2()->getProgrammee())
            tENonProg->addItem(i.current2());
        i.next();
    }
    return *tENonProg;
}

void TacheManager::load(const QString& f){
    //qDebug()<<"debut load\n";
    this->~TacheManager();
    file=f;
    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CalendarException("Erreur ouverture fichier taches");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "taches") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "tache") {
                qDebug()<<"new tache\n";
                QString identificateur;
                QString titre;
                QDate disponibilite;
                QDate echeance;
                QTime duree;
                bool preemptive;

                QXmlStreamAttributes attributes = xml.attributes();
                // Let's check that Task has attribute.
                if(attributes.hasAttribute("preemptive")) {
                    QString val =attributes.value("preemptive").toString();
                    preemptive=(val == "true" ? true : false);
                }
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tache")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found identificteur.
                        if(xml.name() == "identificateur") {
                            xml.readNext(); identificateur=xml.text().toString();
                            //qDebug()<<"id="<<identificateur<<"\n";
                        }

                        // We've found titre.
                        if(xml.name() == "titre") {
                            xml.readNext(); titre=xml.text().toString();
                            //qDebug()<<"titre="<<titre<<"\n";
                        }
                        // We've found disponibilite
                        if(xml.name() == "disponibilite") {
                            xml.readNext();
                            disponibilite=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"disp="<<disponibilite.toString()<<"\n";
                        }
                        // We've found echeance
                        if(xml.name() == "echeance") {
                            xml.readNext();
                            echeance=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"echeance="<<echeance.toString()<<"\n";
                        }
                        // We've found duree
                        if(xml.name() == "duree") {
                            xml.readNext();
                            duree.setHMS(0,xml.text().toString().toUInt(),0);
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                //qDebug()<<"ajout tache "<<identificateur<<"\n";
                ajouterTacheUnitaire(identificateur,titre,duree,disponibilite,echeance,preemptive);

            }
        }
    }
    // Error handling.
    if(xml.hasError()) {
        throw CalendarException("Erreur lecteur fichier taches, parser xml");
    }
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    //qDebug()<<"fin load\n";
}

TacheManager::Handler TacheManager::handler=TacheManager::Handler();

TacheManager& TacheManager::getInstance(){
    if (handler.instance==0) handler.instance=new TacheManager;
    return *(handler.instance);
}

void TacheManager::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}

///L'ajout d'une tache unitaire dans une tache composite implique la copie de la référence de cette tache dans le tache manager
void TacheComposite::ajouterTacheUnitaire(const QString& id, const QString& t, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    if (sousTaches->isTacheExistante(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheUnitaire* newt=new TacheUnitaire(id,t,dur,dispo,deadline,preempt,true);
    sousTaches->addItem(newt);
    updateAttributs();
    TacheManager::getInstance().addItem(newt);
}

void TacheComposite::ajouterTacheComposite(const QString& id, const QString& t){
    if (sousTaches->isTacheExistante(id)) throw CalendarException("erreur, TacheExplorer, tache deja existante");
    TacheComposite* newt=new TacheComposite(id,t);
    sousTaches->addItem(newt);
    updateAttributs();
    TacheManager::getInstance().addItem(newt);
}

void TacheComposite::ajouterTacheExistante(Tache* t){
    if(t->isUnitaire())
        if(dynamic_cast<TacheUnitaire*>(t)->isInTree())
            throw CalendarException ("Erreur : Cette tache appartient déjà à un projet");
    sousTaches->addItem(t);
    updateAttributs();
}


QTime TacheComposite::findDuree()const
{
    TacheExplorer::Iterator i = sousTaches->getIterator();
    QTime d(0,0);
    while(!i.isDone())
    {
        d.addSecs(+i.current2()->getDuree().second());
        i.next();
    }
    return d;
}

QDate TacheComposite::findDispo() const{
    TacheExplorer::Iterator i=sousTaches->getIterator();
    QDate d=DATE_MAX;
    while(!i.isDone()){
        if(i.current2()->getDateEcheance()<d)
        {
            d=i.current2()->getDateEcheance();
        }
        i.next();
    }
    return d;
}

QDate TacheComposite::findEcheance() const{
    TacheExplorer::Iterator i=sousTaches->getIterator();
    QDate d=DATE_MIN;
    while(!i.isDone()){
        if(i.current2()->getDateEcheance()>d)
        {
            d=i.current2()->getDateEcheance();
        }
        i.next();
    }
    return d;
}


bool TacheComposite::findAllProgrammed()const{
    bool d=true;
    TacheExplorer::Iterator i = sousTaches->getIterator();
    while(!i.isDone()){
        if (i.current2()->getProgrammee()==false)
        {
            d=false;
        }
        i.next();
    }
    return d;
}

/**
 * @brief TacheComposite::updateAttributs
 * La plupart des attributs d'une tache composite dépend uniquement de
 * ceux des taches qui la compose, ainsi on apelle cette méthode à chaque fois qu'un attribut
 * des classes filles est modifié.
 */
void TacheComposite::updateAttributs(){
    duree=findDuree();
    disponibilite=findDispo();
    echeance=findEcheance();
    programmee=findAllProgrammed();
    updateTachesPrecedentes();
}



bool VisiteurAParenteDeB::result(){
    TacheExplorer::Iterator it = parente->sousTaches->getIterator();
    while(!it.isDone()){
        it.current2()->accept(this);
        it.next();
    }
    return leResultat;
}

void VisiteurAParenteDeB::visiterTacheComposite(TacheComposite*t){
    if (t->getId()==fille->getId())
        leResultat = true;
    TacheExplorer::Iterator it = t->sousTaches->getIterator();
    while(!it.isDone()){
        it.current2()->accept(this);
        it.next();
    }

}
void VisiteurAParenteDeB::visiterTacheUnitaire(TacheUnitaire *t){
    if (t->getId()==fille->getId())
        leResultat = true;
}

void TacheManager::VisiteurTreeModel::visiterTacheUnitaire (TacheUnitaire* tU){
    ofstream str1(file.toStdString().c_str(), ios::out | ios::app);
    indiceIndentation++;
    str1<<indentation(indiceIndentation);
    string chaine = tU->getTitre().toStdString()+"\t"+tU->getId().toStdString()+"\t"+tU->getDuree().toString().toStdString()+"\t"+tU->getDateDisponibilite().toString().toStdString()+"\t"+tU->getDateEcheance().toString().toStdString()+"\t";
    str1<<chaine;
         if(tU->getProgrammee()){
            str1<<"Oui"<<"\t";
         }
         else
            str1<<"Non"<<"\t";
         if(tU->isPreemptive()){
            str1<<"Oui";
         }
         else
            str1<<"Non";
    str1<<endl;
    str1.close();
    indiceIndentation--;
}
void TacheManager::VisiteurTreeModel::visiterTacheComposite (TacheComposite* tC){
    ofstream str2(file.toStdString().c_str(), ios::out | ios::app);
    indiceIndentation++;
    str2<<indentation(indiceIndentation);
    string chaine = tC->getTitre().toStdString()+"\t"+tC->getId().toStdString()+"\t"+tC->getDuree().toString().toStdString()+"\t"+tC->getDateDisponibilite().toString().toStdString()+"\t"+tC->getDateEcheance().toString().toStdString()+"\t";
    str2<<chaine;
         if(tC->getProgrammee()){
            str2<<"Oui";
         }
         else
            str2<<"Non";
    str2<<endl;
    str2.close();
    Iterator it = tC->sousTaches->getIterator();
    while (!it.isDone()){
        it.current2()->accept(this);
        it.next();
    }
    indiceIndentation--;
}
void TacheManager::VisiteurTreeModel::visiterProjet(Projet *p){
    ofstream str(file.toStdString().c_str(), ios::out | ios::trunc);
    if (!str)
        throw CalendarException(QString("erreur creation d'arbre : fichier inexistant"));
    str.close();
    Iterator it = p->sousTaches->getIterator();
    while (!it.isDone())
    {
        it.current2()->accept(this);
        it.next();
    }
}
string TacheManager::VisiteurTreeModel::indentation(unsigned int n){
    string f ="";
    for (unsigned int i=0;i<n;i++)
        f=f+"  ";
    return f;
}




/**
 * @brief TacheUnitaire::ajouterTachePrecedente
 * @param tP
 * Ici, la tache est ajoutée comme tache précédente à une tache unitaire.
 * Ses taches précédentes pour traitement sont
 * concaténnées sans redondance à celles de la tache courante.
 */
void TacheUnitaire::ajouterTachePrecedente(Tache* tP){
    if (!tP->isUnitaire())
    {
    VisiteurAParenteDeB vS (dynamic_cast<TacheComposite*>(tP),this);
    if(vS.result())
        throw CalendarException("Erreur Precedence : la tache à ajouter contient la tache courante");
    }
    tP->ajouterTachePrecedenteA(this);
    tachesPrecedentesAffichage->addItem(tP);
}


void TacheComposite::ajouterTachePrecedenteA(Tache* u){
        u->getTachesPrecedentesPourTraitement()->addItem(this);
    }

void TacheUnitaire::ajouterTachePrecedenteA(Tache* u){
        u->getTachesPrecedentesPourTraitement()->concatSansRedondance(this->tachesPrecedentesTraitement);
    }

void TacheComposite::updateTachesPrecedentes(){
    TacheExplorer::Iterator it = sousTaches->getIterator();
    while(!it.isDone()){
        it.current2()->ajouterTachePrecedenteA(this);
        tachesPrecedentesAffichage->addItem(it.current2());
        it.next();
    }
}
