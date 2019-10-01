  uniform sampler2D TheTexture;
  uniform float saturation;
  void main(void)
  {
    vec4 color = texture2D(TheTexture, gl_TexCoord[0].st);
    float gray = dot(color.rgb, vec3(0.3, 0.59, 0.11));


    gl_FragColor = vec4( gray, gray, gray, saturation);
  }
