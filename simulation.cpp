#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>


int	NowYear;		// 2021 - 2026
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		    // number of deer in the current population
float NowMeteorProb;    // current probability of a meteor shower
float NowGodsRageMeter; // current measurement of how angry gods are
bool NowMeteorDestruction;  // current state of the destructive nature of a meteor shower
                            // 1 == True, 0 == False

omp_lock_t	Lock;
int		NumInThreadTeam;
int		NumAtBarrier;
int		NumGone;

unsigned int seed = 0.;

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

float Ranf( unsigned int *seedp, float low, float high );
int Ranf( unsigned int *seedp, int ilow, int ihigh );
void set_temp( unsigned int *seed );
void set_precip( unsigned int *seed );
void AreGodsAngry( unsigned int *seed );
void set_gods_rage_meter( unsigned int *seed );
void set_meteor_destruction_imminence( );
void InitBarrier( );
void WaitBarrier( );
void SQR( );
float _ang( );

void Deer( );
void Grain( );
void MeteorShower( );
void Watcher( );

void	InitBarrier( int );
void	WaitBarrier( );

int main(int argc, char *argv[]) {

    #ifndef _OPENMP
    printf(stderr, "OpenMP not supported, exiting program.\n");
    return 1;
    #endif

        // starting date and time:
        NowMonth =    0;
        NowYear  = 2021;

        // starting state
        NowNumDeer = 1;
        NowHeight =  1.;
        NowMeteorProb = 0.;
        NowMeteorDestruction = 0;

        // initialize randomized factors
        set_temp(&seed);
        set_precip(&seed);
        AreGodsAngry(&seed);

        // set num of threads to 4, one for each agent
        omp_set_num_threads(4);

        // number of threads to wait at barrier
        InitBarrier( 4 );

        // create omp sections, one for each agent
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                Deer( );
            }

            #pragma omp section
            {
                Grain( );
            }

            #pragma omp section
            {
                Watcher( );
            }

            #pragma omp section
            {
                MeteorShower( );
            }
        }
        return 0;
}       // implied barrier -- all functions must return in order
        // to allow any of them to get past here

// initialize makeshift barrier
void
InitBarrier( int n )
{
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock( &Lock );
}

// makeshift barrier
void
WaitBarrier( )
{
    omp_set_lock( &Lock );
    {
        NumAtBarrier++;
        if( NumAtBarrier == NumInThreadTeam )
        {
                NumGone = 0;
                NumAtBarrier = 0;
                // let all other threads get back to what they were doing
                // before this one unlocks, knowing that they might immediately
                // call WaitBarrier( ) again:
                while( NumGone != NumInThreadTeam-1 );
                omp_unset_lock( &Lock );
                return;
        }
    }
    omp_unset_lock( &Lock );

    while( NumAtBarrier != 0 );	// this waits for the nth thread to arrive

    #pragma omp atomic
    NumGone++;			// this flags how many threads have returned 
}

// a function for squaring a value
float
SQR( float x )
{
    return x * x;
}

float _ang( )
{
    return ( 30. * ( float )NowMonth + 15. ) * ( M_PI / 180. );
}

// set temp factor
void set_temp( unsigned int *seed )
{
    float temp = AVG_TEMP - AMP_TEMP * cos( _ang( ) );
    NowTemp = temp + Ranf( seed, -RANDOM_TEMP, RANDOM_TEMP );
}

// set precipitation factor
void set_precip( unsigned int *seed )
{
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( _ang( ) );
    NowPrecip = precip + Ranf( seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
    {
        NowPrecip = 0.;
    }
}

// decides whether or not a meteor shower is destructive in nature (50% odds)
void 
set_meteor_destruction_imminence ( )
{
    int prob = rand( ) % 2;
    if ( prob == 1 )
    {
        NowMeteorDestruction = 1;
    }
    else
    {
        NowMeteorDestruction = 0;
    }
}

// randomizer
float
Ranf( unsigned int *seedp,  float low, float high )
{
    float r = (float) rand_r( seedp );              // 0 - RAND_MAX

    return( low + r * ( high - low ) / (float)RAND_MAX );
}

// randomizer
int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)( Ranf(seedp, low, high) );
}

// sets the initial probability of a meteor shower occuring
void
set_meteor_prob( unsigned int *seed )
{
    NowMeteorProb = Ranf( seed, 0.f, 1.f );
}

// measures how angry the gods are
void
AreGodsAngry( unsigned int *seed )
{
    NowGodsRageMeter = Ranf( seed, 0.f, 4.f );
    
}

// calculate the deer population based on carrying capacity
// and the occurence of a destructive meteor shower
void 
Deer( ) 
{
    int deerPopulation;
    int carryingCapacity;
    bool meteor;

    while ( NowYear < 2027 ) 
    {
        deerPopulation = NowNumDeer;
        carryingCapacity = NowHeight;
        meteor = NowMeteorDestruction;
        
        // if there is an abundance of grain, increment deer population
        if ( (float)deerPopulation < carryingCapacity )
        {
            deerPopulation++;
        }

        // if there is a shortage of grain, decrement deer population
        else if ( (float)deerPopulation > carryingCapacity )
        {
            deerPopulation--;
        }

        // pad any negative values to 0
        if ( deerPopulation < 0. )
        {
            deerPopulation = 0.;
        }

        // in the case of a destructive meteor shower,
        // set deer population to 0
        if ( meteor == 1 )
        {
            deerPopulation = 0.;
        }

        // done computing
        WaitBarrier( );

        // share computed deer count
        NowNumDeer = deerPopulation;

        // done assigning
        WaitBarrier( );

        // done printing
        WaitBarrier( );

    }

}

// calculate grain height based on precipitation and temperature
void
Grain( )
{
    float grainHeight;
    float tempFactor;
    float precipFactor;
    bool meteor;

    while ( NowYear < 2027 )
    {
        tempFactor = exp( -SQR(( NowTemp - MIDTEMP ) / 10. ));
        precipFactor = exp( -SQR(( NowPrecip - MIDPRECIP ) / 10. ));

        meteor = NowMeteorDestruction;
        
        // compute grain height based on environmental factors
        grainHeight = NowHeight;
        grainHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        grainHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        // pad negative grain height to 0
        if ( grainHeight < 0. )
        {
            grainHeight = 0.;
        }

        // in the case of a destructive meteor shower,
        // set grain height to 0
        if ( meteor == 1 )
        {
            grainHeight = 0.;
        }

        // done computing
        WaitBarrier( );

        // share computed grain height
        NowHeight = grainHeight;

        // done assigning
        WaitBarrier( );

        // done printing
        WaitBarrier( );
    }
}

// determine whether a meteor shower enters the earth's atmosphere
void
MeteorShower( )
{
    float meteorShowerProb;
    float godsRageMeter;

    while ( NowYear < 2027 )
    {
        meteorShowerProb = NowMeteorProb;
        godsRageMeter = NowGodsRageMeter;

        // calculate the final probability of a meteor shower entering the earth's atmosphere
        // = initial probability * the measurement of how angry the gods are
        meteorShowerProb = meteorShowerProb * godsRageMeter;

        // pad any probabilities greater than 1 to just 1
        if ( meteorShowerProb >= 1. )
        {
            meteorShowerProb = 1.;
        }

        // done computing
        WaitBarrier( );

        // share computed meteor shower probability
        NowMeteorProb = meteorShowerProb;

        // done assigning
        WaitBarrier( );

        // done printing
        WaitBarrier( );
    }
}

// calculate environmental factors, increment time, and print results
void
Watcher( )
{
    // open a file for writing
	FILE *data = std::fopen("data.csv", "w+");

    // write column headers into opened file
	fprintf(data, "Month,Temperature (C),Precipitation (cm),Height (cm),Deer,Meteor Destruction\n");
    
    float tempC;
    float heightCM;
    float precipCM;
    int month;
    float godsRageMeter;
    float meteorProb;

    while ( NowYear < 2027 )
    {
        // wait for other threads to finish computing
        WaitBarrier( );

        // wait for other threads to finish assigning variables
        WaitBarrier( );

        month = NowMonth + 12 * ( NowYear - 2021 );
        tempC = ( 5. / 9. ) * ( NowTemp - 32 );
        heightCM = 2.54 * NowHeight;
        precipCM = 2.54 * NowPrecip;
        meteorProb = NowMeteorProb;

        // write monthly simulation results into opened file

        // printf( "month: %d \t tempC: %.2f \t precipCM: %.2f \t heightCM: %.2f \t NowNumDeer: %d \t meteorDestruction: %d\n", month, tempC, precipCM, heightCM, NowNumDeer, NowMeteorDestruction );
        fprintf( data, "%d,%.2f,%.2f,%.2f,%d,%d\n", month, tempC, precipCM, heightCM, NowNumDeer, NowMeteorDestruction );
        
        // increment time
        if ( ++NowMonth > 11 )
        {
            NowMonth = 0.;
            NowYear++;
        }

        // reset the occurence of a destructive meteor shower back to 0 (False)
        if ( NowMeteorDestruction == 1 )
        {
            NowMeteorDestruction = 0;
        }

        // if the probability of a meteor shower entering the earth's atmopsphere is 1,
        // determine if it is destructive
        if ( meteorProb == 1. ) 
        {
            set_meteor_destruction_imminence( );
        }

        // recalculate environmental and external factors
        set_temp( &seed );
        set_precip( &seed );
        set_meteor_prob( &seed );
        AreGodsAngry( &seed );

        // done printing
        WaitBarrier( );
    }
    
    // close out the file
    std::fclose(data);
}
