#include "apprentissage.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <dirent.h>     //Pour la gestion du répertoire d'images
#include <cstring>
#include <time.h>
using namespace std;


const char* g_dir_exemples("texts/"  ); //dossier ou sont contenus tous les fichiers d'exemples
const char* g_dir_test("test/");
const char g_alphabet[LENGTH_ALPHABET] {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

template <class T>
void displayArray(T* data, int length){//afficher un tableau de valeur
    cout << "[";
    for(int i = 0; i<length-1; i++)
        cout << (data[i]>=0 ? "+" : "") << data[i] << "," ;
    cout << (data[length-1]>=0 ? "+" : "") << data[length-1] << "]";
}

double distance(double* data1, double* data2, int length){//on fait la moyenne des carrés de chaque écart entre data1 et data2
    double res = 0;
    for(int i = 0; i<length; i++)
        res+=((data1[i]-data2[i])*(data1[i]-data2[i]));
    res/=length;
    return res;
}

double distanceMod(double* data1, double* data2, int length){
    double res=0;
    int j;
    for (int i=0; i<length; i++){
        res += ((data1[i]-data2[i])*(data1[i]-data2[i]));
        j = (i+26)%length;
        res += ((data1[i]-data2[j])*(data1[i]-data2[j]));
    }
    res/=length*2;
    return res;
}

bool lecture_example(char* nom_fichier, double entrees[], int taille_entree){ //surcharge, appelle lecture_example
    return lecture_example(nom_fichier,entrees,taille_entree,g_dir_exemples); //par défaut on lit les exemples dans g_dir_exemples
}
bool lecture_example(char* nom_fichier,double entrees[], int taille_entree,const char* directory){
    ifstream file((string(directory)+string(nom_fichier)).c_str()); //ouverture du fichier
    if (!file){
        ofstream file("erreur.txt");
        file << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        cout << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        return false;
    }
    else{
        //cout << "lecture de " << nom_fichier << endl;
        for (int i(0);i<taille_entree;i++)
            file >> entrees[i]; // on insère les valeurs dans entrees
        return true;
    }
}

int compteurExemples(){ //surcharge
    return compteurExemples(g_dir_exemples); //par défaut ...
}
int compteurExemples(const char* directory){
    int nombre_fichiers(0);
    DIR* dp;
    struct dirent* ep;
    dp = opendir(directory);    //On ouvre le répertoire contenant les images avant de le parcourir
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
        {
            if (strlen(ep->d_name) >= 4) //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
                nombre_fichiers++;
        }
        closedir (dp);   //Fermeture du répertoire d'images
    }
    return nombre_fichiers;
}

void getTabloFichier(char** tabloFichiers){ //surcharge
    getTabloFichier(tabloFichiers,g_dir_exemples); //par déa
}
void getTabloFichier(char** tabloFichiers,const char* directory){
    int compteur(0);
    DIR* dp;
    struct dirent* ep;
    dp = opendir(directory);    //On ouvre le répertoire contenant les images avant de le parcourir
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
        {
            if (strlen(ep->d_name) >= 4){ //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
                strcpy(tabloFichiers[compteur],ep->d_name); //on inscrit le nom du fichier dans le tablo
                compteur++;
            }
        }
        closedir (dp);   //Fermeture du répertoire d'images
    }
}

void getTabloExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples){
    getTabloExemples(tabloFichiers, tabloExemple, nb_exemples , g_dir_exemples);
}
void getTabloExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples,const char* directory){
    cout << "Lecture des exemples ... " << flush;
    for (int i(0); i< nb_exemples;i++)
        lecture_example(tabloFichiers[i],tabloExemple[i],FIRST_LAYER_SIZE,directory); //on lit chacun des exemples
    cout << "Lecture terminee." <<endl;
}

void plus_recent(Network* net){ //surcharge
    plus_recent(net, '_'); //default
}
void plus_recent(Network* net , char lettre_testee){
    //on reconstitue le nom du fichier :
    string nom_fichier;
    string nom_db;
    nom_db= string(g_dir_svg) + g_nom_svg+lettre_testee;
    nom_db=nom_db+".txt";

    ifstream file(nom_db.c_str()); //on ouvre le fichier plus_recent.txt
    file >> nom_fichier; //on lit son contenu
    cout << "Reseau " << lettre_testee << " - recuperation : " << nom_fichier << endl;
    net->recup(nom_fichier); // On récupère le réseau stocké dans le fichier de svg le plus récent
}

void write_compte_rendu(Network* net, bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires,char lettre_testee,char* nom_fichier){
    ofstream base_donnes;
    base_donnes.open("../Compte_rendu_test_toutes.csv",ios::out|ios::app);
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buffer,80,"%d/%m/%y",timeinfo);
    base_donnes << buffer << ',';
    strftime(buffer,80,"%H:%M:%S",timeinfo);
    base_donnes << buffer << ","<< lettre_testee << "," << net->getTotalLayerNumber()+1 << ',';

    Layer* temp ;
    temp = net->getFirstLayer();
    do
    {
        base_donnes << temp->getSize() << ',' ;
        temp = temp->getNextLayer();
    } while (temp!=0);
    for (int i(0);i<(4-(net->getTotalLayerNumber()));i++)
        base_donnes << ',';
    base_donnes << count << ',' << distance_moyenne << ',' << DISTANCE_MAXIMALE << ',' << resultat << ',' << temps_mis <<','<< commentaires << ',' << nom_fichier << endl;
}

void apprend(Network* net,char lettre_testee, const int nb_exemples, char** tabloFichiers, double** inputs){
    clock_t t0(clock());//temps de départ du programme

    cout << "Apprentissage de la lettre " << lettre_testee << " ..." << endl;

    int exemple=0;//exemple actuellement selectionné pour l'apprentissage, cette variable est incrémenté à chaque fois qu'il réussi un exemple
    int successes = 0;//le réseau doit enchainer nb_exemples succès pour avoir fini l'apprentissage, cela ne devra pas être le cas pour les caractères manuscrits, parce qu'il y un risque de surapprentissage
    int count = 0;//nombre de passage dans la boucle
    double dist = 0;//pour stocker la moyenne des écarts quadratiques entre les valeurs attendues et les valeurs retournées
    double att_output[LAST_LAYER_SIZE];//valeur attendue en sortie du réseau
    double exp_output[LAST_LAYER_SIZE];//valeur prototypale en sortie du réseau

    clock_t t1(clock());

    while((successes<nb_exemples) && (count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples)){//tant qu'on a pas enchaîner nb_exemples succès
    	//if(successes!=0)//si l'on a réussi l'exemple précédent on passe au suivant
    	exemple ++;
    	exemple%=nb_exemples;//On ne dépasse pas nb_exemples
    	//cout << "ok0" <<endl;

        if(tabloFichiers[exemple][0]==lettre_testee){ att_output[0]=1; }
        else{ att_output[0] =0; }
        net->initNetwork(inputs[exemple]); //on initialise avec les valeurs inputs
    	//cout << "ok1" <<endl;
        net->launch(exp_output);//on lance et on récupère les outputs
        //cout << exp_output << " : " << &exp_output <<endl << att_output << " : " << &att_output<< endl ;
        if (count < nb_exemples * NB_APPRENTISSAGE * BORNE_MAX_CASSE && false)
            dist = distance(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart
        else
            dist = distanceMod(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart
    	//cout << "ok2" <<endl;
        if(dist<DISTANCE_MAXIMALE){//si c'est assez petit, c'est un succès
            successes++;
        }else{//sinon c'est un echec et le réseau recalcule les poids des liaisons
            //cout << "ok3" << endl;
            net->initNetworkGradient(att_output);
            net->learn();
            successes = 0;//on réinitialise aussi les nombre de succès enchaînés
        }

        count++;
        if(count%(NB_APPRENTISSAGE*nb_exemples)==0)//de temps en temps, on affiche dist et un poids(ça sert à rien le poids mais bon)
        {
            cout << "count = " << count << " soit " << count / nb_exemples << " boucles d'apprentissage";
            cout << " ( " << NB_APPRENTISSAGE << " boucles  en : " << ((float)(clock()-t1)/CLOCKS_PER_SEC) << " s. ) ." << endl;
            t1 = clock();
            //cout << "dist :  "   << dist << endl;
            //cout << "Weight : " << net-> getLastLayer()->getNeuron(0)->getBinding(0)->getWeight() << endl;
            //cout << "Weight1 : " << net-> getLastLayer()->getNeuron(0)->getBinding(1)->getWeight() << endl;
        }
    }
    char nom_fichier[MAX_LENGTH_NAME_FILE];
    net->save(lettre_testee,nom_fichier);

    cout << "apprentissage ok : count = " << count << " sur " << MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples ;
    cout << " avec " << successes << "succes, effectue en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes" << endl;
    //quelques infos
    cout << "dist :  "   << dist << endl;
    cout << "count : " << count << endl;

    double distance_totale(0);

    //un fois que le réseau répond correctement à toutes les entrées, on affiche les sortie réelles
    for(exemple = 0; exemple<nb_exemples; exemple++){
        net->initNetwork(inputs[exemple]);
        net->launch(exp_output);
        /*
        cout << tabloFichiers[exemple] ;
        //displayArray(inputs[exemple], FIRST_LAYER_SIZE);
        cout << "\t=>\t";
        displayArray(exp_output, *length);
        cout << endl;
        */
        if(tabloFichiers[exemple][0]==lettre_testee){ att_output[0]=1; }
        else{ att_output[0] =0; }
        distance_totale+=distance(exp_output, att_output , LAST_LAYER_SIZE );
    }

    cout << "distance moyenne sur les exemples : " << distance_totale / nb_exemples << endl ;

    /*TODO : vérification
    //et on test avec une input proche de {0,1} pour voir ce que ça donne, c'est la validation
    cout << "valid : \n";
    double valid[2] = {0.1,0.9};
    net->initNetwork(valid);
    exp_output = net->launch(length);
    displayArray(valid, FIRST_LAYER_SIZE);
    cout << "\t=>\t";
    displayArray(exp_output, *length);
    cout << endl;
    */

    //on affihe le nombre de liaisons total dans les réseau
    //cout << "net bindings : "<< net->getTotalBindingsNumber() << endl;

    // On enregistre l'état du réseau
    net-> save(lettre_testee,nom_fichier);

    double temps_mis(((float)(clock()-t0)/CLOCKS_PER_SEC));
    cout << "Apprentissage effectue en " << temps_mis << " secondes" << endl;

    // On met à jour la data base
    write_compte_rendu(net,(count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples),count/nb_exemples,distance_totale/nb_exemples,temps_mis," ",lettre_testee,nom_fichier);

}

Network** createurs_reseaux(){
    Network** tablo_net = new Network*[LENGTH_ALPHABET];
    cout << "Creation des reseaux ... " << flush ;
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        tablo_net[i] = new Network;//LE réseau
        //Layer* l = new Layer(adresse du réseau, nombre de neurones dans la couche, adresse de la couche précédente, adresse de la couche suivante, fonction de transfert des neuronres de la couche);
        Layer* l1= new Layer(tablo_net[i], FIRST_LAYER_SIZE,  0,  0);
        Layer* l2= new Layer(tablo_net[i], 100, l1, 0);
        Layer* l3= new Layer(tablo_net[i], 10, l2, 0);
        Layer* lend= new Layer(tablo_net[i],LAST_LAYER_SIZE,l3, 0);
        //cout << "l1 : " << l1 << "tablo :" << tablo_net[i]->getFirstLayer() << endl;
        //cout << "tablo créé avec" << tablo_net[i]->getFirstLayerSize() << endl;
    }
    cout << "Reseaux crees !" << endl;
    return tablo_net;
}

void destructeur_reseaux(Network** tablo_net){
    char nom_fichier[MAX_LENGTH_NAME_FILE];
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        tablo_net[i]->save(g_alphabet[i],nom_fichier);
        delete tablo_net[i];
    }
    delete tablo_net;
}

void recupere_apprend(){
    cout << "recuperation des reseaux ... " << flush;


    Network** tablo_net(createurs_reseaux());
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        plus_recent(tablo_net[i],g_alphabet[i]);


    cout << "reseaux recuperes." << endl;
    apprend_tous(tablo_net);
}

Network** apprend_nouveau(){
    /*
    srand(time(0));//pour les poids de départ aléatoires
    //cout << fixed;//afficher tous les chiffres après la virgule
    clock_t t0(clock());//temps de départ du programme
    */
    cout << "Creation du reseau ... " <<flush;

    Network** tablo_net(createurs_reseaux());

    cout << "le reseau est cree."<<endl;
    apprend_tous(tablo_net);
    return tablo_net;
}

void apprend_tous(Network** tablo_net){
    cout << "Bienvenue dans le gestionnaire d'apprentissage du reseau de neurones" << endl << endl;

    cout << "Initialisation des parametres." << endl;
    clock_t t0(clock());//temps de départ du programme

    //nombre d'exemples à traiter
    int const nb_exemples(compteurExemples());

    // Les tableaux contenant les donnees des exemples
    char** tabloFichiers = new char*[nb_exemples];
    double** inputs = new double*[nb_exemples];
    for (int i = 0; i < nb_exemples; ++i)
    {
        tabloFichiers[i] = new char[MAX_LENGTH_NAME_FILE];
        inputs[i] = new double[FIRST_LAYER_SIZE];
    }
    getTabloFichier(tabloFichiers); //on récupère les noms des ficheirs d'exemples
    getTabloExemples(tabloFichiers,inputs,nb_exemples); //on récupère les donnees des exemples
    cout << "Initialisation effectuee en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes, l'apprentissage commence." <<endl <<endl;

    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        apprend(tablo_net[i],g_alphabet[i],nb_exemples,tabloFichiers,inputs);
        cout << endl << endl;
    }

}


char tester(Network** tablo_net, double input[]){
    char lettre_trouvee('_');
    double maxi(BORNE_INF_DISTINCTION);
    //double *exp_output = new double[LAST_LAYER_SIZE];//valeur prototypale en sortie du réseau
    double exp_output[LENGTH_ALPHABET][LAST_LAYER_SIZE];
    for (int i = 0; i < LENGTH_ALPHABET ; ++i)
    {
        tablo_net[i]->initNetwork(input);
        tablo_net[i]->launch(exp_output[i]);
        if (maxi<exp_output[i][0]){
            maxi=exp_output[i][0];
            lettre_trouvee=g_alphabet[i];
        }
    }
    return lettre_trouvee;
}
/*
char teste_Pierrot(char* nom_fichier, const char* directory){
    cout << "recuperation des reseaux ... " << flush;
    Network** tablo_net(createurs_reseaux());
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        plus_recent(tablo_net[i],g_alphabet[i]);
    }
    cout << "reseaux recuperes." << endl;

    double* input = new double[FIRST_LAYER_SIZE];


    cout << "Lecture de l'exemple .." << flush;
    lecture_example(nom_fichier,input,FIRST_LAYER_SIZE,directory);
    cout << "Exemple lu." << endl;

    int* length = new int;//la taille du tableau de sortie (en réalité on peut s'en passer mais bon)
    return tester(tablo_net,input,length);

}

char* recupere_teste(){return recupere_teste(g_dir_test);}
char* recupere_teste(const char* directory){
    cout << "recuperation des reseaux ... " << flush;
    Network** tablo_net(createurs_reseaux());
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        plus_recent(tablo_net[i],g_alphabet[i]);
    }
    cout << "reseaux recuperes." << endl;

    cout << "Initialisation des parametres." << endl;
    clock_t t0(clock());//temps de départ du programme
    //nombre d'exemples à traiter
    int const nb_exemples(compteurExemples(directory));
    cout << "Nombre d'exemples : " << nb_exemples <<endl;
    int* length = new int;//la taille du tableau de sortie (en réalité on peut s'en passer mais bon)
    char* outputs= new char[nb_exemples];
    // Les tableaux contenant les donnees des exemples
    char** tabloFichiers= new char*[nb_exemples]; // tableau contenant les noms des fichiers
    double** inputs = new double*[nb_exemples];
    for(int i(0); i < nb_exemples;i++){
        tabloFichiers[i]=new char[MAX_LENGTH_NAME_FILE]; //initialisation du ta*bleau
        inputs[i]=new double[FIRST_LAYER_SIZE];
    }
    getTabloFichier(tabloFichiers,directory); //on récupère les noms des ficheirs d'exemples
    getTabloExemples(tabloFichiers,inputs,nb_exemples,directory); //on récupère les donnees des exemples
    cout << "Initialisation effectuee en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes, le test commence." <<endl <<endl;

    for (int i = 0; i < nb_exemples; ++i)
    {
        outputs[i]=tester(tablo_net, inputs[i],length);
    }

    return outputs;
}
*/
