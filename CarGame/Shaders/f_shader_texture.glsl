#version 330

in vec4 fs_color;
in vec2 fs_uv;
in vec3 fs_normal;
in vec3 fs_eye_dir;

uniform int shading_mode = 1;

layout (location = 0) out vec4 color;

uniform sampler2D tex0;
uniform bool flag_texture;

uniform int light_mode;

uniform bool inverse;

uniform bool isGround;

uniform bool isGround_shiness;

uniform vec3 light_position;
uniform vec3 light_dir;
uniform float light_cos_cutoff;
uniform vec3 I_l;       // light intensity
float I_a = 1.0;				  // ambient light intensity

uniform float ambient_factor = 0.1f;
uniform float shininess_n;
//uniform vec3 K_s;

void main()
{
	vec3 K_a = ambient_factor * vec3(1.f,1.f,1.f);
	if(light_mode==1)
		K_a = ambient_factor * vec3(0.3f,0.3f,0.3f);
	vec3 K_d = vec3(0.2f,0.2f,0.2f);
	vec3 K_s = vec3(0.2f,0.2f,0.2f);

	vec3 L = normalize(light_position-fs_eye_dir);
	vec3 N = normalize(fs_normal);
	vec3 V = normalize(-fs_eye_dir);
	vec3 H = (L+V)/length(L+V);
	vec3 Sd = normalize(-light_dir);
		
	float d = length(light_position-fs_eye_dir);

	vec3 ambient = I_a * K_a;
	vec3 diffuse = I_l * K_d * max(0.f, dot(L, N));
	vec3 specular = I_l * K_s * pow(max(0.f, dot(N, H)), shininess_n);

	float darkside_s_d = 0.0f;
	if ( dot(Sd,L) < light_cos_cutoff){
		if(light_mode==1){
		specular *= vec3(darkside_s_d,darkside_s_d,darkside_s_d);
		diffuse *= vec3(darkside_s_d,darkside_s_d,darkside_s_d);
		}
	}

	if(isGround){
		if(!isGround_shiness)
			specular *= vec3(0, 0, 0);
	}

	if (flag_texture)
		color = vec4(ambient + diffuse + specular, 1.f) * texture(tex0, fs_uv);
	else 
		color = vec4(ambient + diffuse + specular, 1.f) * fs_color;

	//color.rgb += ambient + diffuse + specular;
	if(inverse)
		color.rgb = (vec3(1.f, 1.f, 1.f), vec3(1.f,1.f,1.f));
	else
		color.rgb += (vec3(1.f, 1.f, 1.f), color.rgb);
	color.a = fs_color.a;
}