varying vec2 coord;

void main()
{
	vec2 nc = normalize( coord );
	float dc = dot( nc, vec2( 1.0, 0.0 ) );
	float dn = dot( nc, vec2( 0.0, 1.0 ) );
	float d1 = ( -dc + 1.0 ) * 0.5;
	float n1 = ceil( dn );
	// to complete our progress ring effect, we have to duplicate operation of cutting ring for ring's right side.
	// so we have to make stretched gradient for right side of ring, just changing the sign of gradient face dot-product to positive.
	float d2 = ( dc + 1.0 ) * 0.5;
	// then we have to make boolean value of ring's right side.
	float n2 = ceil( -dn );
	// now we just calculate both left and right sides gradient values and finally add them.
	// and that's all about cutting progress ring shape in shader!
	float a = ( d1 * n1 ) + ( d2 * n2 );
	float len = length( coord );
	float f = ceil( 1.0 - len ) - ceil( 0.7 - len );
	gl_FragColor = vec4( 1.0, 1.0, 1.0, f * a );
}
