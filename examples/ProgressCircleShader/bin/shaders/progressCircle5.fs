varying vec2 coord;

void main()
{
	vec2 nc = normalize( coord );
	// ok, now the hardest part of painting gradient. first we swapping back channels of vector inside dot-product operation.
	float dc = dot( nc, vec2( 1.0, 0.0 ) );
	// now we have to change side of our gradient face and stretch it on whole ring.
	// to make that first we have to inverse dot-product result, add 1.0 then divide by 2.
	// ofc. because dividing have big cost on GPU, we have to multiply by 0.5.
	// this is the same transforming operation as we used in vertex shader on texture coord.
	float a = ( -dc + 1.0 ) * 0.5;
	float len = length( coord );
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f * a );
}
