/**
 * @file      nbody.cpp
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
  #pragma acc parallel loop gang vector present(pIn, pOut)
  for (int i = 0; i < N; i++)
  {
    const float4 posi = pIn.positions[i];
    const float3 veli = pIn.velocities[i];
    const float wi = posi.w;

    float newVelX = 0.0f;
    float newVelY = 0.0f;
    float newVelZ = 0.0f;
    float collisionVelX = 0.0f;
    float collisionVelY = 0.0f;
    float collisionVelZ = 0.0f;

    #pragma acc loop seq
    for (int j = 0; j < N; j++)
    {
      // checking if particle interacts with itself
      if (i == j) continue;

      const float4 posj = pIn.positions[j];
      const float wj = posj.w;
      const float3 velj = pIn.velocities[j];

      // calculating position differences
      float dx = posj.x - posi.x;
      float dy = posj.y - posi.y;
      float dz = posj.z - posi.z;

       // calculating r as shown in the CPU version
      float r2 = dx * dx + dy * dy + dz * dz;
      float r  = std::sqrt(r2);

      // calculating gravitation velocity
      if (r > COLLISION_DISTANCE)
      {
        const float f = G * wi * wj / r2;
        const float ir = 1.0f / r;

        newVelX += f * dx * ir;
        newVelY += f * dy * ir;
        newVelZ += f * dz * ir;
      }

      // calculating collision velocity
      if (r < COLLISION_DISTANCE)
      {
        const float im = 1.0f / (wi + wj);

        const float vx = (wi * veli.x - wj * veli.x + 2.0f * wj * velj.x) * im;
        const float vy = (wi * veli.y - wj * veli.y + 2.0f * wj * velj.y) * im;
        const float vz = (wi * veli.z - wj * veli.z + 2.0f * wj * velj.z) * im;

        collisionVelX = vx - veli.x;
        collisionVelY = vy - veli.y;
        collisionVelZ = vz - veli.z;
      }
    }

    float4 posNew;
    float3 velNew;

    velNew.x = veli.x + newVelX * (dt / wi) + collisionVelX;
    velNew.y = veli.y + newVelY * (dt / wi) + collisionVelY;
    velNew.z = veli.z + newVelZ * (dt / wi) + collisionVelZ;

    posNew.x = posi.x + velNew.x * dt;
    posNew.y = posi.y + velNew.y * dt;
    posNew.z = posi.z + velNew.z * dt;
    posNew.w = wi;

    pOut.positions[i] = posNew;
    pOut.velocities[i] = velNew;
  }

  /********************************************************************************************************************/
}// end of calculateVelocity
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
