
# UNL Robotic Team 2018 Repository

Master control of competition rover with affiliate libraries.

# Attention

Please resolve conflict before you commit the files. Branch off if necessary.

### Update 06/09/16 
- added ballCoordinate.h lib 
- this lib will provide a tool to parse data package and tokenize it to global variable 
where use can access by using this lib.
- Packages Format:
	- `2;66,12;82,43;`
	- `1;82,103;`

	- Where:

		- 2(1st Token): # of coordinate that contain in the package
		-66(2nd Token,1st Point): represent the color of the ball.represent in ascii 
of the 
char 
'B'
		- 12(2nd Token,2nd Point): y-axis value being pass
- End the package format with `;` to ensure there will be no load of rubish.

