// variable used to receive data from vertex shader.
varying vec2 coord;

void main()
{
	// here we just apply white color to every pixel of our sprite.
	gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
}
