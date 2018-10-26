R"(
#version 330 core

in vec4 viewVertex;

in vec2 fragUV;
in vec3 fragPos;
in vec3 fragNormal;

uniform sampler2D dayTexture;
uniform sampler2D nightTexture;
uniform sampler2D specularTexture;

void main()
{

	vec3 norm = normalize(fragNormal);
	vec3 lightDir = vec3(1, 0, 0.5);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 viewDir = normalize(vec3(0, -1, 5) - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = vec3(spec * 0.1);

	vec4 day = texture(dayTexture, fragUV);
	vec4 night = (texture(nightTexture, fragUV) * vec4(1/min(1.0, diff + 0.1)));
	vec4 blend = mix(night, day, diff);

	vec4 baseColour = blend;
	gl_FragColor = baseColour * (vec4(diff) + vec4(0.1f) + (vec4(specular, 1.0) * texture(specularTexture, fragUV)));
}
)"