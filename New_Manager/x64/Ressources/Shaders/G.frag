

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float radius = 200.0;
    float falloffRadius = 50.0;
    vec2 pos;
    pos.x = 200.0;
    pos.y = 200.0;
   
   float dist = distance(fragCoord,pos);
   
   if(dist < radius)
   {
        vec3 color;
        float t;
        
        color.xyz = vec3(0.0,0.0,0.0);
        if(dist > falloffRadius)
        {
            float fallOff = radius - falloffRadius; 
            t = (dist-falloffRadius)/fallOff;
            color.xyz = mix(vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0),t);
        }
        fragColor = vec4(color,1);
   }
   else fragColor = vec4(1.0,1,1,1);
}