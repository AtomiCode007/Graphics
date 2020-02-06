//  Per Pixel Lighting shader with texture

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
uniform sampler2D tex;

void main()
{
   vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect(-L,N);
   //  V is the view vector (eye vector)
   vec3 V = normalize(View);

   //  Diffuse light is cosine of light and normal vectors
   float Id = max(dot(L,N) , 0.0);
   //  Specular is cosine of reflected and view vectors
   float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;
   for(int j=0;j<3;j++)
   {
         //  Sum color types
      color = gl_FrontMaterial.emission
               + gl_FrontLightProduct[j].ambient
               + Id*gl_FrontLightProduct[j].diffuse
               + Is*gl_FrontLightProduct[j].specular;
}
      //  Apply texture
      gl_FragColor = color * texture2D(tex,gl_TexCoord[0].xy);
   
   
}
