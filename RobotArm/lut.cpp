
// Look Up Table
typedef struct polar{
	int16_t r;
	int16_t z;
}polar_t;

// R start from (7", 15") Z in (7, 9, 11}

#define NUMBER_OF_R 	(uint8_t)10
#define NUMBER_OF_Z		(uint8_t)3

static polar_t const LUT[NUMBER_OF_R][NUMBER_OF_Z] = { 
	{{7000, 5358}, {8001, 8500}, {9000, 9000}},
		{{8000, 4744}, {8800, 6500}, {10000, 8000}},
			{{8500, 4122}, {9600, 5700},{11000,7300}},
				{{9000, 3700},{10200,5300},{11600,6800}}
					{{9800,3366},{11000,4869},{12200,6500}}
						{{10500,2830},{11700,4538},{13000,6000}}
							{{11200,2576},{12200,4200},{13600,5500}}
								{{12000,2326},{13000,3800},{14300,5228}}
									{{12700,2065},{13600,3435},{15000,4700}}
										{{13200,1600},{14500,3036},{16000,4472}}
		};
/**
	Thoughts:	Scanning from top row to bottom (from the least tot the greatest)
				Scanning of the greatest value of each row
				Searching for the first number that is greater then the comparant 
				and its next number is less then the comparant
				compare the adjacent cell for the smallest range
				Identify the smallest range for two adjacent cells
					--> One case is two cells are in diagonal cells
					--> Another case is two cells are in vertical or horizontal
				Identify the four adjacent cells for bilinear interpolation
					--> Maybe need the second variables to get the four cells
				
				Call the second variables and check if the second variables is inbound
				if success, both valuables agree, go for the inverse interpolation
				if fail, abort the current cell and continue with search
				
				if both valuables disagree by the end of the table search, 
				raise flag and call exception


**/
// milis inch, milis
polar_t calibration ( polar_t input ){
	polar_t local = {0, 0};
	uint8_t flag1 = 0, flag2 = 0 ;
	uint8_t index1, index2;
	
	for( int i = 0; i < NUMBER_OF_R ; i++ ){	/* From top to bottom, from the least to the greatest	*/
		flag1 = 0;	/* Reset the flag1 every row	*/
		
		for( int j = NUMBER_OF_Z; j != 0  ; j-- ){	/* From right to left, from the greatest value of each row	*/
			/**
				What are you doing? the value is either less or greater, optimize it please
			**/
			if ( LUT[i][j].r < input.r ){
				if ( flag1 == 1){
					flag2 = 1;	// raise flag2 for finding a range
					index1 = 
				}
				break;	/* Skip the row if the greatest value of a row is less than what we need	*/
			}else if ( LUT[i][j].r >= input.r ){
				if ( flag1 == 0 ){
					flag1 = 1;	// Raise flag1  for
				}
			}
			if (LUT[i][j].z < input.z ){
				
			}
			
			
		}
			
		
	}
	int index1 =(input.r % 700);
	int index2 = (input.z % 700)/2;
	
	LUT[index1][index2] 
	LUT[index1+1][index2+1]
	LUT[index][index2+1]
	LUT[index+1][index2]
	
	
	return local;
 	
}