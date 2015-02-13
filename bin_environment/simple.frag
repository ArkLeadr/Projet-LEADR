#version 400 core

uniform sampler2D texSampler;
uniform sampler2D normalMapSampler;

uniform samplerCube cubeMapSampler;

uniform sampler2DShadow shadowMapSampler;

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


layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 texcoordColor;

vec3 blinn_phong_calc_internal(vec3 lightDir, vec3 color, vec3 normal) {
    float Id = clamp(dot(normal, lightDir), 0, 1);

    vec3 viewDir = normalize(eyePosition - inData.position);
    vec3 halfV = normalize(lightDir + viewDir);

    float Is = 0;
    if (Id > 0) {
        Is = pow(clamp(dot(normal, halfV), 0, 1), shininess);
    }

    // Replace the 3 colors by light ambiant, diffuse and specular intensity respectively
    float tmpAmbientFactor = 0.1;
    return (ka*color*tmpAmbientFactor + texture(texSampler, inData.texcoord).xyz*Id*color + ks*Is*color);
}

vec3 blinn_phong_calc(DirLight light, vec3 normal) {
    vec3 lightDir = normalize(-light.direction);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}

vec3 blinn_phong_calc(PointLight light, vec3 normal) {
    vec3 lightDir = normalize(light.position - eyePosition);

    return blinn_phong_calc_internal(lightDir, light.color, normal);
}

const mat4 gracered = mat4(
     0.009098, -0.004780,  0.024033, -0.014947,
    -0.004780, -0.009098, -0.011258,  0.020210,
     0.024033, -0.011258, -0.011570, -0.017383,
    -0.014947,  0.020210, -0.017383,  0.073787
) ;

const mat4 gracegreen = mat4(
    -0.002331, -0.002184,  0.009201, -0.002846,
    -0.002184,  0.002331, -0.009611,  0.017903,
     0.009201, -0.009611, -0.007038, -0.009331,
    -0.002846,  0.017903, -0.009331,  0.041083
) ;

const mat4 graceblue = mat4(
    -0.013032, -0.005248,  0.005970,  0.000483,
    -0.005248,  0.013032, -0.020370,  0.030949,
     0.005970, -0.020370, -0.010948, -0.013784,
     0.000483,  0.030949, -0.013784,  0.051648
) ;

uniform mat4 hred;
uniform mat4 hgreen;
uniform mat4 hblue;

uniform vec3 shc_env[9];

const int numLights = 1;

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

/*
E(ωn ) = 0.429043L22 (x2 − yn ) + 0.743125L20 zn
n
+ 0.886227L00 − 0.247708L20
+ 0.858086(L2−2 xn yn + L21 xn zn + L2−1 yn zn )
+ 1.023328(L11 xn + L1−1 yn + L10 zn ).
*/

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

vec3 Emat(vec3 n) {
    vec3 ret;

    vec4 v = vec4(n, 1);

    ret.x = dot(v, (hred) * v);
    ret.y = dot(v, (hgreen) * v);
    ret.z = dot(v, (hblue) * v);

    return ret;
}


void main( void )
{
    vec3 finalColor;

    vec3 normal = 2.0 * texture(normalMapSampler, inData.texcoord).xyz - vec3(1.0, 1.0, 1.0);

    normal = normalize(normal);

    vec3 worldNormal = normalize(inData.normal);
    vec3 worldTangent = normalize(inData.tangent);
    vec3 worldBitangent = normalize(cross(worldNormal, worldTangent));

    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * normal);

//    normal = vec3(gl_FrontFacing ? 1 : -1);

//    normal = normalize(mat3(worldTangent, worldBitangent, worldNormal) * vec3(0,0,1));

//    if (normal.x < 0.997) {
//        discard;
//    }

//    normal = normalize(inData.normal);

    finalColor = texture(texSampler, inData.texcoord).xyz;

//    finalColor = texture(cubeMapSampler, reflect(normalize(eyePosition - inData.position), normal)).xyz;

    vec3 Cfinal = blinn_phong_calc(dirLight, normal) + blinn_phong_calc(pointLight, normal);

    fragColor = vec4( /*finalColor **/ Cfinal, 1.0 );


    /* DEBUG OUTPUT */
    /* WIREFRAME FAIT MAISON */
    if (wireframe) {
        float threshold = 0.01;
        if(inData.color.x >= threshold && inData.color.y >= threshold && inData.color.z >= threshold) {
            discard;
        }
        else {
    //        fragColor = vec4( inData.color, 1.0 );
        }
    }

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


//    fragColor = texture(cubeMapSampler, reflect(-normalize(eyePosition - inData.position), normal));

    normalColor = vec4(normal, 1);
    texcoordColor = vec4(inData.texcoord, 0, 1);



    normal = normalize(inData.normal);

    algo2();

    float albedo = 6.5;

    fragColor.xyz = E(normal) * albedo;
}
