varying vec2 coord;

void main()
{
	float len = length( coord );
	// so we have main circle cutted from sprite. now we want to cut another circle inside main circle,
	// so we have to make the same calculation as for first circle but with smaller range
	// and substract it's result from result of main circle. 
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f );
}
