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
