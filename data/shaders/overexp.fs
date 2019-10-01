  uniform sampler2D TheTexture;
  uniform float exp;
  void main(void)
  {
    vec4 color = texture2D(TheTexture, gl_TexCoord[0].st);

    gl_FragColor = vec4( color.rgb.r+exp,color.rgb.g+exp, color.rgb.b+exp, 1);
  }
