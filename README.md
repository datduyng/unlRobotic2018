
# UNL Robotic Team 2018 Repository

Master control of competition rover with affiliate libraries.

# Attention

Please resolve conflict before you commit the files. Branch off if necessary.

### Update 07/28/18 
##### updated ballCoordinate.h lib 
- this lib will provide a tool to parse data package and tokenize it to global variable 
where use can access by using this lib.
- Packages Format:
	 * ;C1,x1,y1,z1;C2,x2,y2,z2;.....; 
	 * ;1,5,12,32;0,12,43,53;
	 * ;1,3,103,12;
	 *
	 * Where:
	 * C: color where ( R:0, g:1, b:2)
	 * x: x-axis value being pass. 
	 * y: y-axis value being pass
	 * z: z-axis value being pass

- End the package format with `;` to ensure there will be no load of rubish.

##### updated CameraArmDriver.h lib 
- recalibrate every time switching servo


## link to Library ( 2018) 
- `Driving.h`
	- [Driving.h](https://github.com/datduyng/Driving.git)
- `SCC_Driver.h`
	- [SCC_Driver](https://github.com/datduyng/SCC_Driver.git)
- `ballCoordinate.h`
	- [ballCoordinate.h](https://github.com/datduyng/ballCoordinate.git)
- `robotPlanner.h`
	- [robotPlanner](https://github.com/datduyng/RobotPlanner.git)
- `CameraArmDriver.h` 
	- [CameraArmDriver.h](https://github.com/datduyng/CameraArmDriver.git)
- `LiquidCrystal_I2C.h`
	- [LiquidCrystal_I2C.h](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
- `NewPing.h`
	- [NewPing](https://github.com/microflo/NewPing)
- `SabertoothSimplified.h`
	- [find in this repo](https://github.com/vrege/RoboConBPGC)
