/**
 * @file      nbody.h
 *
 * @author    Jindrich Vodak \n
 *            Faculty of Information Technology \n
 *            Brno University of Technology \n
 *            xvodak06@stud.fit.vutbr.cz
 *
 * @brief     PCG - Assignment 2 / Step0
 *
 * @version   2025
 *
 * @date      25 November   2025, 00:00 (created) \n
 */

#include <cfloat>
#include <cmath>

#include "nbody.h"
#include "Vec.h"

/* Constants */
constexpr float G                  = 6.67384e-11f;
constexpr float COLLISION_DISTANCE = 0.01f;

/**********************************************************************************************************************/
/*                TODO: Fullfill Partile's and Velocitie's constructors, destructors and methods                      */
/*                                    for data copies between host and device                                         */
/**********************************************************************************************************************/

/**
 * @brief Constructor
 * @param N - Number of particles
 */
Particles::Particles(const unsigned N) : N(N)
{
  positions = new float4[N]();
  velocities = new float3[N]();

  #pragma acc enter data copyin(this[0:1])
  #pragma acc enter data create(positions[0:N], velocities[0:N])
}

/// @brief Destructor
Particles::~Particles()
{
  #pragma acc exit data delete(positions[0:N], velocities[0:N])
  #pragma acc exit data delete(this[0:1])

  delete[] positions;
  delete[] velocities;
}

/**
 * @brief Copy particles from host to device.
 */
void Particles::copyToDevice()
{
  #pragma acc update device(positions[0:N], velocities[0:N])
}

/**
 * @brief Copy particles from device to host
 */
void Particles::copyToHost()
{
  #pragma acc update host(positions[0:N], velocities[0:N])
}

/**
 * @brief Constructor
 * @param N - Number of particles
 */
Velocities::Velocities(const unsigned N)
{
  velocities = new float3[N]();

  #pragma acc enter data copyin(this[0:1])
  #pragma acc enter data create(velocities[0:N])
}

/// @brief Destructor
Velocities::~Velocities()
{
  #pragma acc exit data delete(velocities[0:N])
  #pragma acc exit data delete(this[0:1])

  delete[] velocities;
}

/**
 * @brief Copy velocities from host to device.
 */
void Velocities::copyToDevice()
{
  #pragma acc update device(velocities[0:N])
}

/**
 * @brief Copy velocities from device to host.
 */
void Velocities::copyToHost()
{
  #pragma acc update host(velocities[0:N])
}

/**********************************************************************************************************************/

/**
 * Calculate gravitation velocity
 * @param p      - particles
 * @param tmpVel - temp array for velocities
 * @param N      - Number of particles
 * @param dt     - Size of the time step
 */
void calculateGravitationVelocity(Particles& p, Velocities& tmpVel, const unsigned N, float dt)
{
  /********************************************************************************************************************/
  /*                    TODO: Calculate gravitation velocity, see reference CPU version,                              */
  /*                            you can use overloaded operators defined in Vec.h                                     */
  /********************************************************************************************************************/

  

  /********************************************************************************************************************/
}// end of calculateGravitationVelocity
//----------------------------------------------------------------------------------------------------------------------

/**
 * Calculate collision velocity
 * @param p      - particles
 * @param tmpVel - temp array for velocities
 * @param N      - Number of particles
 * @param dt     - Size of the time step
 */
void calculateCollisionVelocity(Particles& p, Velocities& tmpVel, const unsigned N, float dt)
{
  /********************************************************************************************************************/
  /*                    TODO: Calculate collision velocity, see reference CPU version,                                */
  /*                            you can use overloaded operators defined in Vec.h                                     */
  /********************************************************************************************************************/



  /********************************************************************************************************************/
}// end of calculateCollisionVelocity
//----------------------------------------------------------------------------------------------------------------------

/**
 * Update particles
 * @param p      - particles
 * @param tmpVel - temp array for velocities
 * @param N      - Number of particles
 * @param dt     - Size of the time step
 */
void updateParticles(Particles& p, Velocities& tmpVel, const unsigned N, float dt)
{
  /********************************************************************************************************************/
  /*                    TODO: Update particles position and velocity, see reference CPU version,                      */
  /*                            you can use overloaded operators defined in Vec.h                                     */
  /********************************************************************************************************************/



  /********************************************************************************************************************/
}// end of updateParticles
//----------------------------------------------------------------------------------------------------------------------

/**
 * CPU implementation of the Center of Mass calculation
 */
float4 centerOfMassRef(MemDesc& memDesc)
{
  float4 com{};

  for (std::size_t i{}; i < memDesc.getDataSize(); i++)
  {
    const float3 pos = {memDesc.getPosX(i), memDesc.getPosY(i), memDesc.getPosZ(i)};
    const float  w   = memDesc.getWeight(i);

    // Calculate the vector on the line connecting current body and most recent position of center-of-mass
    // Calculate weight ratio only if at least one particle isn't massless
    const float4 d = {pos.x - com.x,
                      pos.y - com.y,
                      pos.z - com.z,
                      ((memDesc.getWeight(i) + com.w) > 0.0f)
                        ? ( memDesc.getWeight(i) / (memDesc.getWeight(i) + com.w))
                        : 0.0f};

    // Update position and weight of the center-of-mass according to the weight ration and vector
    com.x += d.x * d.w;
    com.y += d.y * d.w;
    com.z += d.z * d.w;
    com.w += w;
  }

  return com;
}// enf of centerOfMassRef
//----------------------------------------------------------------------------------------------------------------------
