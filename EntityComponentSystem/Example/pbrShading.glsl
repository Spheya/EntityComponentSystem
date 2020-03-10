struct DirectionalLight {
  vec3 direction;  // Direction of the light
  vec3 colour;     // Colour of the light
  float intensity; // Intensity of the light in lux
};

struct SphericalLight {
  vec3 position;   // The position of the light
  float radius;    // Radius of the light in meter
  vec3 colour;     // Colour of the light
  float intensity; // Intensity of the light in candela
};

float specularFunction(float factor, float roughness, float lightSize) {
  float e = 2.71828182846;

  float value = 1.0;

  if(roughness < lightSize) {
    if(factor > lightSize - roughness) {
      float scale = (factor - (lightSize - roughness)) / roughness;
      value = pow(e, -0.5 * scale * scale);
    }
  } else {
    float scale = factor / roughness;
    value = pow(e, -0.5 * scale * scale) * lightSize / roughness;
  }

  return value;
}

float schlick(float eta1, float eta2, float cosTheta) {
  float a = (eta1 - eta2) / (eta1 + eta2);
  a *= a;
  return a + (1 - a) * pow(1 - cosTheta, 5.0);
}

vec3 calcLight(DirectionalLight light, vec3 baseColour, vec3 position, vec3 normal, vec3 toCamera) {
  vec3 normalizedLightDirection = normalize(light.direction);

  float diffuse = clamp(dot(normal, -normalizedLightDirection), 0.0, 1.0);

  // Directional lights don't have falloff

  return baseColour * light.colour * light.intensity * diffuse;
}

vec3 calcLight(SphericalLight light, vec3 baseColour, vec3 position, vec3 normal, vec3 toCamera) {
  vec3 toLightVector = light.position - position;
  vec3 toLightDirection = normalize(toLightVector);
  float dist = max(length(toLightVector) - light.radius, 0.0);

  float reflectionAmount = schlick(1.0, 2.0, -dot(normalize(toCamera), normal));
  float refractionAmount = 1.0 - reflectionAmount;

  vec3 reflectionDirection = reflect(normalize(toCamera), normal);

  vec3 closestPoint = position + reflectionDirection * max(dot(reflectionDirection, light.position - position), 0.0);
  vec3 lightToClosestPoint = closestPoint - light.position;

  float functionFactor = max(length(lightToClosestPoint) - light.radius, 0.0);

  float specular = specularFunction(functionFactor, 0.25, light.radius);
  float diffuse = clamp(dot(normal, toLightDirection), 0.1, 1.0);
  float falloff = 1.0 / (4.0 * 3.1415926 * dist * dist);

  return falloff * light.intensity * (refractionAmount * baseColour * light.colour * diffuse + reflectionAmount * light.colour * specular);
}
