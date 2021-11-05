#version 450

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


layout(set = 0, binding = 0) uniform PerFrameData {
    mat4 projection;
    mat4 view;
    vec3 cameraPosition;
    vec3 lightingPosition;
    Material material;
    Light light;
} frame;

layout(set = 1, binding = 0) uniform sampler2D textureSampler;
layout(set = 1, binding = 1) uniform sampler2D emissionSampler;

layout(location = 0) in vec3 in_fragPosition;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_fragTextureCoordinate;
layout(location = 3) in vec3 in_lightingPosition;

layout(location = 0) out vec4 out_color;

void main() {
    // const vec3 LightColor = vec3(1.0, 0.196, 0.0); // Engine exhaust color
    // const vec3 LightColor = vec3(1.0);

    // Ambient
    const float AmbientStrength = 0.05;
    vec3 Ambient = /* AmbientStrength * */ frame.light.ambient * frame.material.ambient;

    // Diffuse
    vec3 lightPositionRelativeToFragment = vec3(frame.view * vec4(frame.light.position, 1.0)) - in_fragPosition;
    vec3 lightNormal = normalize(lightPositionRelativeToFragment);
    vec3 surfaceNormal = normalize(in_normal);
    float diffuseStrength = max(dot(lightNormal, surfaceNormal), 0.0);
    vec3 diffuse = frame.light.diffuse * diffuseStrength * frame.material.diffuse;

    // Specular
    const float SpecularStrength = 0.5;
    vec3 reflection = reflect(-lightNormal, surfaceNormal);
    vec3 viewDirection = normalize(-in_fragPosition);
    float specularAngleStrength = max(dot(viewDirection, reflection), 0.0);
    float specularity = pow(specularAngleStrength, frame.material.shininess);
    vec3 specular = /* SpecularStrength * */ frame.light.specular * specularity * frame.material.specular;

    // Emission
    vec3 emission = texture(emissionSampler, in_fragTextureCoordinate).rgb;

    // Final color
    vec3 color = Ambient + diffuse + specular + emission;
    vec4 objectColor = texture(textureSampler, in_fragTextureCoordinate);

    out_color = objectColor * vec4(color, 1.0);
}
