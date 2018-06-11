
// Look Up Table
typedef struct polar{
	int r;
	int z;
}polar_t;

// R start from (7", 15") Z in (7, 9, 11}

#define NUMBER_OF_R 	(uint8_t)10
#define NUMBER_OF_Z		(uint8_t)3

static polar_t const LUT[NUMBER_OF_R][NUMBER_OF_Z] = { 
	{{7000, 5358}, {8000, 8500}, {9000, 9000}},
		{{8000, 4744}, {8800, 6500}, {10000, 8000}},
			{{8500, 4122}, {9600, 5700},{11000,7300}},
				{{9000, 3700},{10200,5300},{11600,6800}}
					{{9800,3366},{11000,4869},{12200,6500}}
						{{10500,2830},{11700,4538},{13000,6000}}
							{{11200,2576},{12200,4200},{13600,5500}}
								{{12000,2326},{13000,3800},{14300,5228}}
									{{12700,2065},{13600,3435},{15000,4700}}
										{{13200,1600},{14500,3970},{16000,4472}}
		};

// milis inch, milis
polar_t calibration ( polar_t input ){
	polar_t local = {0, 0};
	int index1 =(input.r % 700);
	int index2 = (input.z % 700)/2;
	
	LUT[index1][index2] 
	LUT[index1+1][index2+1]
	LUT[index][index2+1]
	LUT[index+1][index2]
	
	
	return local;
 	
}