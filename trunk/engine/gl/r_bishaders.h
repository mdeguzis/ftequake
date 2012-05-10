/*
WARNING: THIS FILE IS GENERATED BY 'generatebuiltinsl.c'.
YOU SHOULD NOT EDIT THIS FILE BY HAND
*/

#ifdef GLQUAKE
{QR_OPENGL, 110, "bloom_blur",
//apply gaussian filter

"varying vec2 tc;\n"

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
/*offset should be 1.2 pixels away from the center*/
"uniform vec3 e_glowmod;\n"
"uniform sampler2D s_t0;\n"
"void main ()\n"
"{\n"
"gl_FragColor =\n"
"0.3125 * texture2D(s_t0, tc - e_glowmod.st) +\n"
"0.375 * texture2D(s_t0, tc) +\n"
"0.3125 * texture2D(s_t0, tc + e_glowmod.st);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "bloom_filter",
//the bloom filter
//filter out any texels which are not to bloom

"varying vec2 tc;\n"

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"void main ()\n"
"{\n"
"gl_FragColor = (texture2D(s_t0, tc) - vec4(0.5, 0.5, 0.5, 0.0)) * vec4(2.0,2.0,2.0,1.0);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "bloom_final",
//add them together
//optionally apply tonemapping

"varying vec2 tc;\n"

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform sampler2D s_t1;\n"
"uniform sampler2D s_t2;\n"
"uniform sampler2D s_t3;\n"
"void main ()\n"
"{\n"
"gl_FragColor = \n"
"texture2D(s_t0, tc) +\n"
"texture2D(s_t1, tc) +\n"
"texture2D(s_t2, tc) +\n"
"texture2D(s_t3, tc) ;\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "colourtint",
//this glsl shader is useful for cubemapped post processing effects (see csaddon for an example)
"varying vec4 tf;\n"
"#ifdef VERTEX_SHADER\n"
"void main ()\n"
"{\n"
"gl_Position = tf = vec4(v_position.xy,-1.0, 1.0);\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform sampler3D s_t1;\n"
"void main()\n"
"{\n"
"vec2 fc;\n"
"fc = tf.xy / tf.w;\n"
"vec3 raw = texture2D(s_t0, (1.0 + fc) / 2.0).rgb;\n"
"#define LUTSIZE 16.0\n"
"vec3 scale = vec3((LUTSIZE-1.0)/LUTSIZE);\n"
"vec3 bias = vec3(1.0/(2.0*LUTSIZE));\n"
"gl_FragColor = texture3D(s_t1, raw * scale + bias);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "crepuscular_opaque",
//opaque surfaces are drawn to the render target to mask out skies
"#ifdef VERTEX_SHADER\n"
"void main ()\n"
"{\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"void main()\n"
"{\n"
"gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "crepuscular_rays",
"!!cvarf crep_decay\n"
"!!cvarf crep_density\n"
"!!cvarf crep_weight\n"

//this is a post-processing shader, drawn in 2d
//there will be a render target containing sky surfaces drawn with crepuscular_sky, and everything else drawn with crepuscular_opaque (to mask out the sky)
//this shader then just smudges the sky out a bit as though its coming from the sun or whatever through the clouds.
//yoinked from http://fabiensanglard.net/lightScattering/index.php

"varying vec2 tc;\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"gl_Position = v_position;\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"const float crep_decay = 0.94;\n"
"const float crep_density = 0.5;\n"
"const float crep_weight = 0.2;\n"
"uniform vec3 l_lightcolour;\n"
"uniform vec3 l_lightscreen;\n"
"uniform sampler2D s_t0;\n"
"const int NUM_SAMPLES = 100;\n"
"void main()\n"
"{\n"
"vec2 deltaTextCoord = vec2(tc.st - l_lightscreen.xy);\n"
"vec2 textCoo = tc.st;\n"
"deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * crep_density;\n"
"float illuminationDecay = 1.0;\n"
"for(int i=0; i < NUM_SAMPLES ; i++)\n"
"{\n"
"textCoo -= deltaTextCoord;\n"
"vec4 sample = texture2D(s_t0, textCoo);\n"
"sample *= illuminationDecay * crep_weight;\n"
"gl_FragColor += sample;\n"
"illuminationDecay *= crep_decay;\n"
"}\n"
"gl_FragColor *= vec4(l_lightcolour, 1.0);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "crepuscular_sky",
//pretty much a regular sky shader
//though in reality we should render a sun circle in the middle.
//still, its kinda cool to have scrolling clouds masking out parts of the sun.

"#ifdef VERTEX_SHADER\n"
"varying vec3 pos;\n"
"void main ()\n"
"{\n"
"pos = v_position.xyz;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform float e_time;\n"
"uniform vec3 e_eyepos;\n"
"varying vec3 pos;\n"
"uniform sampler2D s_t0;\n"
"uniform sampler2D s_t1;\n"
"void main ()\n"
"{\n"
"vec2 tccoord;\n"
"vec3 dir = pos - e_eyepos;\n"
"dir.z *= 3.0;\n"
"dir.xy /= 0.5*length(dir);\n"
"tccoord = (dir.xy + e_time*0.03125);\n"
"vec3 solid = vec3(texture2D(s_t0, tccoord));\n"
"tccoord = (dir.xy + e_time*0.0625);\n"
"vec4 clouds = texture2D(s_t1, tccoord);\n"
"gl_FragColor.rgb = (solid.rgb*(1.0-clouds.a)) + (clouds.a*clouds.rgb);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "default2d",
//this shader is present for support for gles/gl3core contexts
//it is single-texture-with-vertex-colours, and doesn't do anything special.
//beware that a few things use this, including apparently fonts and bloom rescaling.
//its really not meant to do anything special.

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"attribute vec4 v_colour;\n"
"varying vec2 tc;\n"
"varying vec4 vc;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"vc = v_colour;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"in vec2 tc;\n"
"varying vec4 vc;\n"
"void main ()\n"
"{\n"
"gl_FragColor = texture2D(s_t0, tc) * vc;\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultadditivesprite",
"!!permu FOG\n"

//meant to be used for additive stuff. presumably particles and sprites. though actually its only flashblend effects that use this at the time of writing.
//includes fog, apparently.

"#include \"sys/fog.h\"\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"attribute vec4 v_colour;\n"
"varying vec2 tc;\n"
"varying vec4 vc;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"vc = v_colour;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"varying vec2 tc;\n"
"varying vec4 vc;\n"
"uniform vec4 e_colourident;\n"
"void main ()\n"
"{\n"
"gl_FragColor = fog4additive(texture2D(s_t0, tc) * vc * e_colourident);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultskin",
"!!permu FULLBRIGHT\n"
"!!permu UPPERLOWER\n"
"!!permu FRAMEBLEND\n"
"!!permu SKELETAL\n"
"!!permu FOG\n"

//standard shader used for models.
//must support skeletal and 2-way vertex blending or Bad Things Will Happen.
//the vertex shader is responsible for calculating lighting values.

"varying vec2 tc;\n"
"varying vec3 light;\n"
"#ifdef VERTEX_SHADER\n"
"#include \"sys/skeletal.h\"\n"
"attribute vec2 v_texcoord;\n"
"uniform vec3 e_light_dir;\n"
"uniform vec3 e_light_mul;\n"
"uniform vec3 e_light_ambient;\n"
"void main ()\n"
"{\n"
"vec3 n;\n"
"gl_Position = skeletaltransform_n(n);\n"
"light = e_light_ambient + (dot(n,e_light_dir)*e_light_mul);\n"
"tc = v_texcoord;\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"#include \"sys/fog.h\"\n"
"uniform sampler2D s_t0;\n"
"#ifdef LOWER\n"
"uniform sampler2D s_t1;\n"
"uniform vec3 e_lowercolour;\n"
"#endif\n"
"#ifdef UPPER\n"
"uniform sampler2D s_t2;\n"
"uniform vec3 e_uppercolour;\n"
"#endif\n"
"#ifdef FULLBRIGHT\n"
"uniform sampler2D s_t3;\n"
"#endif\n"
"uniform vec4 e_colourident;\n"
"void main ()\n"
"{\n"
"vec4 col, sp;\n"
"col = texture2D(s_t0, tc);\n"
"#ifdef UPPER\n"
"vec4 uc = texture2D(s_t2, tc);\n"
"col.rgb = mix(col.rgb, uc.rgb*e_uppercolour, uc.a);\n"
"#endif\n"
"#ifdef LOWER\n"
"vec4 lc = texture2D(s_t1, tc);\n"
"col.rgb = mix(col.rgb, lc.rgb*e_lowercolour, lc.a);\n"
"#endif\n"
"col.rgb *= light;\n"
"#ifdef FULLBRIGHT\n"
"vec4 fb = texture2D(s_t3, tc);\n"
"col.rgb = mix(col.rgb, fb.rgb, fb.a);\n"
"#endif\n"
"gl_FragColor = fog4(col * e_colourident);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultsky",
//regular sky shader for scrolling q1 skies
//the sky surfaces are thrown through this as-is.

"#ifdef VERTEX_SHADER\n"
"varying vec3 pos;\n"
"void main ()\n"
"{\n"
"pos = v_position.xyz;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform float e_time;\n"
"uniform vec3 e_eyepos;\n"
"varying vec3 pos;\n"
"uniform sampler2D s_t0;\n"
"uniform sampler2D s_t1;\n"
"void main ()\n"
"{\n"
"vec2 tccoord;\n"
"vec3 dir = pos - e_eyepos;\n"
"dir.z *= 3.0;\n"
"dir.xy /= 0.5*length(dir);\n"
"tccoord = (dir.xy + e_time*0.03125);\n"
"vec3 solid = vec3(texture2D(s_t0, tccoord));\n"
"tccoord = (dir.xy + e_time*0.0625);\n"
"vec4 clouds = texture2D(s_t1, tccoord);\n"
"gl_FragColor.rgb = (solid.rgb*(1.0-clouds.a)) + (clouds.a*clouds.rgb);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultsprite",
"!!permu FOG\n"
//used by both particles and sprites.
//note the fog blending mode is all that differs from defaultadditivesprite

"#include \"sys/fog.h\"\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"attribute vec4 v_colour;\n"
"varying vec2 tc;\n"
"varying vec4 vc;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"vc = v_colour;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"varying vec2 tc;\n"
"varying vec4 vc;\n"
"uniform vec4 e_colourident;\n"
"void main ()\n"
"{\n"
"gl_FragColor = fog4blend(texture2D(s_t0, tc) * vc * e_colourident);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultwall",
"!!permu OFFSETMAPPING\n"
"!!permu FULLBRIGHT\n"
"!!permu FOG\n"
"!!cvarf r_glsl_offsetmapping_scale\n"

//this is what normally draws all of your walls, even with rtlights disabled
//note that the '286' preset uses drawflat_walls instead.

"#include \"sys/fog.h\"\n"
"#if defined(OFFSETMAPPING)\n"
"varying vec3 eyevector;\n"
"#endif\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"attribute vec2 v_lmcoord;\n"
"varying vec2 tc, lm;\n"
"#if defined(OFFSETMAPPING)\n"
"uniform vec3 e_eyepos;\n"
"attribute vec3 v_normal;\n"
"attribute vec3 v_svector;\n"
"attribute vec3 v_tvector;\n"
"#endif\n"
"void main ()\n"
"{\n"
"#if defined(OFFSETMAPPING)\n"
"vec3 eyeminusvertex = e_eyepos - v_position.xyz;\n"
"eyevector.x = dot(eyeminusvertex, v_svector.xyz);\n"
"eyevector.y = -dot(eyeminusvertex, v_tvector.xyz);\n"
"eyevector.z = dot(eyeminusvertex, v_normal.xyz);\n"
"#endif\n"
"tc = v_texcoord;\n"
"lm = v_lmcoord;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform sampler2D s_t1;\n"
"#ifdef OFFSETMAPPING\n"
"uniform sampler2D s_t2;\n"
"#endif\n"
"#ifdef FULLBRIGHT\n"
"uniform sampler2D s_t4;\n"
"#endif\n"
"varying vec2 tc, lm;\n"
"uniform vec4 e_lmscale;\n"
"uniform vec4 e_colourident;\n"
"#ifdef OFFSETMAPPING\n"
"#include \"sys/offsetmapping.h\"\n"
"#endif\n"
"void main ()\n"
"{\n"
"#ifdef OFFSETMAPPING\n"
"vec2 tcoffsetmap = offsetmap(s_t2, tc, eyevector);\n"
"#define tc tcoffsetmap\n"
"#endif\n"
"gl_FragColor = texture2D(s_t0, tc) * texture2D(s_t1, lm) * e_lmscale;\n"
"#ifdef FULLBRIGHT\n"
"gl_FragColor.rgb += texture2D(s_t4, tc).rgb;\n"
"#endif\n"
"gl_FragColor = gl_FragColor * e_colourident;\n"
"#ifdef FOG\n"
"gl_FragColor = fog4(gl_FragColor);\n"
"#endif\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "defaultwarp",
"!!cvarf r_wateralpha\n"
"!!permu FOG\n"

//this is the shader that's responsible for drawing default q1 turbulant water surfaces
//this is expected to be moderately fast.

"#include \"sys/fog.h\"\n"
"varying vec2 tc;\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord.st;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform float e_time;\n"
"uniform float cvar_r_wateralpha;\n"
"void main ()\n"
"{\n"
"vec2 ntc;\n"
"ntc.s = tc.s + sin(tc.t+e_time)*0.125;\n"
"ntc.t = tc.t + sin(tc.s+e_time)*0.125;\n"
"vec3 ts = vec3(texture2D(s_t0, ntc));\n"
"gl_FragColor = fog4(vec4(ts, cvar_r_wateralpha));\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "drawflat_wall",
"!!cvarv r_floorcolor\n"
"!!cvarv r_wallcolor\n"
"!!permu FOG\n"

//this is for the '286' preset walls, and just draws lightmaps coloured based upon surface normals.

"#include \"sys/fog.h\"\n"
"varying vec4 col;\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec3 v_normal;\n"
"attribute vec2 v_lmcoord;\n"
"varying vec2 lm;\n"
"uniform vec3 cvar_r_wallcolor;\n"
"uniform vec3 cvar_r_floorcolor;\n"
"uniform vec4 e_lmscale;\n"
"void main ()\n"
"{\n"
"col = vec4(e_lmscale.rgb/255.0 * ((v_normal.z < 0.73)?cvar_r_wallcolor:cvar_r_floorcolor), e_lmscale.a);\n"
"lm = v_lmcoord;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"varying vec2 lm;\n"
"void main ()\n"
"{\n"
"gl_FragColor = fog4(col * texture2D(s_t0, lm));\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "lpp_depthnorm",
"!!permu BUMP\n"
"!!permu SKELETAL\n"

//light pre-pass rendering (defered lighting)
//this is the initial pass, that draws the surface normals and depth to the initial colour buffer

"varying vec3 norm, tang, bitang;\n"
"#if defined(BUMP)\n"
"varying vec2 tc;\n"
"#endif\n"
"#ifdef VERTEX_SHADER\n"
"#include \"sys/skeletal.h\"\n"
"attribute vec2 v_texcoord;\n"
"void main()\n"
"{\n"
"#if defined(BUMP)\n"
"gl_Position = skeletaltransform_nst(norm, tang, bitang);\n"
"tc = v_texcoord;\n"
"#else\n"
"gl_Position = skeletaltransform_n(norm);\n"
"#endif\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"#if defined(BUMP)\n"
"uniform sampler2D s_t0;\n"
"#endif\n"
"void main()\n"
"{\n"
"vec3 onorm;\n"
"#if defined(BUMP)\n"
"vec3 bm = 2.0*texture2D(s_t0, tc).xyz - 1.0;\n"
"onorm = normalize(bm.x * tang + bm.y * bitang + bm.z * norm);\n"
"#else\n"
"onorm = norm;\n"
"#endif\n"
"gl_FragColor = vec4(onorm.xyz, gl_FragCoord.z / gl_FragCoord.w);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "lpp_light",
//this shader is a light shader. ideally drawn with a quad covering the entire region
//the output is contribution from this light (which will be additively blended)
//you can blame Electro for much of the maths in here.
//fixme: no fog

"varying vec4 tf;\n"
"#ifdef VERTEX_SHADER\n"
"void main()\n"
"{\n"
"tf = ftetransform();\n"
"gl_Position = tf;\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform vec3 l_lightposition;\n"
"uniform mat4 m_invviewprojection;\n"
"uniform vec3 l_lightcolour;\n"
"uniform float l_lightradius;\n"
"vec3 calcLightWorldPos(vec2 screenPos, float depth)\n"
"{\n"
"vec4 pos;\n"
"pos.x = screenPos.x;\n"
"pos.y = screenPos.y;\n"
"pos.z = depth;\n"
"pos.w = 1.0;\n"
"pos = m_invviewprojection * pos;\n"
"return pos.xyz / pos.w;\n"
"}\n"
"void main ()\n"
"{\n"
"vec3 lightColour  = l_lightcolour.rgb;\n"
"float lightIntensity = 1.0;\n"
"float lightAttenuation = l_lightradius; // fixme: just use the light radius for now, use better near/far att math separately once working\n"
"float radiusFar  = l_lightradius;\n"
"float radiusNear  = l_lightradius*0.5;\n"

"vec2 fc;\n"
"fc = tf.xy / tf.w;\n"
"vec4 data = texture2D(s_t0, (1.0 + fc) / 2.0);\n"
"float depth = data.a;\n"
"vec3 norm = data.xyz;\n"

/* calc where the wall that generated this sample came from */
"vec3 worldPos = calcLightWorldPos(fc, depth);\n"

/*calc diffuse lighting term*/
"vec3 lightDir = l_lightposition - worldPos;\n"
"float zdiff = 1.0 - clamp(length(lightDir) / lightAttenuation, 0.0, 1.0);\n"
"float atten = (radiusFar * zdiff) / (radiusFar - radiusNear);\n"
"atten = pow(atten, 2.0);\n"
"lightDir = normalize(lightDir);\n"
"float nDotL = dot(norm, lightDir) * atten;\n"
"float lightDiffuse = max(0.0, nDotL);\n"

"gl_FragColor = vec4(lightDiffuse * (lightColour * lightIntensity), 1.0);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "lpp_wall",
//the final defered lighting pass.
//the lighting values were written to some render target, which is fed into this shader, and now we draw all the wall textures with it.

"varying vec2 tc, lm;\n"
"varying vec4 tf;\n"
"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"attribute vec2 v_lmcoord;\n"
"void main ()\n"
"{\n"
"tc = v_texcoord;\n"
"lm = v_lmcoord;\n"
"gl_Position = tf = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform sampler2D s_t0;\n"
"uniform sampler2D s_t1;\n"
"uniform sampler2D s_t2;\n"
"uniform vec4 e_lmscale;\n"
"void main ()\n"
"{\n"
"vec2 nst;\n"
"nst = tf.xy / tf.w;\n"
"nst = (1.0 + nst) / 2.0;\n"
"vec4 l = texture2D(s_t0, nst)*5.0;\n"
"vec4 c = texture2D(s_t1, tc);\n"
"vec3 lmsamp = texture2D(s_t2, lm).rgb*lmscale;\n"
"vec3 diff = l.rgb;\n"
"vec3 chrom = diff / (0.001 + dot(diff, vec3(0.3, 0.59, 0.11)));\n"
"vec3 spec = chrom * l.a;\n"
"gl_FragColor = vec4((diff + lmsamp) * c.xyz, 1.0);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "postproc_fisheye",
"!!cvarf ffov\n"

//fisheye view rendering, for silly fovs that are still playable.

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"varying vec2 texcoord;\n"
"void main()\n"
"{\n"
"texcoord = v_texcoord.xy;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform samplerCube s_t0;\n"
"varying vec2 texcoord;\n"
"uniform float cvar_ffov;\n"
"void main()\n"
"{\n"
"vec3 tc; \n"
"vec2 d; \n"
"vec2 ang; \n"
"d = texcoord; \n"
"ang.x = sqrt(d.x*d.x+d.y*d.y)*radians(cvar_ffov); \n"
"ang.y = -atan(d.y, d.x); \n"
"tc.x = sin(ang.x) * cos(ang.y); \n"
"tc.y = sin(ang.x) * sin(ang.y); \n"
"tc.z = cos(ang.x); \n"
"gl_FragColor = textureCube(s_t0, tc);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "postproc_panorama",
"!!cvarf ffov\n"

//panoramic view rendering, for promo map shots or whatever.

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"varying vec2 texcoord;\n"
"void main()\n"
"{\n"
"texcoord = v_texcoord.xy;\n"
"gl_Position = ftetransform();\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"uniform samplerCube s_t0;\n"
"varying vec2 texcoord;\n"
"uniform float cvar_ffov;\n"
"void main()\n"
"{\n"
"vec3 tc; \n"
"float ang; \n"
"ang = texcoord.x*-radians(cvar_ffov); \n"
"tc.x = sin(ang); \n"
"tc.y = -texcoord.y; \n"
"tc.z = cos(ang); \n"
"gl_FragColor = textureCube(s_t0, tc);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "rtlight",
"!!permu BUMP\n"
"!!permu SPECULAR\n"
"!!permu OFFSETMAPPING\n"
"!!permu SKELETAL\n"
"!!permu FOG\n"
"!!cvarf r_glsl_offsetmapping_scale\n"

//this is the main shader responsible for realtime dlights.

//texture units:
//s0=diffuse, s1=normal, s2=specular, s3=shadowmap
//custom modifiers:
//PCF(shadowmap)
//CUBE(projected cubemap)


"varying vec2 tcbase;\n"
"varying vec3 lightvector;\n"
"#if defined(SPECULAR) || defined(OFFSETMAPPING)\n"
"varying vec3 eyevector;\n"
"#endif\n"
"#if defined(PCF) || defined(CUBE)\n"
"varying vec4 vshadowcoord;\n"
"uniform mat4 l_projmatrix;\n"
"#endif\n"
"#ifdef VERTEX_SHADER\n"
"#include \"sys/skeletal.h\"\n"
"uniform vec3 l_lightposition;\n"
"attribute vec2 v_texcoord;\n"
"#if defined(SPECULAR) || defined(OFFSETMAPPING)\n"
"uniform vec3 e_eyepos;\n"
"#endif\n"
"void main ()\n"
"{\n"
"vec3 n, s, t, w;\n"
"gl_Position = skeletaltransform_wnst(w,n,s,t);\n"
"tcbase = v_texcoord; //pass the texture coords straight through\n"
"vec3 lightminusvertex = l_lightposition - w.xyz;\n"
"lightvector.x = dot(lightminusvertex, s.xyz);\n"
"lightvector.y = dot(lightminusvertex, t.xyz);\n"
"lightvector.z = dot(lightminusvertex, n.xyz);\n"
"#if defined(SPECULAR)||defined(OFFSETMAPPING)\n"
"vec3 eyeminusvertex = e_eyepos - w.xyz;\n"
"eyevector.x = dot(eyeminusvertex, s.xyz);\n"
"eyevector.y = -dot(eyeminusvertex, t.xyz);\n"
"eyevector.z = dot(eyeminusvertex, n.xyz);\n"
"#endif\n"
"#if defined(PCF) || defined(SPOT) || defined(PROJECTION) || defined(CUBE)\n"
"vshadowcoord = l_projmatrix*vec4(w.xyz, 1.0);\n"
"#endif\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"#include \"sys/fog.h\"\n"
"uniform sampler2D s_t0;\n"
"#if defined(BUMP) || defined(SPECULAR) || defined(OFFSETMAPPING)\n"
"uniform sampler2D s_t1;\n"
"#endif\n"
"#ifdef SPECULAR\n"
"uniform sampler2D s_t2;\n"
"#endif\n"
"#ifdef CUBE\n"
"uniform samplerCube s_t3;\n"
"#endif\n"
"#ifdef PCF\n"
"#ifdef CUBE\n"
"uniform samplerCubeShadow s_t7;\n"
"#else\n"
"uniform sampler2DShadow s_t7;\n"
"#endif\n"
"#endif\n"
"uniform float l_lightradius;\n"
"uniform vec3 l_lightcolour;\n"
"uniform vec3 l_lightcolourscale;\n"
"#ifdef OFFSETMAPPING\n"
"#include \"sys/offsetmapping.h\"\n"
"#endif\n"
"void main ()\n"
"{\n"
"#ifdef OFFSETMAPPING\n"
"vec2 tcoffsetmap = offsetmap(s_t1, tcbase, eyevector);\n"
"#define tcbase tcoffsetmap\n"
"#endif\n"
"vec3 bases = vec3(texture2D(s_t0, tcbase));\n"
"#if defined(BUMP) || defined(SPECULAR)\n"
"vec3 bumps = vec3(texture2D(s_t1, tcbase)) - 0.5;\n"
"#endif\n"
"#ifdef SPECULAR\n"
"vec4 specs = texture2D(s_t2, tcbase);\n"
"#endif\n"
"vec3 nl = normalize(lightvector);\n"
"float colorscale = max(1.0 - dot(lightvector, lightvector)/(l_lightradius*l_lightradius), 0.0);\n"
"vec3 diff;\n"
"#ifdef BUMP\n"
"diff = bases * (l_lightcolourscale.x + l_lightcolourscale.y * max(dot(2.0*bumps, nl), 0.0));\n"
"#else\n"
"diff = bases * (l_lightcolourscale.x + l_lightcolourscale.y * max(dot(vec3(0.0, 0.0, 1.0), nl), 0.0));\n"
"#endif\n"
"#ifdef SPECULAR\n"
"vec3 halfdir = normalize(lightvector - normalize(eyevector));\n"
"float spec = pow(max(dot(halfdir, bumps), 0.0), 1.0 + 32.0 * specs.a);\n"
"diff += spec * specs.rgb * l_lightcolourscale.z;\n"
"#endif\n"
"#ifdef CUBE\n"
"diff *= textureCube(s_t3, vshadowcoord.xyz).rgb;\n"
"#endif\n"
"#ifdef PCF\n"
"#if defined(SPOT)\n"
"const float texx = 512.0;\n"
"const float texy = 512.0;\n"
"vec4 shadowcoord = vshadowcoord;\n"
"#else\n"
"const float texx = 512.0;\n"
"const float texy = 512.0;\n"
"vec4 shadowcoord;\n"
"shadowcoord.zw = vshadowcoord.zw;\n"
"shadowcoord.xy = vshadowcoord.xy;\n"
"#endif\n"
"#ifdef CUBE\n"
"const float xPixelOffset = 1.0/texx; const float yPixelOffset = 1.0/texy; float s = 0.0;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadowCubeProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"colorscale *= s/9.0;\n"
"#else\n"
"const float xPixelOffset = 1.0/texx; const float yPixelOffset = 1.0/texy; float s = 0.0;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(-1.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(0.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, -1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, 0.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"s += shadow2DProj(s_t7, shadowcoord + vec4(1.0 * xPixelOffset * shadowcoord.w, 1.0 * yPixelOffset * shadowcoord.w, 0.05, 0.0)).r;\n"
"colorscale *= s/9.0;\n"
"#endif\n"
"#endif\n"
"#if defined(SPOT)\n"
"if (shadowcoord.w < 0.0) discard;\n"
"vec2 spot = ((shadowcoord.st)/shadowcoord.w - 0.5)*2.0;colorscale*=1.0-(dot(spot,spot));\n"
"#endif\n"
"#if defined(PROJECTION)\n"
"l_lightcolour *= texture2d(s_t3, shadowcoord);\n"
"#endif\n"
"gl_FragColor.rgb = fog3additive(diff*colorscale*l_lightcolour);\n"
"}\n"
"#endif\n"
},
#endif
#ifdef GLQUAKE
{QR_OPENGL, 110, "underwaterwarp",
"!!cvarf r_waterwarp\n"

//this is a post processing shader that is drawn fullscreen whenever the view is underwater.
//its generally expected to warp the view a little.

"#ifdef VERTEX_SHADER\n"
"attribute vec2 v_texcoord;\n"
"varying vec2 v_stc;\n"
"varying vec2 v_warp;\n"
"varying vec2 v_edge;\n"
"uniform float e_time;\n"
"void main ()\n"
"{\n"
"gl_Position = ftetransform();\n"
"v_stc = (1.0+(gl_Position.xy / gl_Position.w))/2.0;\n"
"v_warp.s = e_time * 0.25 + v_texcoord.s;\n"
"v_warp.t = e_time * 0.25 + v_texcoord.t;\n"
"v_edge = v_texcoord.xy;\n"
"}\n"
"#endif\n"
"#ifdef FRAGMENT_SHADER\n"
"varying vec2 v_stc;\n"
"varying vec2 v_warp;\n"
"varying vec2 v_edge;\n"
"uniform sampler2D s_t0;/*$currentrender*/\n"
"uniform sampler2D s_t1;/*warp image*/\n"
"uniform sampler2D s_t2;/*edge image*/\n"
"uniform vec3 e_rendertexturescale;\n"
"uniform float cvar_r_waterwarp;\n"
"void main ()\n"
"{\n"
"float amptemp;\n"
"vec3 edge;\n"
"edge = texture2D( s_t2, v_edge ).rgb;\n"
"amptemp = (0.010 / 0.625) * cvar_r_waterwarp * edge.x;\n"
"vec3 offset;\n"
"offset = texture2D( s_t1, v_warp ).rgb;\n"
"offset.x = (offset.x - 0.5) * 2.0;\n"
"offset.y = (offset.y - 0.5) * 2.0;\n"
"vec2 temp;\n"
"temp.x = v_stc.x + offset.x * amptemp;\n"
"temp.y = v_stc.y + offset.y * amptemp;\n"
"gl_FragColor = texture2D( s_t0, temp*e_rendertexturescale.st );\n"
"}\n"
"#endif\n"
},
#endif