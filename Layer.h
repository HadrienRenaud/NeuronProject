#ifndef LAYERJK_H
#define LAYERJK_H

#include <vector>
#include "Neuron.h"
#include "Network.h"

typedef double (*transfert)(double);

class Neuron;//on va utliser la classe neurone
class Network;//network
class Layer{

public:
    Layer();
    Layer(Network* network, int neurons=0,  Layer* previousLayer=0, Layer* nextLayer=0, transfert trsf=0);
    ~Layer();

    int getSize()const;//nombre de neurones dans la couche
    Layer* getNextLayer() const;
    bool setNextLayer(Layer* layer);
    Layer* getPreviousLayer() const;
    bool setPreviousLayer(Layer* layer);
    bool isFirst()const;//s'agit-il de la couche d'entrée ?
    bool isLast()const;//s'agit-il de la couche d sortie ?
    Neuron* getNeuron(int n)const;//on récupère le n-ième neurone
    void addNeuron(transfert trsf=0);//ajouer un neurone dont on spécifie (ou pas) la fonction de transfert
    void addNeurons(int n, transfert trsf=0);//ajouter plsr neurones
    void calculate() const;//demander à chaque neurone d'envoyer la sauce
    void calculateGradient() const;//demander à chaque neurone d'envoyer le gradient
    int getBindingsNumber()const;//renvoi le nombre de liaisons partant de cette couche
    void increaseBindingsNumber();//c'est juste pour compter le nombre de liaison, ça ne sert à rien mais c'est marrant
    Network* getNetwork()const;//réseau parent
    bool learn();//dit a chaque neurone de learn()
    void resetNeurons()const;//préparer les neurones à propager
    void resetNeuronsGradient()const;//préparer les neurones à rétropropager

private:
    Network* m_network;//le réseau auquel la couche appartient
    std::vector<Neuron*> m_neurons;//l'ensemble de ses neurones
    Layer* m_previousLayer;
    Layer* m_nextLayer;
    int m_bindingsNumber;//nombre qui s'incrémente à chaque nouvelle liaison
};

#endif
