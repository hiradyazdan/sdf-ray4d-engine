#version 440

layout(location = 0) in vec3 vECVertNormal;
layout(location = 1) in vec3 vECVertPos;
//layout(location = 2) flat in vec3 vDiffuseAdjust;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out float depthColor;

layout(std140, binding = 1) uniform buf {
  vec3 ECCameraPosition;
  vec3 ka;
  vec3 kd;
  vec3 ks;
// Have one light only for now.
  vec3 ECLightPosition;
  vec3 attenuation;
  vec3 color;
  float intensity;
  float specularExp;

  float nearPlane;
  float farPlane;
} ubuf;

float LinearizeDepth(float depth)
{
  float near = ubuf.nearPlane;
  float far = ubuf.farPlane;
  float z = depth * 2.0 - 1.0; // NDC (Normalized Device Coordinates)

  return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
  vec3 unnormL = ubuf.ECLightPosition - vECVertPos;
  float dist = length(unnormL);
  float att = 1.0 / (ubuf.attenuation.x + ubuf.attenuation.y * dist + ubuf.attenuation.z * dist * dist);

  vec3 N = normalize(vECVertNormal);
  vec3 L = normalize(unnormL);
  float NL = max(0.0, dot(N, L));
  vec3 dColor = att * ubuf.intensity * ubuf.color * NL;

  vec3 R = reflect(-L, N);
  vec3 V = normalize(ubuf.ECCameraPosition - vECVertPos);
  float RV = max(0.0, dot(R, V));
  vec3 sColor = att * ubuf.intensity * ubuf.color * pow(RV, ubuf.specularExp);

  fragColor = vec4(ubuf.ka + (ubuf.kd/* + vDiffuseAdjust*/) * dColor + ubuf.ks * sColor, 1.0);
  depthColor = LinearizeDepth(gl_FragCoord.z);
}
