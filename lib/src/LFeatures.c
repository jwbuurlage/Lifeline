#include "../include/LFeatures.h"

#include <math.h>

void image_histogram(LImage* image)
{
	return;
}

int factorial(int num){
	switch(num){
	case 0: return 1;
	case 1: return 1;
	case 2: return 2;
	case 3: return 6;
	case 4: return 24;
	case 5: return 120;
	case 6: return 720;
	default: return num*factorial(num-1);
	}
}

float zernikeFunction(int n, int m, float rho)
{
	float sum = 0.0f;
	int absM = abs(m);
	int sMax = n - absM;

	for(int s = 0; s <= sMax; ++s)
	{
		float factor = factorial(2*n + 1 - s)/((float)( factorial(s) * factorial(n-absM-s) * factorial(n+absM+1-s) ));
		factor *= pow(rho, n - s);
		//Multiply with (-1)^s is the same as: if( s is odd ) apply minus sign
		if( s & 1 ) sum -= factor;
		else sum += factor;
	}
	return sum;
}

void image_moments(LImage* image, LFeatureSet* output)
{
	//Set the moments to zero
	memset(output->geometricMoments, 0, sizeof(output->geometricMoments));
	memset(output->zernikeMoments, 0, sizeof(output->zernikeMoments));

	int n = image->size;
	float mid = n/2;
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
			if(image->grid[i*n+j])
			{
				//We need to add this pixels coordinate to the sum
				//But the coordinates are first mapped to [-1,1]
				float x = ((float)j-mid)/mid;
				float y = ((float)i-mid)/mid;
				
				// Geometric moments
				for(int p = 1; p <= MAX_GEOMETRIC_ORDER; ++p)
					for(int q = 1; q <= MAX_GEOMETRIC_ORDER; ++q)
						output->geometricMoments[p-1][q-1] += pow(x, p)*pow(y,q);

				// Pseudo-Zernike moments
				float rho = sqrt(x*x + y*y);
				float theta = atan2(y,x);
				for(int n = 1; n <= MAX_ZERNIKE_N; ++n){
					for(int m = -n; m <= n; ++m){
						float R = zernikeFunction(n, m, rho);
						output->zernikeMoments[n-1][m+n][0] = R*cos(m*theta);
						output->zernikeMoments[n-1][m+n][1] = R*sin(m*theta);
					}
				}
			}
		}
	}
	return;
}

float image_zernike_moment(LImage* image)
{
	return 10.0f;
}

int image_branch_points(LImage* image)
{
	return 0;
}

int image_end_points(LImage* image)
{	
    int n = image->size;
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(image->grid[i*n+j])
            {
                int count = 0;
                if( i!=0 && i!=(n-1) && j!=0 && j!=(n-1) )
                {					
                    if(image->grid[i*n+j+1]) count++;	
                    if(image->grid[i*n+j-1]) count++;
                    if(image->grid[(i-1)*n+j+1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;
                    if(image->grid[(i-1)*n+j-1]) count++;
                    if(image->grid[(i+1)*n+j+1]) count++;
                    if(image->grid[(i+1)*n+j]) count++;
                    if(image->grid[(i+1)*n+j-1]) count++;							
                }	
                else if(i!=0 && i!=(n-1) && j!=0 && j==(n-1))
                {	
                    if(image->grid[i*n+j-1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;
                    if(image->grid[(i-1)*n+j-1]) count++;
                    if(image->grid[(i+1)*n+j]) count++;
                    if(image->grid[(i+1)*n+j-1]) count++;	
                }
                else if( i!=0 && i!=(n-1) && j==0 && j!=(n-1) )
                {
                    if(image->grid[i*n+j+1]) count++;	
                    if(image->grid[(i-1)*n+j+1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;
                    if(image->grid[(i+1)*n+j+1]) count++;
                    if(image->grid[(i+1)*n+j]) count++;	
                }
                else if( i!=0 && i==(n-1) && j!=0 && j!=(n-1) )
                {
                    if(image->grid[i*n+j+1]) count++;	
                    if(image->grid[i*n+j-1]) count++;
                    if(image->grid[(i-1)*n+j+1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;
                    if(image->grid[(i-1)*n+j-1]) count++;
                }
                else if( i==0 && i!=(n-1) && j!=0 && j!=(n-1) )
                {					
                    if(image->grid[i*n+j+1]) count++;	
                    if(image->grid[i*n+j-1]) count++;
                    if(image->grid[(i+1)*n+j+1]) count++;
                    if(image->grid[(i+1)*n+j]) count++;
                    if(image->grid[(i+1)*n+j-1]) count++;							
                }
                else if( i==0 && i!=(n-1) && j==0 && j!=(n-1) )
                {					
                    if(image->grid[i*n+j+1]) count++;	
                    if(image->grid[(i+1)*n+j+1]) count++;
                    if(image->grid[(i+1)*n+j]) count++;							
                }	
                else if( i==0 && i!=(n-1) && j!=0 && j==(n-1) )
                {						
                    if(image->grid[i*n+j-1]) count++;;
                    if(image->grid[(i+1)*n+j]) count++;
                    if(image->grid[(i+1)*n+j-1]) count++;							
                }	
                else if( i!=0 && i==(n-1) && j==0 && j!=(n-1) )
                {					
                    if(image->grid[i*n+j+1]) count++;	;
                    if(image->grid[(i-1)*n+j+1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;							
                }	
                else if( i!=0 && i==(n-1) && j!=0 && j==(n-1) )
                {						
                    if(image->grid[i*n+j-1]) count++;
                    if(image->grid[(i-1)*n+j]) count++;
                    if(image->grid[(i-1)*n+j-1]) count++;							
                }
                if(count == 1)
                {
                    image->grid[i*n+j] = 2;
                }	
            }
		}
	}
	return 0;
}

int image_cross_points(LImage* image){
	return 0;
}