/**
 * @file      nbody.h
 *
 * @author    Jindrich Vodak \n
 *            Faculty of Information Technology \n
 *            Brno University of Technology \n
 *            xvodak06@stud.fit.vutbr.cz
 *
 * @brief     PCG - Assignment 2 / Step1
 *
 * @version   2025
 *
 * @date      25 November   2025, 00:00 (created) \n
 */

#ifndef NBODY_H
#define NBODY_H

#include "h5Helper.h"
#include "Vec.h"

/**
 * @brief Particles data structure
 */
struct Particles
{
  /// @brief Default constructor not allowed
  Particles() = delete;

  /**
   * @brief Constructor
   * @param N - Number of particles
   */
  Particles(const unsigned N);

  /// @brief Copy constructor not allowed
  Particles(const Particles&) = delete;

  /// @brief Move constructor not allowed
  Particles(Particles&&) = delete;

  /// @brief Destructor
  ~Particles();

  /// @brief Copy assignment operator not allowed
  Particles& operator=(const Particles&) = delete;

  /// @brief Move assignment operator not allowed
  Particles& operator=(Particles&&) = delete;

  /**
   * @brief Copy particles from host to device
   */
  void copyToDevice();

  /**
   * @brief Copy particles from device to host
   */
  void copyToHost();

  /********************************************************************************************************************/
  /* TODO: Particles data structure optimized for use on GPU. Use float3 and float4 structures defined in file Vec.h  */
  /********************************************************************************************************************/
  unsigned N;
  float4* positions;
  float3* velocities;

  /********************************************************************************************************************/
};

/**
 * Calculate velocity
 * @param pIn  - particles input
 * @param pOut - particles output
 * @param N    - Number of particles
 * @param dt   - Size of the time step
 */
void calculateVelocity(Particles&     pIn,
                       Particles&     pOut,
                       const unsigned N,
                       float          dt);

/**
 * CPU implementation of the Center of Mass calculation
 * @param memDesc - Memory descriptor of particle data on CPU side
 */
float4 centerOfMassRef(MemDesc& memDesc);

#endif /* NBODY_H */
