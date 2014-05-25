// variable used to send data to fragment shader.
varying vec2 coord;

void main()
{
	// multiply vertex tex coord by texture matrix so we can use it as texcoord in unit space.
	vec2 texcoord = ( gl_TextureMatrix[ 0 ] * gl_MultiTexCoord0 ).xy;
	// transform texcoord from range <0.0; 1.0> to range <-1.0, 1.0> so our coords center is on sprite center.
	// this will help us to cut circle shapes from sprite.
	coord = texcoord * vec2( 2.0 ) - vec2( 1.0 );
	// multiply vertex position by MVP matrix to get vertex position in screen space.
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
