#include "../include/LFeatures.h"

#include <math.h>

typedef enum 
{
	L_EMPTY,
	L_NORMAL_POINT,
	L_END_POINT,
	L_BRANCH_POINT,	
} points;

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

int image_counter(LImage* image, int i, int j)
{
	int count = 0;
	if( i!=0 && i!=(n-1) && j!=0 && j!=(n-1) )
	{
		if(image->grid[get_neighbour(i,j,0)]) count++;	
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,2)]) count++;
		if(image->grid[get_neighbour(i,j,3)]) count++;	
		if(image->grid[get_neighbour(i,j,4)]) count++;
		if(image->grid[get_neighbour(i,j,5)]) count++;
		if(image->grid[get_neighbour(i,j,6)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;										
	}	
	else if(i!=0 && i!=(n-1) && j!=0 && j==(n-1))
	{	
		if(image->grid[get_neighbour(i,j,0)]) count++;
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,5)]) count++;
		if(image->grid[get_neighbour(i,j,6)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;	
	}
	else if( i!=0 && i!=(n-1) && j==0 && j!=(n-1) )
	{
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,2)]) count++;
		if(image->grid[get_neighbour(i,j,3)]) count++;
		if(image->grid[get_neighbour(i,j,4)]) count++;
		if(image->grid[get_neighbour(i,j,5)]) count++;
	}
	else if( i!=0 && i==(n-1) && j!=0 && j!=(n-1) )
	{
		if(image->grid[get_neighbour(i,j,0)]) count++;
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,2)]) count++;
		if(image->grid[get_neighbour(i,j,3)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;
	}
	else if( i==0 && i!=(n-1) && j!=0 && j!=(n-1) )
	{					
		if(image->grid[get_neighbour(i,j,3)]) count++;
		if(image->grid[get_neighbour(i,j,4)]) count++;
		if(image->grid[get_neighbour(i,j,5)]) count++;
		if(image->grid[get_neighbour(i,j,6)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;						
	}
	else if( i==0 && i!=(n-1) && j==0 && j!=(n-1) )
	{	
		if(image->grid[get_neighbour(i,j,3)]) count++;
		if(image->grid[get_neighbour(i,j,4)]) count++;
		if(image->grid[get_neighbour(i,j,5)]) count++;										
	}	
	else if( i==0 && i!=(n-1) && j!=0 && j==(n-1) )
	{						
		if(image->grid[get_neighbour(i,j,5)]) count++;
		if(image->grid[get_neighbour(i,j,6)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;							
	}	
	else if( i!=0 && i==(n-1) && j==0 && j!=(n-1) )
	{					
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,2)]) count++;
		if(image->grid[get_neighbour(i,j,3)]) count++;						
	}	
	else if( i!=0 && i==(n-1) && j!=0 && j==(n-1) )
	{						
		if(image->grid[get_neighbour(i,j,0)]) count++;
		if(image->grid[get_neighbour(i,j,1)]) count++;
		if(image->grid[get_neighbour(i,j,7)]) count++;							
	}					
	return count;
}

int get_neighbour(int i, int j, int neighbour)
{
	if(neighbour == 0 || neighbour == 8)
	{
		return (i-1)*n+j-1;
	}
	else if(neighbour == 1)
	{
		return (i-1)*n+j;
	}
	else if(neighbour == 2)
	{
		return (i-1)*n+j+1;
	}
	else if(neighbour == 3)
	{
		return (i)*n+j+1;
	}
	else if(neighbour == 4)
	{
		return (i+1)*n+j+1;
	}
	else if(neighbour == 5)
	{
		return (i+1)*n+j;
	}
	else if(neighbour == 6)
	{
		return (i+1)*n+j-1;
	}
	else if(neighbour == 7)
	{
		return (i)*n+j-1;
	}
	return 0;
}

int wh_space(image, int i, int j)
{
	int number_wh_space = 0;
	for(int k = 0; k < 8; k++)
	{
		if(image->grid[get_neighbour(i, j, k)] != image->grid[get_neighbour(i, j, k + 1)] && !image->grid[get_neighbour(i, j, k + 1)]) number_wh_space++;
	}
	return number_wh_space
}

List* image_point_remover(LImage* image)
{
	int n = image->size;
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
			if(image->grid[i*n+j])
			{
				int count = image_counter(image, i, j);
				if(count >= 6)
				{
					image->grid[i*n+j] = 0;
				}
				if(count >= 2 && count <= 5)
				{
					int whiteCount = 0;
					int state = 0;
					int sec_state = 0;
					for(int k; k < 8; k++)
					{
						if(state == 0)
						{
							if(image->grid[get_neighbour(i, j, k)]) 
							{
								state = 1;
							}
						}
						else if(state == 1)
						{
							if(!image->grid[get_neighbour(i, j, k)]) state = 2;	
						}
						else if(state == 2)
						{
							
							if(image->grid[get_neighbour(i, j, k)])
							{
								if(k%2 == 1) // Als er een enkele whitespace tussen 2 zwarte vakjes zit op een hoekpunt.
								{
									state = 1;
								}
								else 
								{
									state = 0;
									sec_state++;
								}
							}
							else
							{
								state = 0;
								sec_state++;
								
							}
						}
						
						
					}
					if(sec_state <= 1)
					{
						image->grid[i*n+j] = 0;
					}
				}
			}
		}
	}
	return 0;	
}
List* image_end_points(LImage* image)
{
	int n = image->size;
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
			if(image->grid[i*n+j])
			{
				int count = image_counter(image, i, j);
				if(count == 1)
				{
					image->grid[i*n+j] = 2;
				}
			}
		}
	}
	return 0;	
}
List* image_cross_points(LImage* image){
	return 0;
}
