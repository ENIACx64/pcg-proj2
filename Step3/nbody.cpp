/**
 * @file      nbody.cpp
 *
 * @author    Name Surname \n
 *            Faculty of Information Technology \n
 *            Brno University of Technology \n
 *            xlogin00@fit.vutbr.cz
 *
 * @brief     PCG - Assignment 2 / Step3
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
Particles::Particles(const unsigned N)
{



}

/// @brief Destructor
Particles::~Particles()
{



}

/**
 * @brief Copy particles from host to device.
 */
void Particles::copyToDevice()
{



}

/**
 * @brief Copy particles from device to host
 */
void Particles::copyToHost()
{



}

/**********************************************************************************************************************/

/**
 * Calculate velocity
 * @param pIn  - particles input
 * @param pOut - particles output
 * @param N    - Number of particles
 * @param dt   - Size of the time step
 */
void calculateVelocity(Particles& pIn, Particles& pOut, const unsigned N, float dt)
{
  /********************************************************************************************************************/
  /*             TODO: Calculate gravitation velocity and position, see reference CPU version,                        */
  /*                            you can use overloaded operators defined in Vec.h                                     */
  /********************************************************************************************************************/



  /********************************************************************************************************************/
}// end of calculateVelocity
//----------------------------------------------------------------------------------------------------------------------

/**
 * Calculate particles center of mass
 * @param p       - particles
 * @param comBuff - pointer to a center of mass buffer
 * @param N       - Number of particles
 */
void centerOfMass(Particles& p, float4* comBuff, const unsigned N)
{
  /********************************************************************************************************************/
  /*                                    TODO: Calculate particles center of mass                                      */
  /********************************************************************************************************************/



  /********************************************************************************************************************/
}// end of centerOfMass
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
