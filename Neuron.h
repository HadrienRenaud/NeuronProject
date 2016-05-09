#ifndef NEURONJK_H
#define NEURONJK_H

#define THRESHOLD 0

#include <vector>
#include <cmath>
#include "Layer.h"
#include "Binding.h"

#define PENTE 1//demi-pente de la sigmoide à l'origine
#define MU 0.9//taux d'apprentissage 0<MU<1

typedef double (*transfert)(double);//on défini un format de pointeur de fonction, on l'invoquera avec le mot transfert
                                    //ce format est destiné à contenir les fonctions de transfert des neurones

class Binding;//on indique que l'on va se servir de la class Binding
class Layer;//idem
class Neuron{

public :
    //constructeur par défaut, contructeur de copie et constructeur que l'on utilise vraiment dans le programme
    Neuron();
    Neuron(const Neuron& neuron);//pour le constructeur de copie, on associe à la copie : la mm fonction de transfert, la mm couche et les memes liaisons vers les meme neurones,
    Neuron(Layer* layer, transfert trsf=0);//si aucune fonction de transfert n'est donné en paramètre, on prend une sigmoide
    ~Neuron();//destructeur
    //METHODES
    void setTransfert(double (*)(double));//changer la fonction de transfert
    transfert getTransfert() const; //fonction de transfert
    Layer* getLayer()const;
    int getBindingsNumber()const;//donne le nombre de liaisons avec les neurones des couches PRECEDENTES
    void receive();//le neurone récupère la somme pondérée des outputs des neurones des couches d'avant auxquels il est connecté
    void sendGradient();//le neurone envoie son propre gradient aux neurones précedents, pondérée par le poids de chaque liaison
    void addBinding(Binding* binding);//nouvelle liaison
    void addBinding(Neuron* neuron, double weight=1);//invoque la méthode précédente
    double getInput()const;
    double getOutput()const;
    double getGradient()const;
    bool initNeuron(double input);//remet à 0 l'input. Si le neuron est dans la première couche du réseau, input est pris en compte.
    bool initNeuronGradient(double expectedOutput);//mm chose qu'au dessus, Si le réseau est dans la dernière couche, assignation de expectedOutput est autorisée
    Binding* getBinding(int n);//récupérer la liaison d'indice n
    int getIndexInLayer()const;//connaître la position du neuron dans sa couche
    void learn();//algo d'apprentissage. cette méthode est invoqué par learn() de la class couche, qui est elle-mêe invpquée par learn() de la class réseau. cette dernière est appelée dans le main


private :
    double m_input;//somme pondérée des entrées
    double m_output;//transfert(m_input)
    transfert m_trsf;//fonction de transfert du neuron
    Layer* m_layer;//couche à laquelle il appartient
    std::vector<Binding*> m_bindings;//l'ensemble des liaisons (neurones sources, et non successeurs)
    int m_indexInLayer;//position du neuron dans sa couche
    double m_gradient;//gradient du neurone

};

//2 couples (fonction,dérivée)
double threshold(double input);
double threshold1(double input);

double sigmo(double input);
double sigmo1(double input);

#endif
