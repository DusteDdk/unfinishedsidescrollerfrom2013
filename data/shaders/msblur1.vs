uniform sampler2D MyTex;
void main()
{
	// perform standard transform on vertex
	gl_Position = ftransform();
}
