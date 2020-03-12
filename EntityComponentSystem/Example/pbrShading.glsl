struct DirectionalLight {
  vec3 direction;  // Direction of the light
  vec3 colour;     // Colour of the light
  float intensity; // Intensity of the light in lux
};

struct PointLight {
  vec3 position;   // The position of the light
  float radius;    // Radius of the light in meter
  vec3 colour;     // Colour of the light
  float intensity; // Intensity of the light in candela
};

struct Material {
  vec3 baseColour;
  float metalness;
  float roughness;
};

// TODO: put in math.glsl
const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 f0) {
  return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

float normalDistributionGGX(vec3 normal, vec3 halfwayVector, float roughness) {
  float roughnessSquared = roughness * roughness;
  float nDotH = max(dot(normal, halfwayVector), 0.0);
  float denom = (nDotH * nDotH * (roughnessSquared - 1.0) + 1.0);
  denom = PI * denom * denom;

  return roughnessSquared / denom;
}

float geometrySchlickGGX(vec3 normal, vec3 v, float roughness) {
    float nDotV = max(dot(normal, v), 0.0);

    float r = roughness + 1.0;
    float k = r * r * 0.125;
    float denom = nDotV * (1.0 - k) + k;

    return nDotV / denom;
}

float geometrySmith(vec3 normal, vec3 toCameraDirection, vec3 toLightDirection, float roughness) {
    return geometrySchlickGGX(normal, toCameraDirection, roughness) * geometrySchlickGGX(normal, toLightDirection, roughness);
}

vec3 GGX(Material material, vec3 toCameraDirection, vec3 toLightDirection, vec3 position, vec3 normal) {
  vec3 baseReflectivity = mix(vec3(0.04), material.baseColour, material.metalness);

  vec3 halfwayVector = normalize(toCameraDirection + toLightDirection);

  float normalDistribution = normalDistributionGGX(normal, halfwayVector, material.roughness);
  float geometry = geometrySmith(normal, toCameraDirection, toLightDirection, material.roughness);
  vec3 fresnel = fresnelSchlick(max(dot(normal, toCameraDirection), 0.0), baseReflectivity);

  vec3 specular = normalDistribution * geometry * fresnel / (4.0 * max(dot(normal, toCameraDirection), 0.001) * max(dot(normal, toLightDirection), 0.001));

  vec3 refractionEnergy = vec3(1.0) - fresnel * material.metalness;

  return (refractionEnergy * material.baseColour / PI + specular) * max(dot(normal, toLightDirection), 0.0);
}

vec3 calcLight(DirectionalLight light, Material material, vec3 position, vec3 normal, vec3 toCamera) {
  vec3 toCameraDirection = normalize(toCamera);
  vec3 toLightDirection = normalize(-light.direction);

  return light.intensity * light.colour * GGX(material, toCameraDirection, toLightDirection, position, normal);
}

vec3 calcLight(PointLight light, Material material, vec3 position, vec3 normal, vec3 toCamera) {
  vec3 toLight = light.position - position;
  vec3 toCameraDirection = normalize(toCamera);
  vec3 toLightDirection = normalize(toLight);
  float falloff = 1.0 / (4.0 * PI * dot(toLight, toLight));

  return light.intensity * light.colour * falloff * GGX(material, toCameraDirection, toLightDirection, position, normal);
}
