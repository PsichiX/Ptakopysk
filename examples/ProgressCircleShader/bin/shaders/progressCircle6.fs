varying vec2 coord;

void main()
{
	vec2 nc = normalize( coord );
	float dc = dot( nc, vec2( 1.0, 0.0 ) );
	// next step to make our ring looks more like progress bar is to cut half of this ring.
	// to make that we have to calculate another dot-product value that determines left side of our ring.
	// so we just have to make dot-product with swapped vector channels inside dot-product operation.
	float dn = dot( nc, vec2( 0.0, 1.0 ) );
	// here we moved stretching operation to separated variable.
	float d1 = ( -dc + 1.0 ) * 0.5;	
	// now we have to calculate boolean value of our ring's left side.
	float n1 = ceil( dn );
	// and finally to cut the left half of ring just multiply stretched gradient alpha by boolean left side of ring.
	float a = d1 * n1;
	float len = length( coord );
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f * a );
}
