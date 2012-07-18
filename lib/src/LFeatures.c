#include "../include/LFeatures.h"

#include <cmath>

void image_histogram(LImage* image)
{
	return;
}

float image_moment(LImage* image, int orderX, int orderY)
{
	float sum = 0;
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
				sum += pow(x, orderX) * pow(y, orderY);
			}
		}
	}
	return sum;
}

float image_zernike_moment(LImage* image)
{
	return 10.0f;
}

List* image_branch_points(LImage* image)
{
	return 0;
}

List* image_end_points(LImage* image)
{
	int n = image->size;
	float mid = n/2;
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
						image->grid[i*n+j] = 2
					}	
				}
		}
	}
	return 0;
}

List* image_cross_points(LImage* image){
	return 0;
}
