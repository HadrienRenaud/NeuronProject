#include "Network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

Network::Network():m_totalBindingsNumber(0),m_initialized(false),m_gradientInitialized(false),m_firstLayer(0){
    m_momentum = ALPHA;
}

Network::Network(string nom_fichier):m_totalBindingsNumber(0),m_initialized(false),m_gradientInitialized(false){
    recuperateur(nom_fichier);
    m_momentum = ALPHA;
}

Network::~Network(){
    delete m_firstLayer;
}

void Network::recup(string nom_fichier){
    //if (m_firstLayer!=nullptr){delete m_firstLayer;} //Si le réseau est non vide, on le vide
    recuperateur(nom_fichier);
}

void Network::recuperateur(string nom_fichier){
    ifstream file(nom_fichier.c_str()); // On ouvre le fichier
    int nbTotalLayer, lengthLayer,neurbis,i,j;
    double weight;
    char* pEnd(0);
    Neuron* neurone;
    string ligne, weight_str, neurone_str;
    Layer *layer,*previousLayer(0);
    file >> nbTotalLayer; // On lit le nombre total de couches
    file >> lengthLayer; // On lit le nombre de neurones de la premiere couche, dont on a pas besoin de récupérer les liasons
    while(getline(file,ligne)){ // pour chaque couche
        //cout << "Network : Network1 layer " << layer << " previousLayer " << previousLayer <<endl ;
        istringstream ligne_stream(ligne); //objet crée pour le traitement de la ligne récupérée
        layer = new Layer(this,lengthLayer,previousLayer); // nouvelle couche créée
        i=0; // initialisation de l'indice du neurone recevant la liaison sur le couche layer
        //cout << "Network : Network2 layer " << layer << " previousLayer " << previousLayer <<endl ;
        if ( previousLayer != 0 ){ // si on est pas sur la première couche
            while (getline(ligne_stream,neurone_str,',')) // pour tout neurone de la couche
            {
                j=0; // indice de la liaison arrivant au neurone considéré
                istringstream neurone_stream(neurone_str); // objet créé pour le traitement du string récupéré
                //cout << "Network : Network3 while neurone_str " << neurone_str << endl ;
                neurone = layer -> getNeuron(i); // neurone considéré
                while (getline(neurone_stream,weight_str,' ')) // pour toute liaison arrivant au neurone
                {
                    if(weight_str.size()){ // si ce n'est pas un faux poids vide
                        weight=strtod(weight_str.c_str(),&pEnd); //on récupère le poids de la liaison
                        //cout << "Network : Network3 while weight_str " << weight_str << " weight " << weight << endl ;
                        //cout << "Network : Network3 while neurone " << neurone << " weight " << weight << endl ;
                        neurone->getBinding(j)->setWeight(weight); //on change le poids de la liaison
                    j++;
                    }
                }
                i++;
            }
        }
        previousLayer=layer;
        //cout << "Network : Network4 layer " << layer << " previousLayer " << previousLayer <<endl ;
        file >> lengthLayer; // on lit la longueur de la prochaine couche
    }
}
char* Network::save(char lettre_testee){
    char* nom_fichier;
    return save(lettre_testee,nom_fichier);
}
char* Network::save(char* char_nom_fichier){return save('_',char_nom_fichier);}
char* Network::save(char lettre_testee,char* char_nom_fichier){
    // On trouve le nom du fichier
    time_t t = time(0); // l'heure
    ostringstream temps; //flux pour traitement
    temps << asctime(localtime(&t)); //on récupère le flux correspondant au temps
    string nom_fichier(temps.str()); // on crée un string correspondant au flux
    for(int i(0);i<nom_fichier.size();i++){ // on parcourt le nom du fichier
        if(!isalnum(nom_fichier[i])){   // et on remplace tout ce qui ne va pas dans un
            nom_fichier[i]='_';         // nom de fichier par '_'
        }
    }
    string dir_svg(g_dir_svg);
    nom_fichier= dir_svg + nom_fichier + "_" + lettre_testee + g_extension_svg ; // adresse complete
    //on écrit dans le fichier
    ofstream file(nom_fichier.c_str()); // flux sortant dans le fichier
    file << this->getTotalLayerNumber() << ' '; // on entre le nombre total de couches
    Layer *layer(this->getFirstLayer()); // on initialise la premiere couche
    file << layer -> getSize() << ' ';   // en donnant sa longueur
    Neuron *neurone;
    Binding *binding;
    while (layer->getNextLayer()!=0){//pour toute couche
        layer = layer -> getNextLayer(); // on prend la suivante
        file << endl << layer -> getSize() << ' '; // on donne sa taille
        for (int i(0); i < layer->getSize();i++){ // pour tout neurone de la couche
            neurone=layer->getNeuron(i); // on récupère le neurone
            for (int j(0);j< neurone->getBindingsNumber();j++){ // pour toute liaison de la couche précédente vers ce neurone
                file << neurone->getBinding(j)->getWeight() << ' '; // on ajoute au fichier le poids de la liaison
            }
            file << ','; //séparateur
        }
    }
    //cout << "réseau enregistre dans le fichier : " << nom_fichier << endl;
    // on sauvegarde le dernier fichier enregistré dans getMostRecent.txt :
    string getMostRecent;
    getMostRecent = g_dir_svg + string(g_nom_svg) + lettre_testee;
    getMostRecent = getMostRecent + ".txt";
    ofstream fichier_recent(getMostRecent.c_str());
    fichier_recent << nom_fichier.c_str();
    //cout << "fichier " << getMostRecent << " mis à jour." <<endl;
    strcpy(char_nom_fichier, nom_fichier.c_str());
    return char_nom_fichier;
}

void Network::setFirstLayer(Layer* layer){
    if(layer){
        m_firstLayer = layer;
    }
    else
    {
        m_firstLayer = new Layer(this);
    }

}

Layer* Network::getFirstLayer()const{
    return m_firstLayer;
}

Layer* Network::getLastLayer()const{
    //cout << "getLastLayer appelée ";
    Layer* temp = m_firstLayer;
    do{
        //cout << temp <<endl;
        temp = temp->getNextLayer();
        //cout << "getLastLayer récupère " << temp << endl ;
    }while(temp->getNextLayer()!=m_firstLayer&&temp->getNextLayer()!=0);
    //cout << "end getLastLayer"<<endl;
    return temp;
}

int Network::getTotalLayerNumber(){
    Layer* temp = m_firstLayer;
    int i =0;
    do{
        temp = temp->getNextLayer();
        i++;
    }while(temp->getNextLayer()!=m_firstLayer&&temp->getNextLayer()!=0);
    return i;
}

int Network::getFirstLayerSize()const{
    return m_firstLayer->getSize();
}

void Network::initNetwork(double *inputs){
    //on initialise la premiere couche avec les inputs
    //cout << " Network : initNetwork" << getFirstLayerSize() << " : " << getFirstLayer() << endl;
    for(int i = 0; i< getFirstLayerSize(); i++){
        if (inputs[i]!=inputs[i]){cout << "Network : initNetwork : inputs [i ="<<i<<"] = " << inputs[i] << endl ; displayArray(inputs,getFirstLayerSize());}
        //cout << "coucou0"<<endl;
        //cout << getFirstLayer()->getNeuron(i) << endl;
        getFirstLayer()->getNeuron(i)->initNeuron(inputs[i]);
        //cout << "coucou1" << endl;
    }
    //puis on réinitialise tous les aures neurones du réseau
    //cout <<"Coucou" << endl;
    Layer* temp = m_firstLayer;
    do{
        //cout << temp;
        temp = temp->getNextLayer();
        temp->resetNeurons();
    }while(temp->getNextLayer()!=m_firstLayer&&temp->getNextLayer()!=0);
    m_initialized = true;
}

void Network::initNetworkGradient(double* expectedOutputs){
    //on initialise la dernière couche
    for(int i = 0; i< getLastLayer()->getSize(); i++){
        getLastLayer()->getNeuron(i)->initNeuronGradient(expectedOutputs[i]);
    }
    //dans cette boucle, on parcourt toutes les layer (sauf la dernière), et on
    //mets les gradient à zéro
    Layer* last = getLastLayer();
    Layer* temp = last;
    do{
        temp = temp->getPreviousLayer();
        temp->resetNeuronsGradient();
    }while(temp->getPreviousLayer()!=last&&temp->getPreviousLayer()!=0);
    m_gradientInitialized = true;
}

int Network::getTotalBindingsNumber()const{
        return m_totalBindingsNumber;
}

void Network::increaseTotalBindingsNumber(int n){
    m_totalBindingsNumber+=n;
}

bool Network::isALoop()const{//on regarde si le réseau se mord la queue
    Layer* temp = m_firstLayer;
    do{
        temp = temp->getNextLayer();
    }while(temp!=m_firstLayer&&temp!=0);
    if(temp)
        return true;
    else
        return false;
}

void Network::launch(double outputs[]){
    if(m_initialized && !isALoop()){//si on est dans le cas normal : le réseau n'est pas une boucle
        Layer* temp = m_firstLayer;
        while(temp->getNextLayer()!=0){
            temp = temp->getNextLayer();
            temp->calculate();
        }//lorsque sort, la dernière couche a été stimulée : c'est bon.

        //maintenant on récupère la sortie dans un tableau
        //double* outputs = new double[temp->getSize()];
        //cout << "outputs : ";
        for(int i = 0; i<temp->getSize(); i++){
            outputs[i] = temp->getNeuron(i)->getOutput();
            //cout << outputs[i] ;
        }
        //cout << endl;
        m_initialized = false;//le réseau n'est plus utilisable pour l'instant
        //return outputs;
    }
}


bool Network::launchGradient(){
    if(!m_gradientInitialized)  //on vérifie que le réseau a bien été intialisé
        return 0;               //pour les gradient
    if(!isALoop()){//si on est dans le cas normal : le réseau n'est pas une boucle
        Layer* temp = getLastLayer();
        do{
            temp->calculateGradient();//on calcule le gradient de la couche d'avant
            temp = temp->getPreviousLayer();
        }while(temp!=m_firstLayer);//jusqu'à la première
        m_gradientInitialized = false;  //le réseau n'est plus utilisable
                                        //pour le moment
        return true;
    }//pas de else, flemme de faire le cas de la boucle
    return 0;
}

bool Network::learn(){
    launchGradient();
    if(!isALoop()){//si on est dans le cas normal : le réseau n'est pas une boucle
        Layer* temp = m_firstLayer;
        while(temp!=0){//on parcourt une par une les couches, on dit à chacune de learn()
            temp->learn();
            temp = temp->getNextLayer();
        }
        return true;
    }
    return false;
}

double Network::getMomentum(){
    return m_momentum;
}

void Network::getMostRecent(){ //surcharge
    getMostRecent('_'); //default
}
void Network::getMostRecent(char lettre_testee){
    //initialisations
    string nom_fichier;
    string nom_db;

    //on reconstitue le nom du fichier :
    nom_db= string(g_dir_svg) + g_nom_svg+lettre_testee;
    nom_db=nom_db+".txt";

    ifstream file(nom_db.c_str()); //on ouvre le fichier getMostRecent.txt
    file >> nom_fichier; //on lit son contenu
    cout << "Reseau " << lettre_testee << " - recuperation du fichier : " << nom_fichier << endl;
    recup(nom_fichier); // On récupère le réseau stocké dans le fichier de svg le plus récent
}

void Network::writeReport(bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires,char lettre_testee,char* nom_fichier){
    //initialisations
    ofstream base_donnes;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    Layer* temp ;

    //Inscription des premières données
    base_donnes.open("../Compte_rendu_test_toutes.csv",ios::out|ios::app);
    timeinfo = localtime (&rawtime);
    strftime(buffer,80,"%d/%m/%y",timeinfo);
    base_donnes << buffer << ',';
    strftime(buffer,80,"%H:%M:%S",timeinfo);
    base_donnes << buffer << ","<< lettre_testee << "," << getTotalLayerNumber()+1 << ',';

    //inscription des couches
    temp = m_firstLayer;
    do
    {
        base_donnes << temp->getSize() << ',' ;
        temp = temp->getNextLayer();
    } while (temp!=0);
    for (int i(0);i<(4-(getTotalLayerNumber()));i++)
        base_donnes << ',';

    //fin de l'inscription des données
    base_donnes << count << ',' << distance_moyenne << ',' << DISTANCE_MAXIMALE  << ',';
    cout << resultat << ',' << temps_mis <<','<< commentaires << ',' << nom_fichier << endl;
}

void Network::learnNetwork(char lettre_testee, const int nb_exemples, char** tabloFichiers, double** inputs){
    clock_t t0(clock());//temps de départ du programme

    cout << "Apprentissage de la lettre " << lettre_testee << " ..." << endl;

    // Initialisations
    int exemple=0;//exemple actuellement selectionné pour l'apprentissage, cette variable est incrémenté à chaque fois qu'il réussi un exemple
    int successes = 0;//le réseau doit enchainer nb_exemples succès pour avoir fini l'apprentissage, cela ne devra pas être le cas pour les caractères manuscrits, parce qu'il y un risque de surapprentissage
    int count = 0;//nombre de passage dans la boucle
    double dist = 0;//pour stocker la moyenne des écarts quadratiques entre les valeurs attendues et les valeurs retournées
    double att_output[LAST_LAYER_SIZE];//valeur attendue en sortie du réseau
    double exp_output[LAST_LAYER_SIZE];//valeur prototypale en sortie du réseau
    char nom_fichier[MAX_LENGTH_NAME_FILE];
    double distance_totale(0);


    clock_t t1(clock());

    //APPRENTISSAGE
    while((successes<nb_exemples) && (count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples)){//tant qu'on a pas enchaîner nb_exemples succès
        //incrémentation
    	exemple ++;
    	exemple%=nb_exemples;//On ne dépasse pas nb_exemples
        count++;

        //Résultat attendu
        if(tabloFichiers[exemple][0]==lettre_testee)
            att_output[0]=1;
        else
            att_output[0] =0;

        //Calcul de la réponse du réseau
        initNetwork(inputs[exemple]); //on initialise avec les valeurs inputs
    	launch(exp_output);//on lance et on récupère les outputs

        //Calcul de l'écart
        if (count < nb_exemples * NB_APPRENTISSAGE * BORNE_MAX_CASSE && false)
            dist = distance(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart
        else
            dist = distanceMod(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart

        //On apprend, ou pas en fonction du résultat
        if(dist<DISTANCE_MAXIMALE){//si c'est assez petit, c'est un succès
            successes++;
        }else{//sinon c'est un echec et le réseau recalcule les poids des liaisons
            initNetworkGradient(att_output);
            learn();
            successes = 0;//on réinitialise aussi les nombre de succès enchaînés
        }

        // Affichages de temps en temps ...
        if(count%(NB_APPRENTISSAGE*nb_exemples)==0)//de temps en temps, on affiche dist et un poids(ça sert à rien le poids mais bon)
        {
            cout << "count = " << count << " soit " << count / nb_exemples << " boucles d'apprentissage";
            cout << " ( " << NB_APPRENTISSAGE << " boucles  en : " << ((float)(clock()-t1)/CLOCKS_PER_SEC) << " s. ) ." << endl;
            t1 = clock();
        }
    }

    //On sauvegarde le réseau
    save(lettre_testee,nom_fichier);

    //Affichages ...
    if (count >= MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples)
        cout << "apprentissage INFRUCTUEUX sur count = " << count ;
    else
        cout << "apprentissage productif : count = " << count << " sur " << MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples ;

    cout << " avec " << successes << "succes, effectué en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes" << endl;

    //Calcul de la distance moyenne
    for(exemple = 0; exemple<nb_exemples; exemple++){
        //Réponse attendue
        if(tabloFichiers[exemple][0]==lettre_testee)
            att_output[0]=1;
        else
            att_output[0] =0;
        //Réponse du réseau
        initNetwork(inputs[exemple]);
        launch(exp_output);

        //Calcul de la distance
        distance_totale+=distance(exp_output, att_output , LAST_LAYER_SIZE );
    }
    cout << "distance moyenne sur les exemples : " << distance_totale / nb_exemples << endl ;

    //Calcul du temps mis
    double temps_mis(((float)(clock()-t0)/CLOCKS_PER_SEC));
    cout << "Apprentissage effectué en " << temps_mis << " secondes" << endl;

    // On met à jour les données dans le fichier
    writeReport((count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples),count/nb_exemples,distance_totale/nb_exemples,temps_mis," ",lettre_testee,nom_fichier);
}

template <class T>
void displayArray(T* data, int length){//afficher un tableau de valeur
    cout << "[";
    for(int i = 0; i<length-1; i++)
        cout << (data[i]>=0 ? "+" : "") << data[i] << "," ;
    cout << (data[length-1]>=0 ? "+" : "") << data[length-1] << "]";
}

double distance(double* data1, double* data2, int length){//on fait la moyenne des carrés de chaque écart entre data1 et data2
    double res = 0; //initialisation
    for(int i = 0; i<length; i++) //on parcourt la liste
        res+=((data1[i]-data2[i])*(data1[i]-data2[i])); //On augmente la moyenne du carré de la distance entre data1 et data2 ~ Variance
    res/=length; // On moyenne
    return res;
}

double distanceMod(double* data1, double* data2, int length){
    double res=0; //initialisation
    int j=-1; //initialisation
    for (int i=0; i<length; i++){
        res += ((data1[i]-data2[i])*(data1[i]-data2[i])); //carré de la distance
        j = (i+26)%length; //position de la majuscule
        res += ((data1[i]-data2[j])*(data1[i]-data2[j])); //carré de la distance à la majuscule
    }
    res/=length*2; // on moyenne
    return res;
}
