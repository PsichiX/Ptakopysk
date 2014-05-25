varying vec2 coord;

void main()
{
	// to cut circle shape we have to calculate distance from current pixel to sprite center using texture coords from vertex shader.
	// if we will not transform texture coords from range <0.0; 1.0> to range <-1.0; 1.0> we will cut a quart of circle.
	float len = length( coord );
	// when we have distance to center of sprite, we have to calculate boolean value that determines if current pixel is inside circle of range 1.0.
	// to check if current pixel is inside curcle of range 1.0, we have to calculate ceil value of maximum range (1.0) minus distance to center.
	float f = ceil( 1.0 - len );
	// now we have to apply this boolean value as result pixel alpha.
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f );
}
