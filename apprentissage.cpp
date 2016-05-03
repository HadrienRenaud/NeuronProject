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
const char g_alphabet[LENGTH_ALPHABET] {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

template <class T>
void displayArray(T* data, int length){//afficher un tableau de valeur
    cout << "["; //début de l'affichage
    for(int i = 0; i<length-1; i++) // pour chaque valeur du tableau,
        cout << (data[i]>=0 ? "+" : "") << data[i] << "," ; // on l'affiche avec son signe
    cout << (data[length-1]>=0 ? "+" : "") << data[length-1] << "]"; //fin de l'affichage
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

bool readExemple(char* nom_fichier, double entrees[], int taille_entree){ //surcharge, appelle readExemple
    return readExemple(nom_fichier,entrees,taille_entree,g_dir_exemples); //par défaut on lit les exemples dans g_dir_exemples
}
bool readExemple(char* nom_fichier,double entrees[], int taille_entree,const char* directory){
    ifstream file((string(directory)+string(nom_fichier)).c_str()); //ouverture du fichier
    if (!file){
        //ERREUR -> message dans erreur.txt, affiché dans la console
        ofstream file("erreur.txt");
        file << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        cout << "Apprentissage - Bug 1 : Exemple " << directory << nom_fichier << " non lu." << endl;
        return false;
    }
    else{
        // On lit le fichier
        for (int i(0);i<taille_entree;i++)
            file >> entrees[i]; // on insère les valeurs dans entrees
        return true;
    }
}

int countExemples(){ //surcharge
    return countExemples(g_dir_exemples); //par défaut ...
}
int countExemples(const char* directory){
    int nombre_fichiers(0);     //Initialisation
    DIR* dp;
    struct dirent* ep;
    dp = opendir(directory);    //On ouvre le répertoire contenant les images avant de le parcourir
    if (dp != NULL) //Si l'ouverture du répertoire a fonctionné
    {
        while ((ep = readdir(dp))) //On parcourt le répertoire
            if (strlen(ep->d_name) >= 4) //Si le nom du fichier est d'au moins 4 caractères du genre .png ou .bmp (sinon le programme détecte d'autres fichiers invisibles bizarres)
                nombre_fichiers++;
        closedir (dp);   //Fermeture du répertoire d'images
    }
    return nombre_fichiers;
}

void getArrayOfFileNames(char** tabloFichiers){ //surcharge
    getArrayOfFileNames(tabloFichiers,g_dir_exemples); //par déa
}
void getArrayOfFileNames(char** tabloFichiers,const char* directory){
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

void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples){ //Surcharge
    getArrayOfExemples(tabloFichiers, tabloExemple, nb_exemples , g_dir_exemples); // Par défaut, directory = g_dir_exemples
}
void getArrayOfExemples(char** tabloFichiers, double** tabloExemple, int nb_exemples,const char* directory){
    cout << " Lecture des exemples ... " << flush;
    for (int i(0); i< nb_exemples;i++)
        readExemple(tabloFichiers[i],tabloExemple[i],FIRST_LAYER_SIZE,directory); //on lit chacun des exemples
    cout << "Lecture terminée." <<endl;
}

void getMostRecent(Network* net){ //surcharge
    getMostRecent(net, '_'); //default
}
void getMostRecent(Network* net , char lettre_testee){
    //initialisations
    string nom_fichier;
    string nom_db;

    //on reconstitue le nom du fichier :
    nom_db= string(g_dir_svg) + g_nom_svg+lettre_testee;
    nom_db=nom_db+".txt";

    ifstream file(nom_db.c_str()); //on ouvre le fichier getMostRecent.txt
    file >> nom_fichier; //on lit son contenu
    cout << "Reseau " << lettre_testee << " - recuperation du fichier : " << nom_fichier << endl;
    net->recup(nom_fichier); // On récupère le réseau stocké dans le fichier de svg le plus récent
}

void writeReport(Network* net, bool resultat, int count, double distance_moyenne, double temps_mis, string commentaires,char lettre_testee,char* nom_fichier){
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
    base_donnes << buffer << ","<< lettre_testee << "," << net->getTotalLayerNumber()+1 << ',';

    //inscription des couches
    temp = net->getFirstLayer();
    do
    {
        base_donnes << temp->getSize() << ',' ;
        temp = temp->getNextLayer();
    } while (temp!=0);
    for (int i(0);i<(4-(net->getTotalLayerNumber()));i++)
        base_donnes << ',';

    //fin de l'inscription des données
    base_donnes << count << ',' << distance_moyenne << ',' << DISTANCE_MAXIMALE  << ',';
    cout << resultat << ',' << temps_mis <<','<< commentaires << ',' << nom_fichier << endl;
}

void learnNetworks(Network* net,char lettre_testee, const int nb_exemples, char** tabloFichiers, double** inputs){
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
            att_output[0]=1
        else
            att_output[0] =0;

        //Calcul de la réponse du réseau
        net->initNetwork(inputs[exemple]); //on initialise avec les valeurs inputs
    	net->launch(exp_output);//on lance et on récupère les outputs

        //Calcul de l'écart
        if (count < nb_exemples * NB_APPRENTISSAGE * BORNE_MAX_CASSE && false)
            dist = distance(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart
        else
            dist = distanceMod(exp_output, att_output , LAST_LAYER_SIZE );//on calcule l'écart

        //On apprend, ou pas en fonction du résultat
        if(dist<DISTANCE_MAXIMALE){//si c'est assez petit, c'est un succès
            successes++;
        }else{//sinon c'est un echec et le réseau recalcule les poids des liaisons
            net->initNetworkGradient(att_output);
            net->learn();
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
    net->save(lettre_testee,nom_fichier);

    //Affichages ...
    if (count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples)
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
        net->initNetwork(inputs[exemple]);
        net->launch(exp_output);

        //Calcul de la distance
        distance_totale+=distance(exp_output, att_output , LAST_LAYER_SIZE );
    }
    cout << "distance moyenne sur les exemples : " << distance_totale / nb_exemples << endl ;

    //Calcul du temps mis
    double temps_mis(((float)(clock()-t0)/CLOCKS_PER_SEC));
    cout << "Apprentissage effectué en " << temps_mis << " secondes" << endl;

    // On met à jour les données
    writeReport(net,(count < MAX_LIMIT_LOOP*NB_APPRENTISSAGE*nb_exemples),count/nb_exemples,distance_totale/nb_exemples,temps_mis," ",lettre_testee,nom_fichier);

}

Network** createNetworks(){
    Network** tablo_net = new Network*[LENGTH_ALPHABET]; //Création du tableau
    cout << "Creation des réseaux ... " << flush ;
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        tablo_net[i] = new Network;//Le réseau
        //Layer* l = new Layer(adresse du réseau, nombre de neurones dans la couche, adresse de la couche précédente, adresse de la couche suivante, fonction de transfert des neuronres de la couche);
        Layer* l1= new Layer(tablo_net[i], FIRST_LAYER_SIZE,  0,  0); //première couche
        Layer* l2= new Layer(tablo_net[i], 100, l1, 0); //seconde
        Layer* l3= new Layer(tablo_net[i], 10, l2, 0); //troisieme
        Layer* lend= new Layer(tablo_net[i],LAST_LAYER_SIZE,l3, 0); //couche de fin
    }
    cout << "Reseaux créés !" << endl;
    return tablo_net;
}

void destroyNetworks(Network** tablo_net){
    cout << "Sauvegarde et destruction des réseaux ..." << flush;
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        tablo_net[i]->save(g_alphabet[i]);
        delete tablo_net[i];
    }
    delete tablo_net;
    cout << "Réseaux détruits !" << endl;
}

void getAndLearn(){
    cout << "récupération des réseaux ... " << flush;

    Network** tablo_net(createNetworks());//initialisation

    for (int i = 0; i < LENGTH_ALPHABET; ++i)
        getMostRecent(tablo_net[i],g_alphabet[i]); // on récupère le plus récent

    cout << "réseaux récupérés." << endl;
    learnAllNetworks(tablo_net);
}

Network** createNewAndLearn(){
    cout << "Création du réseau ... " <<flush;

    Network** tablo_net(createNetworks()); //on crée le réseau

    cout << "le réseau est créé."<<endl;
    learnAllNetworks(tablo_net); // et on apprend
    return tablo_net;
}

void learnAllNetworks(Network** tablo_net){
    cout << "Bienvenue dans le gestionnaire d'apprentissage du réseau de neurones" << endl << endl;

    cout << "Initialisation des paramètres." << endl;
    clock_t t0(clock());//temps de départ du programme

    //nombre d'exemples à traiter
    int const nb_exemples(countExemples());

    // Initialisation des tableaux contenant les donnees des exemples
    char** tabloFichiers = new char*[nb_exemples];
    double** inputs = new double*[nb_exemples];
    for (int i = 0; i < nb_exemples; ++i)
    {
        tabloFichiers[i] = new char[MAX_LENGTH_NAME_FILE];
        inputs[i] = new double[FIRST_LAYER_SIZE];
    }

    //Récupération des données des fichiers
    getArrayOfFileNames(tabloFichiers); //on récupère les noms des ficheirs d'exemples
    getArrayOfExemples(tabloFichiers,inputs,nb_exemples); //on récupère les donnees des exemples
    cout << "Initialisation effectuée en " << ((float)(clock()-t0)/CLOCKS_PER_SEC) << " secondes, l'apprentissage commence." <<endl <<endl;

    //Apprentissage
    for (int i = 0; i < LENGTH_ALPHABET; ++i)
    {
        learnNetworks(tablo_net[i],g_alphabet[i],nb_exemples,tabloFichiers,inputs);
        cout << endl << endl;
    }
}

char testNetworks(Network** tablo_net, double input[]){
    //initialisation
    char lettre_trouvee('_');
    double maxi(BORNE_INF_DISTINCTION);
    double exp_output[LENGTH_ALPHABET][LAST_LAYER_SIZE];

    //tests
    cout << "Results : ";
    for (int i = 0; i < LENGTH_ALPHABET ; ++i) //pour chaque réseau
    {
        //on récupère la réponse
        tablo_net[i]->initNetwork(input);
        tablo_net[i]->launch(exp_output[i]);

 //on l'affiche si elle est suffisement grande
        if (exp_output[i][0]>(BORNE_INF_DISTINCTION/10))
            cout << g_alphabet[i] << " : " << exp_output[i][0]*100 << "% -- " ;

        //on trouve le maximum des réponses aux premiers réseaux
        if (maxi<exp_output[i][0])
        { //On modifie maxi si elle est supérieure
            maxi=exp_output[i][0];
            lettre_trouvee=g_alphabet[i];
        }
    }
    cout << endl;
    return lettre_trouvee;
}
