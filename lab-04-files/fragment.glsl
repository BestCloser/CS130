varying vec4 position;
varying vec3 normal;
varying vec3 light_direction;

void main()
{
    vec4 normal4 = vec4(normal[0], normal[1], normal[2], 0);

    vec4 finalColor = gl_LightModel.ambient;

    for (int i = 0; i < gl_MaxLights; i++)
    {
        vec4 l_ray = vec4(light_direction[0], light_direction[1], light_direction[2], 1);
        // replacing last component from 0 to 1 fixes specular component displaying Specular Monkey
        vec4 e_ray = normalize(-position);
        vec4 r_ray = normalize(2.0 * dot(l_ray, normal4) * normal4 - l_ray);

        vec4 ambient = gl_LightSource[i].ambient * gl_FrontMaterial.ambient;
        // not including gl_LightModel.ambient because green becomes darker and doesn't match the test

        vec4 diffuse = gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * max(dot(normalize(l_ray), normal4), 0.0);

	    vec4 specular = gl_LightSource[i].specular * gl_FrontMaterial.specular * pow(max(dot(r_ray, e_ray), 0.0), gl_FrontMaterial.shininess);

	    finalColor += ambient + diffuse + specular;
    }

    gl_FragColor = finalColor;
}
