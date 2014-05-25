varying vec2 coord;

void main()
{
	// when we have cutted ring from sprite, we want to paint this ring using gradient of alpha value.
	// first we have to get normalized direction vector from sprite center to current pixel position.
	vec2 nc = normalize( coord );
	// so how use this normal vector, you ask me? we will use it for dot-product operation!
	// dot-product gives us cosinus value of angle between two normalized vectors.
	// this cosinus value we can use as our gradient value for current pixel alpha.
	float a = dot( nc, vec2( 1.0, 0.0 ) );
	float len = length( coord );
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	// to colorize only cutted shape of ring, we have to multiply value of gradient alpha by boolean value that determines if current pixel is solid.
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f * a );
}
