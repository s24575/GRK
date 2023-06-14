#version 430 core
layout (location = 0) out vec4 color;

in struct Vertex {
    vec2  texcoord;
    vec3  normal;
    vec3  light_dir;
    vec3  view_dir;
    float dist;
} frag_vertex;

uniform struct PointLight{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 attenuation;
}light;


uniform struct Material{
  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
}material;

uniform sampler2D texture_unit;

void main(void){

   float attenuation = 1.0 /(light.attenuation[0] +
     light.attenuation[1] * frag_vertex.dist +
     light.attenuation[2] * frag_vertex.dist * frag_vertex.dist);

  vec3 normal   = normalize(frag_vertex.normal);
  vec3 light_dir = normalize(frag_vertex.light_dir);
  vec3 view_dir  = normalize(frag_vertex.view_dir);


  color = material.ambient * light.ambient;
  float n_dot_l = max(dot(normal, light_dir), 0.0);
  color += material.diffuse * light.diffuse * n_dot_l;
  float r_dot_v_pow = pow(max(dot(reflect(-light_dir, normal), view_dir), 0.0), material.shininess);
  color += material.specular * light.specular * r_dot_v_pow;
  color *= attenuation;
  color += material.emission;
  color *= texture(texture_unit, frag_vertex.texcoord);

}

