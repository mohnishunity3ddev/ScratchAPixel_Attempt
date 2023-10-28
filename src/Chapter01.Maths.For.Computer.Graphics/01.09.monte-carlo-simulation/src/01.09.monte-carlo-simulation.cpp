#if 1
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <jmath.h>
#include <file_operations.h>

// #define ONED
#define VISUALIZE_TRANSMISSION 0
#define VISUALIZE_REFLECTANCE 0
#define VISUALIZE_ABSORPTION 1

// NOTE: 
// -> In this monte carlo simulation, we are simulating what light photons go
// through when interacting with a surface slab of thickness d.
// -> Total Internal Reflection is not simulated here.
// -> Light photons go through Scattering, Reflectance from the top surface when it
// begins its interaction with the slab and Absorption where the photons are
// absorbed by the atoms of the slab material.

// -> We use inverse transform sampling to sample the pdf of Beer-Lambert law.
// which basically tells the probability of a photon interacting with the atoms
// of the material given the distance it has travelled within the surface.

//

#define M_PI 3.141592653589793

inline float
Rand01()
{
    float Result = (float)rand() / (float)RAND_MAX;
    return Result;
}

// NOTE:
// This function uses the inverse sampling method to get the Cosine of the angle
// which the photon gets scattered by given the G(Anisotropy) Paramter.
// The PDF that is sampled using the inverse sampling method is called the
// Henyey-Greenstein scattering phase function which basically gives the
// probability that the photon gets scattered by an Angle Theta. Since we are
// using the inverse sampling method, we compute the inverse CDF of this PDF, so
// we input a uniform distribution 0-1 value representing the probability as the
// input and get back the Cosine of the angle the photon will be deflected by.
double
GetCosTheta(const double &G)
{
    if(G == 0) return 2*Rand01() - 1;
    double Mu = (1 - G*G) / (1 - G + 2*G*Rand01());
    double Result = (1 + G*G - Mu*Mu) / (2.0 * G);
    return Result;
}

// NOTE:
// This is the function where the photon packet gets scattered by the material of the slab we are using.
// Scattering happens since we modify the X, Y, Z velocities of the photon packet here in this function.
void
Scatter(double &PhotonVelX, double &PhotonVelY, double &PhotonVelZ,
        const double &G_HenyeyGreenstein)
{
    double CosTheta = GetCosTheta(G_HenyeyGreenstein);
    double SinTheta = sqrt(1.0 - CosTheta*CosTheta);
    
    double Phi = 2*M_PI*Rand01();
    double SinPhi = sin(Phi);
    double CosPhi = cos(Phi);
    
    // NOTE: If photon packet is moving towards the bottom of the slab and hasnt
    // been scattered yet.
    if(PhotonVelZ == 1.0)
    {
        // Check Polar Coordinates.
        PhotonVelX = SinTheta * CosPhi;
        PhotonVelY = SinTheta * SinPhi;
        PhotonVelZ = CosTheta;
    }
    // NOTE: If photon packet is moving towards the top of the slab and hasnt
    // been scattered yet.
    else if(PhotonVelZ == -1.0)
    {
        // Check Polar Coordinates.
        PhotonVelX = SinTheta * CosPhi;
        PhotonVelY = -SinTheta * SinPhi;
        PhotonVelZ = -CosTheta;
    }
    else
    {
        // NOTE: Here, we are rotating the incident direction vector along any
        
        // vector that is perpendicular to it. We are using the rodriguez
        // formula to rotate this incident photon direction along this
        // orthoginal axis.
        
        double Denominator = sqrt(1.0 - PhotonVelZ*PhotonVelZ);
        double PhotonVelZCosPhi = PhotonVelZ * CosPhi;
        
        double VelX = (PhotonVelX*CosTheta) + (SinTheta*((PhotonVelX*PhotonVelZCosPhi) - (PhotonVelY*SinPhi)) / Denominator);
        double VelY = (PhotonVelY*CosTheta) + (SinTheta*((PhotonVelY*PhotonVelZCosPhi) - (PhotonVelX*SinPhi)) / Denominator);
        double VelZ = -Denominator*SinTheta*CosPhi + PhotonVelZ*CosTheta;
        
        PhotonVelX = VelX;
        PhotonVelY = VelY;
        PhotonVelZ = VelZ;
    }
}

void
MonteCarloLightTransportSimulation(double *&Records, const uint32_t &Size)
{
    // Number of photons for the light.
    int NumPhotons = 100'000;
    double Scale = 1.0 / NumPhotons;    
    // NOTE: One way of thinking about these values is this:
    // Sigma_S / Sigma_T gives the prbability of the photon scattering.
    // Sigma_A / Sigma_T gives the prbability of the photon getting absorbed.
    
    // Absorption coefficient
    const double Sigma_A = 1.0;
    // Scattering coefficient
    const double Sigma_S = 2.0;
    // Extinction coefficient
    const double Sigma_T = Sigma_A + Sigma_S;
    const double SlabThicknessZ = 0.5;
    const double SlabSizeXY     = 0.5;
    
    // NOTE:
    // This 'g' parameter is used in the H-G scattering phase function and defines anisotropy.
    // -> if g = 0, isotropic scattering where the scattering happens equally in all directions.
    // -> if g < 0, scattering direction points back to the light source - backward scattering
    // -> if g > 0, forward scattering in a cone, 
    const double G_HenyeyGreenstein = 0.75;
    
    // NOTE:
    // This is used for Russian Roullette technique for MC Methods.
    // Its a technique for combining a bunch of photon inside a single packet.
    // The fate of the packet - whether it is absorbed or not is decided
    // together for the packet of say 100 photons. More explanation below where
    // we actually do the rusisan roulette test.
    static const short M_RussianRoulette = 10;
    
    // Num of photons leaving the slab through the top surface.
    int DiffuseReflectance = 0;
    // Num of photons leaving the slab through the bottom surface.
    int Transmission = 0;
    // Num of photons getting absorbed by the material of the surface.
    int Absorption = 0;
    
    for(int i = 0; 
        i < NumPhotons; 
        ++i)
    {
        // The Weight given to the photon packet.
        double PhotonPacketUnabsorbedWeight = 1.0;
        
        // The postion of the photon in our simulation'
        double PhotonPX = 0, PhotonPY = 0, PhotonPZ = 0;
        // The current direction of the velocity of the photon.
        // NOTE: In the beginning, the photon packet is moving in the positive Z
        // Direction ie towards the bottom of the slab.
        double PhotonVelX = 0, PhotonVelY = 0, PhotonVelZ = 1;
        
        while(PhotonPacketUnabsorbedWeight != 0)
        {
            // NOTE:
            // -> RandomPath is basically the distance the photon travels before
            // interacting with the slab material's atoms.
            // -> Rand01 is value between 0 and 1 from a uniform distribution.
            // -> Beer Lambert's Law gives us probability of a photon getting either
            // absorbed or scattered by the atoms of the material the photon is
            // travelling through the material. the pdf of this is:
            // Sigma*e^(-Sigma*x) where x is the distance travelled by the photon
            // inside a material. Sigma is the extinction coefficient.
            // -> As we saw in inverse sampling method, we get a CDF of this
            // function and then invert it and then feed it a value which represents
            // the probability that a photon DOES interact with the material's
            // atom(uniform distributed 0-1 value) and then get back the X(Distance
            // travelled by the photon) from this. The inverted CDF of this beer
            // lambert pdf is LogBaseE(Pr(Distance < X)) / ExtinctionCoeff.
            double S = -log(Rand01()) / Sigma_T;
            
            // NOTE: 
            // We have to check if the photon is still inside the slab or has
            // gone outside it after interaction. We get this info from the photon's
            // position and velocity. If the velocity's Z is positive, meaning if
            // its going towards the bottom of the slab, then its distance from the
            // bottom slab surface is D - P.Z where D is the thickness of the slab.
            // and P.Z is the position of the photon in the Z-Axis.+ve Z Axis is
            // going along the thickness of the slab towards its bottom surface.
            double DistanceToSlabBoundary = 0;
            
            // Positive Z is going towards the bottom surface of the slab.
            if(PhotonVelZ > 0)
            {
                DistanceToSlabBoundary = (SlabThicknessZ - PhotonPZ) / PhotonVelZ;
            }
            else if(PhotonVelZ < 0)
            {
                DistanceToSlabBoundary = -PhotonPZ / PhotonVelZ;
            }
            
            // NOTE: Did the packet leave the Slab?
            if(S > DistanceToSlabBoundary)
            {
#ifdef ONED
                if(PhotonVelZ > 0)
                {
                    Transmission += PhotonPacketWeight;
                }
                else
                {
                    DiffuseReflectance += PhotonPacketWeight;
                }

#else
#if !VISUALIZE_ABSORPTION
                int Xi = (int)((PhotonPX + SlabSizeXY/2) / SlabSizeXY*Size);
                int Yi = (int)((PhotonPY + SlabSizeXY/2) / SlabSizeXY*Size);
                
                bool Cond = false;
#if VISUALIZE_TRANSMISSION
                Cond = (PhotonVelZ > 0);
#elif VISUALIZE_REFLECTANCE
                Cond = (PhotonVelZ < 0);
#endif
                
                if((Cond) && (Xi >= 0) && (PhotonPX < Size) && (Yi >= 0) &&
                   (Yi < Size))
                {
                    Records[Yi*Size + Xi] += (PhotonPacketUnabsorbedWeight);
                }
#endif
#endif
                break;
            }
            
            // NOTE: Move the photon Packet.
            PhotonPX += S*PhotonVelX;
            PhotonPY += S*PhotonVelY;
            PhotonPZ += S*PhotonVelZ;
            
            // NOTE: Photon Packet loses energy(Absorption)
            double DeltaWeight = Sigma_A / Sigma_T;
            // We decrease the weight since deltaWeight represents the
            // percentage of photons that have been absorbed. So we need to
            // decrease the "weight" of the packet.
            PhotonPacketUnabsorbedWeight -= DeltaWeight;
            PhotonPacketUnabsorbedWeight = PhotonPacketUnabsorbedWeight > 0.0 ? PhotonPacketUnabsorbedWeight : 0.0;
            if(PhotonPacketUnabsorbedWeight < 0.001)
            {
                if(Rand01() > 1.0 / M_RussianRoulette)
                {
                    // NOTE: Prematurely Kill the entire packet since the weight
                    // of the photon packet is too low.
#if VISUALIZE_ABSORPTION
                    int Xi = (int)((PhotonPX + SlabSizeXY / 2) / SlabSizeXY * Size);
                    int Yi = (int)((PhotonPY + SlabSizeXY / 2) / SlabSizeXY * Size);
                    if ((Xi >= 0) && (PhotonPX < Size) &&
                        (Yi >= 0) && (Yi < Size))
                    {
                        Records[Yi * Size + Xi] += (1.0);
                    }
#endif
                    break;
                }
                else
                {
                    // NOTE: Since we prematurely killed the acket above, to
                    // make sure we dont bias it and to make sure we sort of
                    // "counter" the bias above we add photons here to the
                    // photon packet. This is what this M is being used for
                    PhotonPacketUnabsorbedWeight *= M_RussianRoulette;
                }
            }
            
            Scatter(PhotonVelX, PhotonVelY, PhotonVelZ, G_HenyeyGreenstein);
        }
    }
#ifdef ONED
    printf("DiffuseReflectance: %f, Transmission: %f\n",
           DiffuseReflectance * Scale, Transmission * Scale);
#endif
}

int
main()
{
    srand(13);
    
    double *Records = nullptr;
    const uint32_t Size = 512;
    
    Records = new double[Size * Size * 3];
    memset(Records, 0, sizeof(double) * Size * Size * 3);
    
    uint32_t NumPassesAllowed = 10;
    uint32_t PassCount = 1;
    
    // The Image.
    float *Pixels = new float[Size * Size];
    
    while(PassCount < NumPassesAllowed)
    {
        MonteCarloLightTransportSimulation(Records, Size);
        
        for (int i = 0; i < Size * Size; ++i)
        {
            Pixels[i] = Records[i] / PassCount;
        }
        
        ++PassCount;
        printf("Pass Count: %d\n", PassCount);
    }

#if 0
    WritePPMImage("./out.ppm", Pixels, Size);
#else
    std::ofstream ofs;
    ofs.open("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << Size << " " << Size << "\n255\n";
    for (uint32_t i = 0; i < Size * Size; ++i)
    {
        float MinVal = Pixels[i] < 1.0f ? Pixels[i] : 1.0f;
        unsigned char Val = (unsigned char)(255 * MinVal);
        ofs << Val << Val << Val;
    }
    ofs.close();
#endif
    delete[] Records;
    delete[] Pixels;
    
    return 0;
}

#else

//[header]
// Monte Carlo simulation of light transport
//[/header]
//[compile]
// Download the mcsim.cpp file to a folder.
// Open a shell/terminal, and run the following command where the files is
// saved:
//
// c++ -O3 -o mcsim mcsim.cpp -std=c++11
//
// Run with: ./mcsim. Open the file ./out.png in Photoshop or any program
// reading PPM files.
//[/compile]
//[ignore]
// Copyright (C) 2012  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//[/ignore]

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>

// #define ONED
#define M_PI 3.141592653589793

inline float
Rand01()
{
    float Result = (float)rand() / (float)RAND_MAX;
    return Result;
}

double
getCosTheta(const double &g) // sampling the H-G scattering phase function
{
    if (g == 0)
        return 2 * Rand01() - 1;
    double mu = (1 - g * g) / (1 - g + 2 * g * Rand01());
    return (1 + g * g - mu * mu) / (2.0 * g);
}

// [comment]
// Combpute the new photon direction (due to scattering event)
// [/comment]
void
spin(double &mu_x, double &mu_y, double &mu_z, const double &g)
{
    double costheta = getCosTheta(g);
    double phi      = 2 * M_PI * Rand01();
    double sintheta = sqrt(1.0 - costheta * costheta); // sin(theta)
    double sinphi   = sin(phi);
    double cosphi   = cos(phi);
    if (mu_z == 1.0)
    {
        mu_x = sintheta * cosphi;
        mu_y = sintheta * sinphi;
        mu_z = costheta;
    }
    else if (mu_z == -1.0)
    {
        mu_x = sintheta * cosphi;
        mu_y = -sintheta * sinphi;
        mu_z = -costheta;
    }
    else
    {
        double denom     = sqrt(1.0 - mu_z * mu_z);
        double muzcosphi = mu_z * cosphi;
        double ux = sintheta * (mu_x * muzcosphi - mu_y * sinphi) / denom +
                    mu_x * costheta;
        double uy = sintheta * (mu_y * muzcosphi + mu_x * sinphi) / denom +
                    mu_y * costheta;
        double uz = -denom * sintheta * cosphi + mu_z * costheta;
        mu_x = ux, mu_y = uy, mu_z = uz;
    }
}

// [comment]
// Simulate the transport of light in a thin translucent slab
// [/comment]
void
MCSimulation(double *&records, const uint32_t &size)
{
    // [comment]
    // Total number of photon packets
    // [/comment]
    uint32_t nphotons = 100000;
    double scale      = 1.0 / nphotons;
    double sigma_a = 1, sigma_s = 2, sigma_t = sigma_a + sigma_s;
    double d = 4.5, slabsize = 0.5, g = 0.75;
    static const short m = 10;
    double Rd = 0, Tt = 0;
    for (int n = 0; n < nphotons; ++n)
    {
        double w = 1;
        double x = 0, y = 0, z = 0, mux = 0, muy = 0, muz = 1;
        while (w != 0)
        {
            double s              = -log(Rand01()) / sigma_t;
            double distToBoundary = 0;
            if (muz > 0)
                distToBoundary = (d - z) / muz;
            else if (muz < 0)
                distToBoundary = -z / muz;
            // [comment]
            // Did the pack leave the slab?
            // [/comment]
            if (s > distToBoundary)
            {
#ifdef ONED
                // compute diffuse reflectance and transmittance
                if (muz > 0)
                    Tt += w;
                else
                    Rd += w;
#else
                int xi = (int)((x + slabsize / 2) / slabsize * size);
                int yi = (int)((y + slabsize / 2) / slabsize * size);
                if (muz < 0 && xi >= 0 && x < size && yi >= 0 && yi < size)
                {
                    records[yi * size + xi] += w;
                }
#endif
                break;
            }
            // [comment]
            // Move photon packet
            // [/comment]
            x += s * mux;
            y += s * muy;
            z += s * muz;
            // [comment]
            // The photon packet looses energy (absorption)
            // [/comment]
            double dw = sigma_a / sigma_t;
            w -= dw;
            w = std::max(0.0, w);
            if (w < 0.001)
            { // russian roulette test
                if (Rand01() > 1.0 / m)
                    break;
                else
                    w *= m;
            }
            // [comment]
            // Scatter
            // [/comment]
            spin(mux, muy, muz, g);
        }
    }
#ifdef ONED
    printf("DiffuseReflectance %f Transmission %f\n", Rd * scale, Tt * scale);
#endif
}

int
main(int argc, char **argv)
{
    double *records     = NULL;
    const uint32_t size = 512;
    records             = new double[size * size * 3];
    memset(records, 0x0, sizeof(double) * size * size * 3);
    uint32_t npasses = 1;
    
    float *pixels = new float[size * size]; // image
    while (npasses < 64)
    {
        MCSimulation(records, size);
        for (int i = 0; i < size * size; ++i)
            pixels[i] = records[i] / npasses;
        // display(pixels);
        npasses++;
        printf("num passes: %d\n", npasses);
    }
    
    // save image to file
    std::ofstream ofs;
    ofs.open("./out.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << size << " " << size << "\n255\n";
    for (uint32_t i = 0; i < size * size; ++i)
    {
        unsigned char val = (unsigned char)(255 * std::min(1.0f, pixels[i]));
        ofs << val << val << val;
    }
    
    ofs.close();
    
    delete[] records;
    delete[] pixels;
    
    return 0;
}
#endif