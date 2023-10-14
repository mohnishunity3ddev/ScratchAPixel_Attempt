#include <cstdio>
#include <cstdlib>
#include <random>

static const int MAX_NUM = 20; // items in the population are numbered (number between 0 and 20)
static const int MAX_FREQ = 50; // number of items holding a particular number varies between 1 and 50

/*
void
SomeExample()
{
    int minSampples = 16; // minimum sample size
    int maxSampples = 32; // maximum sample size
    
    std::mt19937 rng;
    
    int population[MAX_NUM + 1];
    int popSize                 = 0;
    float popMean               = 0;
    float popVar                = 0;
    static const int numSamples = 1000;
    
    rng.seed(17);
    
    // creation population
    std::uniform_int_distribution<uint32_t> distr(1, MAX_FREQ);
    
    for (int i = 0; i <= MAX_NUM; ++i)
    {
        population[i] = distr(rng);
        popSize += population[i];
        popMean += population[i] * i;    // prob * x_i
        popVar += population[i] * i * i; // prob * x_i^2
    }
    
    popMean /= popSize;
    popVar /= popSize;
    popVar -= popMean * popMean;
    fprintf(stderr, "size %d mean %f var %f\n", popSize, popMean, popVar);
    
    // NOTE: The parameters given to its constructor are basically - the minimum
    // value this generates, and the max value that it generates. This
    // distribution will return a value between the ranges specified here.
    std::uniform_int_distribution<uint32_t> n_samples_distr(minSampples,
                                                            maxSampples);
    std::uniform_int_distribution<uint32_t> pick_item_distr(0, popSize - 1);
    
    float expectedValueMean = 0, varianceMean = 0;
    // now that we have some data and stats to work with sample it
    for (int i = 0; i < numSamples; ++i)
    {
        int n             = n_samples_distr(rng); // sample size
        float sample_mean = 0, sample_variance = 0;
        // draw samples from the population and compute stats
        for (int j = 0; j < n; ++j)
        {
            int item_index = pick_item_distr(rng), k;
            for (k = 0; k <= MAX_NUM; ++k)
            {
                item_index -= population[k];
                if (item_index < 0)
                    break;
            }
            // k is the value we picked up from the population,
            // this is the outcome of a number between [0:20]
            sample_mean += k;
            sample_variance += k * k;
        }
        sample_mean /= n;
        sample_variance /= n;
        sample_variance -= sample_mean * sample_mean;
        
        float c1[3] = {1, 0, 0};
        float c2[3] = {0, 0, 1};
        float t     = (n - minSampples) / (float)(maxSampples - minSampples);
        float r     = c1[0] * (1 - t) + c2[0] * t;
        float g     = c1[1] * (1 - t) + c2[1] * t;
        float b     = c1[2] * (1 - t) + c2[2] * t;
        printf("sample mean: %f sample variance: %f col: %f %f %f;\n",
               sample_mean, sample_variance, r, g, b);
    }
}
*/

void
TrainExample()
{
    srand(13);
    
    int NumSimulations = 10000;
    int NumBins        = 100;
    int Bins[NumBins];
    memset(Bins, 0, sizeof(int) * NumBins);
    
    const float Distance = 10;
    
    for(int I = 0; 
        I < NumSimulations; 
        ++I)
    {
        float MyRand = ((float)rand()) / (float)RAND_MAX;
        // Random Number between -5 and 5.
        float Diff = (2*MyRand - 1) * 5;
        int WhichBin = (int)(NumBins * (Diff / 5 + 1) * 0.5f);
        Bins[WhichBin]++;
        float Time = 30 + Diff;
        float Speed = 60*Distance / Time;
    }

    float Sum = 0;
    for(int I = 0; 
        I < NumBins; 
        ++I)
    {
        float Normalized = Bins[I] / (float)NumSimulations;
        printf("%f %f\n", 5 * (2 * (I / (float)(NumBins)) - 1), Normalized);
        Sum += Normalized;
    }
    
    fprintf(stderr, "sum %f\n", Sum);
}

int
main(int argc, char **argv)
{
    // SomeExample();
    TrainExample();
    
    return 0;
}