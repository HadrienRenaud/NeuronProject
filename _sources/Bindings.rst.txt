.. NeuronProject documentation file. Created by Hadrien Renaud-Lebret on wed Feb 14. 2017.

Bindinds module
=================

.. default-domain:: cpp

.. class:: Bindings

  Binding class, composed of the source neuron and a weight.

  :note: A :class:`Neuron` has the same number of :class:`Bindings` than of predecessor :class:`Neuron`.

  .. function:: Binding()
                Binding(Neuron* neuron, double weight = 1 )

      :param neuron: Source Neuron
      :param weight: Weight of the binding

  .. function:: ~Binding()

    Destructor

  .. function:: Neuron* getNeuron() const

    Returns the source neuron

  .. function:: void setWeight(double weight = 1)

    Set the weight of the binding.

  .. function:: double getWeight() const

    Returns the weight of the Binding.

  .. function:: void addWeight(double weight)

    Shortcut for `setWeight(weight + getWeight)`

  .. var:: Neuron* neuron_

    Source neuron.

  .. var:: double weight_

    Weight of the binding.

  .. var:: double previousStep_

    Weight a the previous step of the learning algorithm.
