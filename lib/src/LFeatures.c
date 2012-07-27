#include "../include/LFeatures.h"

#include <math.h>
#include <stdio.h>

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
			if(image->grid[i*n+j].enabled)
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

int get_neighbour(int i, int j, int neighbour, int n)
{
	neighbour = neighbour % 8;
	
	//Check edges
	if( i == 0 ){
		if( neighbour <= 2 ) return n*n;
	}else if( i == n-1 ){
		if( neighbour >= 4 && neighbour <= 6 ) return n*n;
	}
	if( j == 0 ){
		if( neighbour >= 6 || neighbour == 0 ) return n*n;
	}else if( j == n-1 ){
		if( neighbour >= 2 && neighbour <= 4 ) return n*n;
	}
	switch(neighbour)
	{
		case 0: return (i-1)*n+j-1;
		case 1: return (i-1)*n+j;
		case 2: return (i-1)*n+j+1;
		case 3: return (i)*n+j+1;
		case 4: return (i+1)*n+j+1;
		case 5: return (i+1)*n+j;
		case 6: return (i+1)*n+j-1;
		case 7: return (i)*n+j-1;
		default: return n*n;
	}
}

int image_counter(LImage* image, int i, int j, int n)
{
	int count = 0;
	for(int k = 0; k < 8; ++k) if(image->grid[get_neighbour(i,j,k,n)].enabled) count++;
	return count;
}

int image_thin(LImage* image)
{
	int n = image->size;
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
			if(image->grid[i*n+j].enabled)
			{
				//We want to check if we can remove this pixel
				//We have to check it the surrounding 8 pixels are
				//still connected when we remove this pixel			
					
				int count = image_counter(image, i, j, n);
				if(count >= 7) //With 7 or more surrounding pixels they will always be connected
				{
					image->grid[i*n+j].enabled = 0; //So we can remove the center pixel
				}
				else if(count >= 2)
				{
					//Now we want to check if the surrounding pixels are connected
					//We find the first black pixel, then from there search for the
					//first white pixel that might disconnect it
					int firstBlack = -1;
					int state = 0;
					int disconnections = 0;
					for(int k = 0; k < 16; k++)
					{
						if(state == 0) //Searching for black pixel
						{
							if(image->grid[get_neighbour(i, j, k, n)].enabled) 
							{
								if( firstBlack == -1 ) firstBlack = k;
								state = 1;
							}
						}
						else if(state == 1) //Black pixel was found, keep following untill white
						{
							if(!image->grid[get_neighbour(i, j, k, n)].enabled) state = 2;	
						}
						else if(state == 2) //White pixel found. Did this white pixel disconnect the black pixels?
						{
							//Check the pixel after the white pixel
							if(image->grid[get_neighbour(i, j, k, n)].enabled)
							{
								//There was a black pixel after the white pixel
								//If the white pixel was at a corner, then it
								//is not disconnected. If the white pixel is in the middle
								//then it is disconnected
								if(k%2 == 1)
									state = 1;
								else 
								{
									state = 0;
									disconnections++;
								}
							}
							else //A second white pixel. Then yes this was a disconnection
							{
								state = 0;
								disconnections++;
								
							}
						}
						//Check if we already went a full round
						if( k >= 8 ){
							if( (k-8) >= firstBlack ) break;						
						}
					}
					//If the 8 pixels have one disconnection or less
					//Then they are still connected so we can remove this pixel
					if(disconnections <= 1)
					{
						image->grid[i*n+j].enabled = 0;
					}
				}
			}
		}
	}
	return 0;	
}

int image_end_points(LImage* image)
{
	int n = image->size;
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
			if(image->grid[i*n+j].enabled)
			{
				int count = image_counter(image, i, j, n);
				if(count == 1)
				{
					image->grid[i*n+j].type = 1;
				}
			}
		}
	}
	return 0;	
}

int image_counter_dummy(LImage* image, int i, int j, int n)
{
	int count = 0;
	for(int k = 0; k < 8; ++k) if(image->grid[get_neighbour(i,j,k,n)].dummy == 1) count++;
	return count;
}

int image_branch_points(LImage* image)
{
	int n = image->size;
	//First mark all pixels with more than 2 neighbors with a dummy variable
	for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
			int count = image_counter(image, i, j, n);
			if(image->grid[i*n+j].enabled && count > 2)
			{
				image->grid[i*n+j].dummy = 1;
			}else{
				image->grid[i*n+j].dummy = 0;
			}
		}
	}
	for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		int count_dummy = image_counter_dummy(image, i, j, n);
      		if(image->grid[i*n+j].dummy == 1 && count_dummy > 1)
      		{
      			for(int k = 0; k < 16; k++)
      			{
  					if(image->grid[get_neighbour(i, j, k, n)].dummy == 1 && image->grid[get_neighbour(i, j, k + 2, n)].dummy == 1) 
  					{
  						image->grid[get_neighbour(i, j, k, n)].dummy = 0;
  						image->grid[get_neighbour(i, j, k + 2, n)].dummy = 0;
  						image->grid[get_neighbour(i, j, k + 1, n)].enabled = 1;
  						image->grid[get_neighbour(i, j, k + 1, n)].type = 2;
  						image->grid[i*n+j].dummy = 0;
  					}

      			}     			
      		}
      	}
    }
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		if(image->grid[i*n+j].type == 2)
      		{
      			for(int k = 0; k < 8; k++)
      			{
      				if(image->grid[get_neighbour(i, j, k, n)].dummy == 1) image->grid[get_neighbour(i, j, k, n)].dummy = 0;
      				if(image->grid[get_neighbour(i, j, k, n)].type == 2) image->grid[get_neighbour(i, j, k, n)].type = 0;
      			}
      		}
      	} 	
    }
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		if(image->grid[i*n+j].dummy == 1) 
      		{
      			image->grid[i*n+j].dummy = 0;
      			image->grid[i*n+j].type = 2;
      		}
      	}
    }
    for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		if(image->grid[i*n+j].type == 2)
      		{
      			for(int k = 0; k < 8; k++)
      			{
      				if(image->grid[get_neighbour(i, j, k, n)].type == 2) image->grid[get_neighbour(i, j, k, n)].type = 0;
      			}
      		}
      	}
    }
	return 0;
}

int image_branch_points_count(LImage* image)
{
	int n = image->size;
	int number_branch_points = 0;
	for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		if(image->grid[i*n+j].type == 2) number_branch_points++;
      	}
    }
	return number_branch_points;
}

int image_end_points_count(LImage* image)
{
	int n = image->size;
	int number_end_points = 0;
	for(int i = 0; i < n; ++i)
    {
    	for(int j = 0; j < n; ++j)
      	{
      		if(image->grid[i*n+j].type == 1) number_end_points++;
      	}
    }
	return number_end_points;
}

int image_connected_components(LImage* image)
{
	int n = image->size;
	int number_connected_components = 0;
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
  		{
  			if(image->grid[i*n+j].type == 1 && image->grid[i*n+j].componentNum == 0)
  			{
  				number_connected_components++;
  				image->grid[i*n+j].componentNum = number_connected_components;
  				for(int x = 0; x < n * n / 4; ++x)
  				{
	  				for(int i = 0; i < n; ++i)
					{
						for(int j = 0; j < n; ++j)
	  					{
	  						if(image->grid[i*n+j].enabled && image->grid[i*n+j].componentNum == 0)
	  						{
	  							for(int k = 0; k < 8; k++)
	  							{
	  								if(image->grid[get_neighbour(i, j, k, n)].componentNum == number_connected_components) image->grid[i*n+j].componentNum = number_connected_components;
	  							}
	  						}
	  					}
	  				}
	  			}
  			}
  		}
	}
	return number_connected_components;
	
}

int image_points_curvature(LImage* image)
{
	int n = image->size;
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			int count = image_counter(image, i, j, n);
			if(image->grid[i*n+j].enabled && count == 2)
			{
				for(int k = 0; k < 8; k++)
	  			{
	  				if(image->grid[get_neighbour(i, j, k, n)].enabled)
	  				{
	  					if(image->grid[get_neighbour(i, j, (k + 3) % 8, n)].enabled) image->grid[i*n+j].curvature = 1;
	  					if(image->grid[get_neighbour(i, j, (k - 3) % 8, n)].enabled) image->grid[i*n+j].curvature = 1;
	  					if(image->grid[get_neighbour(i, j, (k + 2) % 8, n)].enabled) image->grid[i*n+j].curvature = 2;
	  					if(image->grid[get_neighbour(i, j, (k - 2) % 8, n)].enabled) image->grid[i*n+j].curvature = 2;
	  					if(image->grid[get_neighbour(i, j, (k + 1) % 8, n)].enabled) image->grid[i*n+j].curvature = 3;
	  					if(image->grid[get_neighbour(i, j, (k - 1) % 8, n)].enabled) image->grid[i*n+j].curvature = 3;
	  				}
	  			}
			}
		}
	}
	return 0;
}

int image_line_type(LImage* image)
{
	int n = image->size;
	int diagonal_number = 0;
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			int count = image_counter(image, i, j, n);
			if(image->grid[i*n+j].enabled && count == 2)
			{
				if(image->grid[get_neighbour(i, j, 0, n)].enabled && image->grid[get_neighbour(i, j, 4, n)].enabled)
				{
					diagonal_number++;
					image->grid[i*n+j].diagonalNum = diagonal_number;
					for(int x = 0; x < sqrt(2) * n; ++x)
	  				{
		  				for(int i = 0; i < n; ++i)
						{
							for(int j = 0; j < n; ++j)
		  					{
		  						int countt = image_counter(image, i, j, n);
		  						if(image->grid[i*n+j].enabled && image->grid[i*n+j].diagonalNum == 0 && countt == 2)
		  						{
		  							for(int k = 0; k < 8; k++)
		  							{
		  								if(image->grid[get_neighbour(i, j, k, n)].diagonalNum == diagonal_number && image->grid[get_neighbour(i, j, (k + 4) % 8, n)].enabled) image->grid[i*n+j].diagonalNum = diagonal_number;
		  							}
		  						}
		  					}
		  				}
		  			}
				}
				if(image->grid[get_neighbour(i, j, 2, n)].enabled && image->grid[get_neighbour(i, j, 6, n)].enabled)
				{
					diagonal_number++;
					image->grid[i*n+j].diagonalNum = diagonal_number;
					for(int x = 0; x < sqrt(2) * n; ++x)
	  				{
		  				for(int i = 0; i < n; ++i)
						{
							for(int j = 0; j < n; ++j)
		  					{
		  						int countt = image_counter(image, i, j, n);
		  						if(image->grid[i*n+j].enabled && image->grid[i*n+j].diagonalNum == 0 && countt == 2)
		  						{
		  							for(int k = 0; k < 8; k++)
		  							{
		  								if(image->grid[get_neighbour(i, j, k, n)].diagonalNum == diagonal_number && image->grid[get_neighbour(i, j, (k + 4) % 8, n)].enabled) image->grid[i*n+j].diagonalNum = diagonal_number;
		  							}
		  						}
		  					}
		  				}
		  			}
				}
			}
		}
	}
	return 0;
}

int image_cross_points(LImage* image){
	return 0;
}
