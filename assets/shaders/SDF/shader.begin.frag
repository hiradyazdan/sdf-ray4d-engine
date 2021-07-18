#version 430 core

layout(binding = 0) uniform UniformBufferObject {
  float u_GlobalTime;
  vec2 u_Resolution;
  vec3 u_Camera;
  vec3 u_CameraUp;
} ubo;

layout(location = 0) in vec2 o_FragCoord;
layout(location = 0) out vec4 o_FragColor;

struct Light {
  vec3 pos;
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;
  float intensity;
};

const float traceprecision = 0.01f;
const Light SunLight = Light(vec3(2.0f, 2.5f, 2.0f), vec3(1.0, 0.8, 0.55), vec3(1.00, 0.90, 0.70), vec3(0.40,0.60,1.00), 1.0);
const Light fillLightA = Light(vec3(-2.0f, 5.5f, -1.0f), vec3(0.78, 0.88, 1.0), vec3(1.00, 0.90, 0.70), vec3(0.40,0.60,1.00), 0.5);
const Light fillLightB = Light(vec3(-1.0f, 5.5f, 2.0f), vec3(1.0, 0.88, 0.78), vec3(1.00, 0.90, 0.70), vec3(0.40,0.60,1.00), 0.5);
const Light fillLightC = Light(vec3(0.0f, -5.5f, 0.0f), vec3(1.0, 0.88, 0.78), vec3(1.00, 0.90, 0.70), vec3(0.40,0.60,1.00), 1.0);
const Light Lights[4] = Light[4](SunLight, fillLightA, fillLightB, fillLightC);

struct TraceResult
{
  vec3 color;
  float t;
  float d;
};

/**
 * Signed distance field functions
 * These function were originally written by Inigo Quilez
 * Modified by Teemu Lindborg where needed.
 * [Accessed Decemeber 2016] Available from: http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
 */

// Sphere
vec4 sdSphere(vec3 p, float s, vec3 color)
{
  return vec4(length(p)-s, color);
}

// Box signed exact
vec4 sdBox(vec3 p, vec3 b, vec3 color)
{
  vec3 d = abs(p) - b;
  return vec4(min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0)), color);
}

// Box fast
vec4 sdFastBox(vec3 _position, float _w, vec3 color)
{
  vec3 pos = abs(_position);
  float dx = pos.x - _w;
  float dy = pos.y - _w;
  float dz = pos.z - _w;
  float m = max(dx, max(dy, dz));
  return vec4(m, color);
}

// Torus - signed - exact
vec4 sdTorus(vec3 p, vec2 t, vec3 color)
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return vec4(length(q)-t.y, color);
}

// Cylinder - signed - exact
vec4 sdCylinder(vec3 p, vec3 c, vec3 color)
{
  return vec4(length(p.xz-c.xy)-c.z, color);
}

//Cone - signed - exact
vec4 sdCone(vec3 p, vec2 c, vec3 color)
{
  // c must be normalized
  float q = length(p.xy);
  return vec4(dot(c,vec2(q,p.z)), color);
}

// Plane - signed - exact
vec4 sdPlane(vec3 p, vec4 n, vec3 color)
{
  // n must be normalized
  return vec4(dot(p,n.xyz) + n.w, color);
}

// Hexagonal Prism - signed - exact
vec4 sdHexPrism(vec3 p, vec2 h, vec3 color)
{
  vec3 q = abs(p);
  return vec4(max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x), color);
}

// Triangular Prism - signed - exact
vec4 sdTriPrism(vec3 p, vec2 h, vec3 color)
{
  vec3 q = abs(p);
  return vec4(max(q.z-h.y,max(q.x*0.866025+p.y*0.5,-p.y)-h.x*0.5), color);
}

// Capsule / Line - signed - exact
vec4 sdCapsule(vec3 p, vec3 a, vec3 b, float r, vec3 color)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return vec4(length( pa - ba*h ) - r, color);
}

// Capped cylinder - signed - exact
vec4 sdCappedCylinder(vec3 p, vec2 h, vec3 color)
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return vec4(min(max(d.x,d.y),0.0) + length(max(d,0.0)), color);
}

// Capped Cone - signed - bound
vec4 sdCappedCone(vec3 p, vec3 c, vec3 color)
{
  vec2 q = vec2( length(p.xz), p.y );
  vec2 v = vec2( c.z*c.y/c.x, -c.z );
  vec2 w = v - q;
  vec2 vv = vec2( dot(v,v), v.x*v.x );
  vec2 qv = vec2( dot(v,w), v.x*w.x );
  vec2 d = max(qv,0.0)*qv/vv;
  return vec4(sqrt( dot(w,w) - max(d.x,d.y) ) * sign(max(q.y*v.x-q.x*v.y,w.y)), color);
}

// Ellipsoid - signed - bound
vec4 sdEllipsoid(vec3 p, vec3 r, vec3 color)
{
  return vec4((length( p/r ) - 1.0) * min(min(r.x,r.y),r.z), color);
}

/*
 * Unsigned distance fields
 */
// Box unsigned exact
vec4 udBox(vec3 p, vec3 b, vec3 color)
{
  return vec4(length(max(abs(p)-b,0.0)), color);
}

// Round Box unsigned exact

vec4 udRoundBox(vec3 p, vec3 b, float r, vec3 color)
{
  return vec4(length(max(abs(p)-b,0.0))-r, color);
}

float g(float a, float b)
{
  return a + b + sqrt(a*a + b*b);
}

float w1(float a, float b, float t)
{
  return g(b, t-1) / (g(a, -t) + g(b, t-1));
}

float w2(float a, float b, float t)
{
  return g(a, -t) / (g(a, -t) + g(b, t-1));
}

vec3 lerp(vec4 a, vec4 b, float t)
{
//  return t*a.yzw + (1-t)*b.yzw;
  return w1(a.x, b.x, t)*a.yzw + w2(a.x, b.x, t)*b.yzw;
}

vec4 opBlend(vec4 a, vec4 b, float k)
{
  float h = clamp(0.5+0.5*(b.x-a.x)/k, 0.0, 1.0);
  float d = mix(b.x, a.x, h) - k*h*(1.0-h);
  return vec4(d, lerp(a, b, h));
}

vec4 opUnion(vec4 a, vec4 b)
{
  return a.x <= b.x ? a : b;
}

vec4 opIntersection(vec4 a, vec4 b)
{
  return a.x >= b.x ? a : b;
}

vec4 opSubtraction(vec4 a, vec4 b)
{
  return -a.x >= b.x ? vec4(-a.x, a.yzw) : b;
}

vec3 opRepetition(vec3 p, vec3 c)
{
  vec3 q = mod(p,c)-0.5*c;
  return q;
}

vec4 map(vec3 _position)
{
  vec4 pos = vec4(4.0, 3.0, 4.0, 0.0);
