varying vec2 coord;

void main()
{
	vec2 nc = normalize( coord );
	// now just try to swap channels of second vector in dot-product operation to see how to rotate face of our gradient.
	float a = dot( nc, vec2( 0.0, 1.0 ) );
	float len = length( coord );
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f * a );
}
