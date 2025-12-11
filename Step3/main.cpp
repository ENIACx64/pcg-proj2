/**
 * @file      main.cpp
 *
 * @author    Jindrich Vodak \n
 *            Faculty of Information Technology \n
 *            Brno University of Technology \n
 *            xvodak06@stud.fit.vutbr.cz
 *
 * @brief     PCG - Assignment 2 / Step3
 *
 * @version   2025
 *
 * @date      25 November   2025, 00:00 (created) \n
 */


#include <chrono>
#include <cmath>
#include <csetjmp>
#include <cstdio>
#include <string>

#include "nbody.h"
#include "h5Helper.h"
#include "Vec.h"

// adding global variables for stream IDs
#define STREAM_NEW_POS 1
#define STREAM_COPY_DATA 2
#define STREAM_CALC_COM 3

/**
 * Main routine
 */
int main(int argc, char **argv)
{
  if (argc != 7)
  {
    std::printf("Usage: %s <N> <dt> <steps> <write intensity> <input> <output>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Number of particles
  const unsigned N = static_cast<unsigned>(std::stoul(argv[1]));
  // Length of time step
  const float dt = std::stof(argv[2]);
  // Number of steps
  const unsigned steps = static_cast<unsigned>(std::stoul(argv[3]));
  // Write frequency
  const unsigned writeFreq = static_cast<unsigned>(std::stoul(argv[4]));

  // Log benchmark setup
  std::printf("       NBODY STEP3 simulation\n"
              "N:                       %u\n"
              "dt:                      %f\n"
              "steps:                   %u\n",
              N, dt, steps);

  const std::size_t recordsCount = (writeFreq > 0) ? (steps + writeFreq - 1) / writeFreq : 0;

  Particles particles[2]{Particles{N}, Particles{N}};

  /********************************************************************************************************************/
  /*                                     TODO: Fill memory descriptor parameters                                      */
  /********************************************************************************************************************/

  /*
   * Caution! Create only after CPU side allocation
   * parameters:
   *                        Stride of two           Offset of the first
   *         Data pointer   consecutive elements    element in FLOATS
   *                        in FLOATS (not bytes)   (not bytes)
   */
  MemDesc md(&particles[0].positions[0].x, 4, 0,
             &particles[0].positions[0].y, 4, 0,
             &particles[0].positions[0].z, 4, 0,
             &particles[0].velocities[0].x, 3, 0,
             &particles[0].velocities[0].y, 3, 0,
             &particles[0].velocities[0].z, 3, 0,
             &particles[0].positions[0].w, 4, 0,
             N,
             recordsCount);

  /********************************************************************************************************************/

  // Initialisation of helper class and loading of input data
  H5Helper h5Helper(argv[5], argv[6], md);

  try
  {
    h5Helper.init();
    h5Helper.readParticleData();
  }
  catch (const std::exception& e)
  {
    std::fprintf(stderr, "Error: %s\n", e.what());
    return EXIT_FAILURE;
  }

  /********************************************************************************************************************/
  /*                   TODO: Allocate memory for center of mass buffer. Remember to clear it.                         */
  /********************************************************************************************************************/
  // rounding to the closest power of two (for division)
  unsigned maxN = 1;
  while (maxN < N) maxN <<= 1;

  float4* comBuffer = new float4[maxN]();
  #pragma acc enter data create(comBuffer[0:maxN])

  /********************************************************************************************************************/

  /********************************************************************************************************************/
  /*                                     TODO: Memory transfer CPU -> GPU                                             */
  /********************************************************************************************************************/
  particles[0].copyToDevice();
  particles[1].copyToDevice();
  #pragma acc wait (STREAM_COPY_DATA)

  /********************************************************************************************************************/

  // Lambda for checking if we should write current step to the file
  auto shouldWrite = [writeFreq](unsigned s) -> bool
  {
    return writeFreq > 0u && (s % writeFreq == 0u);
  };

  // Lamda for getting record number
  auto getRecordNum = [writeFreq](unsigned s) -> unsigned
  {
    return s / writeFreq;
  };
  
  // Start measurement
  const auto start = std::chrono::steady_clock::now();

  /********************************************************************************************************************/
  /*            TODO: Edit the loop to work asynchronously and overlap computation with data transfers.               */
  /*                  Use shouldWrite lambda to determine if data should be outputted to file.                        */
  /*                           if (shouldWrite(s, writeFreq)) { ... }                                                 */
  /*                        Use getRecordNum lambda to get the record number.                                         */
  /********************************************************************************************************************/
  for (unsigned s = 0u; s < steps; ++s)
  {
    const unsigned srcIdx = s % 2;        // source particles index
    const unsigned dstIdx = (s + 1) % 2;  // destination particles index

    /******************************************************************************************************************/
    /*                                        TODO: GPU computation                                                   */
    /******************************************************************************************************************/
    calculateVelocity(particles[srcIdx], particles[dstIdx], N, dt);

    if (shouldWrite(s))
    {
      #pragma acc wait(STREAM_NEW_POS)
      centerOfMass(particles[dstIdx], comBuffer, N);

      #pragma acc wait(STREAM_CALC_COM)
      particles[srcIdx].copyToHost();

      #pragma acc update host(comBuffer[0:1]) async(STREAM_COPY_DATA)

      #pragma acc wait(STREAM_COPY_DATA)

      h5Helper.writeParticleData(getRecordNum(s));
      h5Helper.writeCom(comBuffer[0], getRecordNum(s));
    }

    /******************************************************************************************************************/
  }

  const unsigned resIdx = steps % 2;    // result particles index

  /********************************************************************************************************************/
  /*            TODO: Invocation of center of mass kernel, do not forget to add additional synchronization            */
  /********************************************************************************************************************/
  #pragma acc wait(STREAM_NEW_POS, STREAM_COPY_DATA, STREAM_CALC_COM)

  centerOfMass(particles[resIdx], comBuffer, N);

  /********************************************************************************************************************/

  // End measurement
  const auto end = std::chrono::steady_clock::now();

  // Approximate simulation wall time
  const float elapsedTime = std::chrono::duration<float>(end - start).count();
  std::printf("Time: %f s\n", elapsedTime);

  /********************************************************************************************************************/
  /*                                     TODO: Memory transfer GPU -> CPU                                             */
  /********************************************************************************************************************/
  particles[resIdx].copyToHost();
  #pragma acc wait (STREAM_COPY_DATA)

  /********************************************************************************************************************/

  // Compute reference center of mass on CPU
  const float4 refCenterOfMass = centerOfMassRef(md);

  std::printf("Reference center of mass: %f, %f, %f, %f\n",
              refCenterOfMass.x,
              refCenterOfMass.y,
              refCenterOfMass.z,
              refCenterOfMass.w);

  if (comBuffer != nullptr)
  {   std::printf("Center of mass on GPU: %f, %f, %f, %f\n",
                comBuffer->x,
                comBuffer->y,
                comBuffer->z,
                comBuffer->w);

    // Writing final COM to the file
    h5Helper.writeComFinal(*comBuffer);
  }
  
  // Writing final values to the file
  h5Helper.writeParticleDataFinal();

  /********************************************************************************************************************/
  /*                                TODO: Free center of mass buffer memory                                           */
  /********************************************************************************************************************/
  #pragma acc exit data delete(comBuffer[0:maxN])
  delete[] comBuffer;

  /********************************************************************************************************************/

  return EXIT_SUCCESS;
}// end of main
//----------------------------------------------------------------------------------------------------------------------
