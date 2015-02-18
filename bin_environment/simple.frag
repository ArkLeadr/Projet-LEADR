#version 400 core

uniform sampler2D texSampler;
uniform sampler2D normalMapSampler;

uniform samplerCube cubeMapSampler;

uniform sampler2DShadow shadowMapSampler;

uniform sampler2D dispMapSampler;

uniform mat4 lightMVP;

in Data {
    vec3 normal;
    vec2 texcoord;
    vec3 tangent;

    vec3 position;
    vec3 color;
} inData;

//in vec3 fNormal;
//in vec2 fTexcoord;

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
};

uniform DirLight dirLight;
uniform PointLight pointLight;

uniform vec3 eyePosition;

uniform bool wireframe;

//Material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;

//vec3 ka = vec3(1, 1, 1);
//vec3 kd = vec3(1, 1, 1);
//vec3 ks = vec3(1, 1, 1);
//float shininess = 64;

vec3 V;

vec3 B;
vec3 T;
vec3 N;

const int numLights = 1;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 texcoordColor;

#define approx(val, target, delta) ( (val <= (target + delta) ) && (val >= (target - delta) ) )

bool vec_approx(vec3 v, vec3 target, float delta) {
    return (approx(v.x, target.x, delta) && approx(v.y, target.y, delta) && approx(v.z, target.z, delta));
}


vec3 toTanSpace(vec3 v) {
    return normalize(vec3(dot(v, T), dot(v, B), dot(v, N)));
}

vec3 fromTanSpace(vec3 v) {
    return normalize(mat3(T, B, N) * v);
}

vec3 fresnel_schlick(vec3 f0, float LdotH) {
    return f0 + (1 - f0) * pow(1 - LdotH, 5);
}


vec3 blinn_phong_calc_internal(vec3 L, vec3 color, vec3 N) {
    float Id = clamp(dot(N, L), 0, 1);

    vec3 H = normalize(L + V);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(N, H), 0, 1), shininess);
    }

    // Replace the 3 colors by light ambiant, diffuse and specular intensity respectively
    float tmpAmbientFactor = 0.1;
    return (ka*color*tmpAmbientFactor + texture(texSampler, inData.texcoord).xyz*Id*color + ks*Is*color);
}

vec3 blinn_phong_calc(DirLight light, vec3 N) {
    vec3 L = normalize(-light.direction);

    return blinn_phong_calc_internal(L, light.color, N);
}

vec3 blinn_phong_calc(PointLight light, vec3 N) {
    vec3 L = normalize(light.position - inData.position);

    return blinn_phong_calc_internal(L, light.color, N);
}

/* LEADR SPECIFIC */

// 2 GL_RGBA16F textures (becomes float vec4 here)
uniform sampler2D leadr1; //~x, ~y, ~x*~y, ~x²
uniform sampler2D leadr2; //~y², h

float E_xn = 0;
float E_yn = 0;
float E_xnyn = 0;
float E_xn2 = 0;

float E_yn2 = 0;

float disp = 0;

float var_x = 0;
float var_y = 0;
float c_xy = 0;


//vec4 mean1 = textureLod(leadr1, inData.texcoord, 4096); // 4096 should be clamped to the max mip level
//vec4 mean2 = textureLod(leadr2, inData.texcoord, 4096); // giving us then the total mean

//END JUNK

/* Traitement LEADR */
void computeLeadrStatistics()
{
//    float fakeLod = 1;

//    vec4 stats1 = textureLod(leadr1, inData.texcoord, fakeLod);
//    vec4 stats2 = textureLod(leadr2, inData.texcoord, fakeLod);

    vec4 stats1 = texture(leadr1, inData.texcoord);
    vec4 stats2 = texture(leadr2, inData.texcoord);

    float baseRoughnessOffset = 0;

    E_xn    = stats1.x;
    E_yn    = stats1.y;
    E_xnyn  = stats1.z;
    E_xn2   = stats1.w;

    E_yn2   = stats2.x;

    disp    = stats2.y;

    var_x = E_xn2 - E_xn*E_xn  + baseRoughnessOffset;

    var_y = E_yn2 - E_yn*E_yn  + baseRoughnessOffset;

    c_xy = E_xnyn - E_xn*E_yn;

}

vec3 getMesoTan() {
    return normalize(vec3(-E_xn, -E_yn, 1));
}

vec3 getMesoWorld() {
    return normalize(-E_xn*T - E_yn*B + N);
}

float P22(float x, float y)
{
    x -= E_xn;
    y -= E_yn;

    float det = var_x * var_y - c_xy * c_xy ;

//    return exp(88.73);

    if (approx(det, 0, 0.0000000000001)) // "Dirac if plane"
    {
//        return float((x == 0) && (y == 0));

        return float(approx(x, 0, 0.0001) && approx(y, 0, 0.0001));
    }

//    return det + 0.1;

    float arg_exp = -0.5 * ( x * x * var_y + y * y * var_x - 2.0* x * y * c_xy ) / det ;

//    return arg_exp;

//    float val = arg_exp;

//    return exp( arg_exp - 3000000000000000000.0);

    float P22_ = 0.15915 * inversesqrt ( abs(det) ) * exp ( arg_exp );

    return float(P22_ > 1);
}

float D_beckmann(vec3 n) {
    float x = -n.x / n.z;
    float y = -n.y / n.z;

    float P22_ = P22(x, y);

    float D_ = P22_ / (n.z * n.z * n.z * n.z); // Here, n.z is the same as dot(wn, wg) given in the formula

//    return (approx(n.z, dot(n, toTanSpace(N)), 0) ? 1 : 0); WORKS ! PROOF

//    return float(isnan(P22_) || isinf(P22_));

//    return P22_;

    return D_;
}

float D_dirac(vec3 n) {
    return (vec_approx(n, vec3(0, 0, 1), 0.01) ? 1 : 0);
//    return (n == vec3(0, 0, 1) ? 1 : 0);
}

float D_cos(vec3 n) {
    vec3 mesoTan = getMesoTan();

    return max(0.0, dot(n, mesoTan));
}

float smith(vec3 v) {
    vec2 v_xy = normalize(vec2(dot(v, T), dot(v, B)));

    float mu_phi = v_xy.x * E_xn + v_xy.y * E_yn;
    float var_phi = v_xy.x * v_xy.x * var_x + v_xy.y * v_xy.y * var_y + 2.0* v_xy.x * v_xy.y * c_xy ;

    float cos_theta_v = dot(v , N);

    float cot_theta_v = cos_theta_v / sqrt(1.0 - cos_theta_v * cos_theta_v);

    float nu_v = clamp((cot_theta_v - mu_phi) * inversesqrt(2.0 * var_phi), 0.001, 1.599);

    float Lambda_ = (1.0 -1.259 * nu_v + 0.396 * nu_v * nu_v) / (3.535 * nu_v + 2.181 * nu_v * nu_v);

    return Lambda_ ;
}


float roughSpecularPointLight(vec3 L) {

    vec3 H = normalize(L + V);

    vec3 Htan = toTanSpace(H);
    vec3 Ltan = toTanSpace(L);
    if ( Htan.z <= 0.0) return 0.0;

    float d = D_beckmann(Htan);

    float lambda_V = smith((V)) * 1;
    float lambda_L = smith((L)) * 1;

    float masking_shadowing_terms = 1 / (1 + lambda_V + lambda_V);

    float inverse_projected_area = 1 / dot(getMesoWorld(), V);

//    float I = 0.25 * inverse_projected_area * d * masking_shadowing_terms;

    float I = d;

    return I;


    return I + 10000;
}


const int nbSamples = 5;
const float sampleLength = 0.9;

const float p[5] = float[5](-1.8, -0.9, 0, 0.9, 1.8);
const float w[5] = float[5](0.0725, 0.2443, 0.3663, 0.2443, 0.0725);

/* TODO : see how to integrate Fresnel + smith terms in this equation */
vec3 roughSpecularCubeMap() {
    float sigma_x = sqrt(var_x);
    float sigma_y = sqrt(var_y);

    float r_xy = 1;

    if (sigma_x*sigma_y != 0)
        r_xy = c_xy / (sigma_x*sigma_y);

    float sq_one_minus_r_xy2 = sqrt(1 - r_xy*r_xy);

//    float LODoffset = -1.48 + log2(sampleLength*max(sigma_x, sigma_y)* float(textureSize(cubeMapSampler, 0).x));

    // Replace -1.48 by 0.5 as said in the supplemental
    float LODoffset = 0.5 + log2(sampleLength*max(sigma_x, sigma_y)* float(textureSize(cubeMapSampler, 0).x));

    float S = 0;
    vec3 I = vec3(0);


    for (int j = 0; j < nbSamples; ++j) {
        for (int k = 0; k < nbSamples; ++k) {
            float x = p[j]*sigma_x + E_xn;
            float y = (r_xy*p[j] + sq_one_minus_r_xy2*p[k]) * sigma_y + E_yn;


            // microfacet normal
            vec3 meso = getMesoWorld();

            // microfacet projected area
            float dotVN = max(0.0, dot(V, meso));
            float dotZN = dot(N, meso);
            float S_ = dotVN / dotZN;

            // reflected vector
            vec3 R = reflect(-V, meso);

            // Jacobian : slope space area to solid angle
            float J = abs(4.0 * dotZN*dotZN*dotZN * dotVN);

            // LOD in cube map
            float LOD = 0.72*log(J) + LODoffset ;
            // radiance
            vec3 I_ = textureLod(cubeMapSampler, R, LOD).rgb;
            // sum up
            S += w[j]*w[k] * S_ ;
            I += w[j]*w[k] * S_ * I_ ;
        }
    }

    return I / S;
}

uniform mat4 hred;
uniform mat4 hgreen;
uniform mat4 hblue;

uniform vec3 shc_env[9];

#define L00     0
#define L1_1    1
#define L10     2
#define L11     3
#define L2_2    4
#define L2_1    5
#define L20     6
#define L21     7
#define L22     8

vec3 Llm[9] = vec3[9](vec3(0), vec3(0), vec3(0), vec3(0), vec3(0), vec3(0), vec3(0), vec3(0), vec3(0));

/* Algo 2 */
void algo2()
{
    float smith_wo = 0;

    for (int i = 0; i < 9; ++i) {
        Llm[i] = vec3(0);
    }

    vec3 LD = normalize(pointLight.position - eyePosition);
    vec3 LC = pointLight.color;

    for (int i = 0; i < numLights; ++i) {
        vec3 Li = LC * 0.1; //vec3(0);
        vec3 wi = LD; //vec3(0);

        float xi = wi.x;
        float yi = wi.y;
        float zi = wi.z;

        float smith_wi = 0;

        vec3 P = Li/(1 + smith_wo + smith_wi);

        float c;

        /* L_{00}.  Note that Y_{00} = 0.282095 */
        c = 0.282095 ;
        Llm[L00] += c * P;

        /* L_{1m}. -1 <= m <= 1.  The linear terms */
        c = 0.488603 ;
        Llm[L1_1] += c * yi * P;
        Llm[L10] += c * zi * P;
        Llm[L11] += c * xi * P;

        /* The Quadratic terms, L_{2m} -2 <= m <= 2 */

        /* First, L_{2-2}, L_{2-1}, L_{21} corresponding to xy,yz,xz */
        c = 1.092548 ;
        Llm[L2_2] += c * xi*yi * P;
        Llm[L2_1] += c * yi*zi * P;
        Llm[L21] += c * xi*zi * P;

        /* L_{20}.  Note that Y_{20} = 0.315392 (3z^2 - 1) */
        c = 0.315392 ;
        Llm[L20] += c * (3*zi*zi - 1) * P;

        /* L_{22}.  Note that Y_{22} = 0.546274 (x^2 - y^2) */
        c = 0.546274 ;
        Llm[L22] += c * (xi*xi - yi*yi) * P;
    }

    for (int i = 0; i < 9; ++i) {
        Llm[i] += shc_env[i] / (1 + smith_wo);
    }
}

/* E(wn) */
vec3 E(vec3 n) {
    float xn = n.x;
    float yn = n.y;
    float zn = n.z;

    return
            0.429043 * Llm[L22] * (xn*xn - yn*yn)
            + 0.743125 * Llm[L20] * zn*zn
            + 0.886227 * Llm[L00] - 0.247708 * Llm[L20]
            + 0.858086 * (Llm[L2_2]*xn*yn + Llm[L21]*xn*zn + Llm[L2_1]*yn*zn)
            + 1.023328 * (Llm[L11]*xn + Llm[L1_1]*yn + Llm[L10]*zn)
            ;
}

vec3 E_mat(vec3 n) {
    vec3 ret;

    vec4 v = vec4(n, 1);

    ret.x = dot(v, (hred) * v);
    ret.y = dot(v, (hgreen) * v);
    ret.z = dot(v, (hblue) * v);

    return ret;
}

/* LEADR SPECIFIC END */

vec3 dumbTest(vec3 self) {
//    return fromTanSpace((getMesoTan()));

//    return (E_yn < 0 ? vec3(E_yn) : vec3(1, 0, 0));

//    return vec3(float(c_xy > 10));

//    return vec3(D_beckmann(vec3(0.0, 0.0, 1.0)));

    return self;
}


void main( void )
{
    V = normalize(eyePosition - inData.position); // Initialize global V (view vector) for all further uses


    computeLeadrStatistics();

    vec3 finalColor;

    vec3 normal = 2.0 * texture(normalMapSampler, inData.texcoord).xyz - vec3(1.0, 1.0, 1.0);

    normal = normalize(normal);

    vec3 worldNormal = normalize(inData.normal);
    vec3 worldTangent = normalize(inData.tangent);
    vec3 worldBitangent = normalize(cross(worldNormal, worldTangent));

    N = worldNormal;
    T = worldTangent;
    B = worldBitangent;

    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * normal);

//    normal = vec3(gl_FrontFacing ? 1 : -1);

//    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * vec3(0,0,1));

//    if (normal.x < 0.997) {
//        discard;
//    }

    normal = normalize(inData.normal);

    vec3 texAlbedo = texture(texSampler, inData.texcoord).xyz;

//    finalColor = texture(texSampler, inData.texcoord).xyz;

//    finalColor = texture(cubeMapSampler, reflect(normalize(eyePosition - inData.position), normal)).xyz;

    vec3 Cfinal = blinn_phong_calc(dirLight, normal) + blinn_phong_calc(pointLight, normal);

    fragColor = vec4( /*finalColor **/ Cfinal, 1.0 );

#if ENABLED_SHADOWING
    vec4 lightSpaceFragPosition = lightMVP * vec4(inData.position, 1);

    vec3 projCoords = lightSpaceFragPosition.xyz / lightSpaceFragPosition.w;

    vec3 shadowMapUVCoords;
    shadowMapUVCoords.x = 0.5 * projCoords.x + 0.5;
    shadowMapUVCoords.y = 0.5 * projCoords.y + 0.5;
    shadowMapUVCoords.z = 0.5 * projCoords.z + 0.5;
    float z = 0.5 * projCoords.z + 0.5;

    shadowMapUVCoords.z = shadowMapUVCoords.z - 0.00005;

    float shadowFactor;

    float depth = texture(shadowMapSampler, shadowMapUVCoords);

    shadowFactor = depth;

    fragColor = fragColor * shadowFactor;
#endif

//    fragColor = texture(cubeMapSampler, reflect(-V, normal));

    normalColor = vec4(normal, 1);
    texcoordColor = vec4(inData.texcoord, 0, 1);

    vec3 pointL = normalize(pointLight.position - inData.position);

    vec3 pointH = normalize(pointL + V);

//    pointL = normalize(vec3(1, 0, 0));

    vec3 H = normalize(pointL + V);

    vec3 Htan = toTanSpace(H);

//    fragColor.xyz = vec3(roughSpecularPointLight(pointL)); // WARNING ! NaN detected here :(

    fragColor.xyz = roughSpecularCubeMap();

    vec3 mesoN = normalize(-E_xn*T - E_yn*B + N);

//    fragColor.xyz = blinn_phong_calc(pointLight, mesoN);

//    fragColor.xyz = texture(leadr2, inData.texcoord).yyy;

//    float val = textureLod(leadr2, inData.texcoord, 4096).y;

    fragColor.xyz = vec3(D_beckmann(toTanSpace(H)));

//    fragColor.xyz = texture(dispMapSampler, inData.texcoord).yyy;

//    fragColor.xyz = vec3(V.z);

//    normal = normalize(inData.normal);

//    algo2();

//    float albedo = 6.5;

//    fragColor.xyz = E(N) * albedo;

//    fragColor.xyz = vec3(D_beckmann(normalize(vec3(1, 1, 1))));

//    fragColor.xyz = texture(dispMapSampler, inData.texcoord).xyz;

    fragColor.xyz = dumbTest(fragColor.xyz);
}
