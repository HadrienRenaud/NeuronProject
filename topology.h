#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include "config.h"

//! Determine the number of connected component
/*! In the image passed as component
    \param matrix : image
    \return nombre int number of connected component
 */
int nbConnectedComponent(double matrix[], int n = TOPOLOGY_INPUT_SIZE);

#endif /* end of include guard: TOPOLOGY_H_INCLUDED */
