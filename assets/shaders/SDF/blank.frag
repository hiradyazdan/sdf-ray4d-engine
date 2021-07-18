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
  return pos;
}

// Ray for tracing
mat2x3 createRay(vec3 _origin, vec3 _lookAt, vec3 _upV, vec2 _uv, float _fov, float _aspect)
{
  mat2x3 ray;
  vec3 direction, rayUpV, rightV;
  vec2 uv;
  float fieldOfViewRad;

  ray[0] = _origin;
  direction = normalize(_lookAt - _origin);
  rayUpV = normalize( _upV - direction * dot(direction, _upV));
  rightV = cross(direction, rayUpV);
  uv = _uv * 2 - vec2(1.);
  fieldOfViewRad = _fov * 3.1415 / 180;

  ray[1] = direction + tan(fieldOfViewRad / 2.f) * rightV * uv.x + tan(fieldOfViewRad / 2.f) / _aspect * rayUpV * uv.y;
  ray[1] = normalize(ray[1]);

  return ray;
}

TraceResult castRay(mat2x3 _ray)
{
  TraceResult trace;
  trace.t = 1.f;
  float tmax = 20.f;
  for(int i = 0; i < 64; ++i)
  {
    vec4 r = map(_ray[0] + trace.t * _ray[1]);
    trace.d = r.x;
    trace.color = r.yzw;
    if(trace.d <= traceprecision || trace.t > tmax) {
      break;
    }
    trace.t += trace.d;
  }

  return trace;
}

vec3 calcNormal(vec3 _position)
{
  vec3 offset = vec3(0.0005, -0.0005, 1.0);
  vec3 normal = normalize(offset.xyy*map( _position + offset.xyy ).x +
                          offset.yyx*map( _position + offset.yyx ).x +
                          offset.yxy*map( _position + offset.yxy ).x +
                          offset.xxx*map( _position + offset.xxx ).x);
  return normalize(normal);
}

/**
 * Ambient occlusion is based on a shadertoy example by Inigo Quilez
 * [Accessed January 2017] Available from: https://www.shadertoy.com/view/Xds3zN
 */
float calcAO(vec3 _position, vec3 _normal)
{
  float occ = 0.0;
  float sca = 1.0;
  for(int i = 0; i < 5; i++)
  {
    float hr = 0.01 + 0.12*float(i)/4.0;
    vec3 aopos = _normal * hr + _position;
    float dd = map(aopos).x;
    occ += -(dd-hr)*sca;
    sca *= 0.95;
  }
  return clamp(1.0 - 3.0*occ, 0.0, 1.0);
}

/**
 * The sky rendering is based on a shadertoy example by Inigo Quilez
 * [Accessed January 2017] Available from: https://www.shadertoy.com/view/4ttSWf
 */
vec3 renderSky(mat2x3 _ray)
{
  // background sky
  vec3 col = 0.9*vec3(0.4, 0.65, 1.0) - _ray[1].y*vec3(0.4, 0.36, 0.4);

  // sun glare
  float sun = clamp(dot(normalize(SunLight.pos), _ray[1]), 0.0, 1.0);
  col += 0.6*vec3(1.0, 0.6, 0.3)*pow(sun, 32.0);

  return col;
}

vec3 applyFog(vec3 color, float distance)
{
  float fogAmount = 1.0 - exp(-distance*2);
  vec3 fogColor = vec3(0.4, 0.65, 1.0);
  return mix(color, fogColor, fogAmount);
}

float softshadow(vec3 ro, vec3 rd, float mint, float tmax)
{
  float res = 1.0;
  float t = mint;
  for(int i = 0; i < 16; ++i)
  {
    float h = map(ro + normalize(rd)*t).x;
    res = min(res, 8.0*h/t);
    t += clamp(h, 0.02, 0.10);
    if(h < traceprecision || t > tmax)
      break;
  }
  return clamp(res, 0.0, 1.0);
}

// Rendering function
vec3 render(mat2x3 _ray)
{
  TraceResult trace = castRay(_ray);
  vec3 col = vec3(0.0);//renderSky(_ray);

  if(trace.d <= traceprecision)
  {
    vec3 p = _ray[0] + trace.t * _ray[1];
    vec3 n = calcNormal(p);
    vec3 reflection = reflect(_ray[1], n);
    float intensitySum = 0.f;
    col = vec3(0.0);

    for(int i = 0; i < 4; ++i) {

      vec3 lightDir = normalize(Lights[i].pos - p);

      float diffuse = clamp(dot(n, lightDir), 0.0, 1.0) * softshadow(p, Lights[i].pos, 0.02, 2.5);
      float ambient = clamp(0.5 + 0.5*n.y, 0.0, 1.0);
      float occlusion = calcAO(p, n);

      float specular = pow(clamp(dot(reflection, lightDir), 0.0, 1.0 ), 16.0);

      vec3 acc = vec3(0.0);
      acc += 1.40 * diffuse * Lights[i].diffuse;
      acc += 1.20 * ambient * Lights[i].ambient * occlusion;
      if(i == 0)
        acc += 2.00 * specular * Lights[i].specular * diffuse;

      col += trace.color * acc * Lights[i].intensity;
      intensitySum += Lights[i].intensity;
    }
    col /= intensitySum;
    col = applyFog(col, trace.t/150.f);

    // Vigneting
    vec2 q = o_FragCoord.xy / ubo.u_Resolution.xy;
    col *= 0.5 + 0.5*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.25 );
  }
  return vec3( clamp(col, 0.0, 1.0) );
}

void main()
{
  vec3 cameraPosition = ubo.u_Camera;
  vec3 upVector = ubo.u_CameraUp;
  vec3 lookAt = vec3(0.f);
  float aspectRatio = ubo.u_Resolution.x / ubo.u_Resolution.y;

  mat2x3 ray = createRay(cameraPosition, lookAt, upVector, o_FragCoord, 90.f, aspectRatio);
  vec3 color = render(ray);

  // Gamma correction
  o_FragColor = vec4(pow(color, vec3(0.4545)), 1.f);
}
