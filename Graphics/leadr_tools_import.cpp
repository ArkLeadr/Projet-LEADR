#include "leadr_tools_import.h"




bool importCoeffs(const char *filename, SphericalHarmonicsCoeffs &shc) {
    FILE* fp = NULL;

    fp = fopen(filename, "rb");

    if (!fp) {
        std::cerr << "Error opening file " << filename << '\n';

        return false;
    }

    fread((void*) shc.coeffs, sizeof(float), 9*3, fp);

    fclose(fp);

    return true;
}


void computeMatrixRepresentation(SphericalHarmonicsCoeffs &shc) {
    toSingleChannelMatrix(shc.coeffs, shc.hred, 0);
    toSingleChannelMatrix(shc.coeffs, shc.hgreen, 1);
    toSingleChannelMatrix(shc.coeffs, shc.hblue, 2);
}


void toSingleChannelMatrix(SimpleShc coeffs, mat4 &m, int channel) {
    float c1,c2,c3,c4,c5 ;
    c1 = 0.429043 ; c2 = 0.511664 ;
    c3 = 0.743125 ; c4 = 0.886227 ; c5 = 0.247708 ;

    m[0][0] = c1*coeffs[8][channel] ; /* c1 L_{22}  */
    m[0][1] = c1*coeffs[4][channel] ; /* c1 L_{2-2} */
    m[0][2] = c1*coeffs[7][channel] ; /* c1 L_{21}  */
    m[0][3] = c2*coeffs[3][channel] ; /* c2 L_{11}  */

    m[1][0] = c1*coeffs[4][channel] ; /* c1 L_{2-2} */
    m[1][1] = -c1*coeffs[8][channel]; /*-c1 L_{22}  */
    m[1][2] = c1*coeffs[5][channel] ; /* c1 L_{2-1} */
    m[1][3] = c2*coeffs[1][channel] ; /* c2 L_{1-1} */

    m[2][0] = c1*coeffs[7][channel] ; /* c1 L_{21}  */
    m[2][1] = c1*coeffs[5][channel] ; /* c1 L_{2-1} */
    m[2][2] = c3*coeffs[6][channel] ; /* c3 L_{20}  */
    m[2][3] = c2*coeffs[2][channel] ; /* c2 L_{10}  */

    m[3][0] = c2*coeffs[3][channel] ; /* c2 L_{11}  */
    m[3][1] = c2*coeffs[1][channel] ; /* c2 L_{1-1} */
    m[3][2] = c2*coeffs[2][channel] ; /* c2 L_{10}  */
    m[3][3] = c4*coeffs[0][channel] - c5*coeffs[6][channel] ;
}


void printCoeffs(const SimpleShc coeffs) {
    printf("\n         Lighting Coefficients\n\n") ;
    printf("(l,m)       RED        GREEN     BLUE\n") ;

    printf("L_{0,0}   %9.6f %9.6f %9.6f\n",
           coeffs[0][0],coeffs[0][1],coeffs[0][2]) ;
    printf("L_{1,-1}  %9.6f %9.6f %9.6f\n",
           coeffs[1][0],coeffs[1][1],coeffs[1][2]) ;
    printf("L_{1,0}   %9.6f %9.6f %9.6f\n",
           coeffs[2][0],coeffs[2][1],coeffs[2][2]) ;
    printf("L_{1,1}   %9.6f %9.6f %9.6f\n",
           coeffs[3][0],coeffs[3][1],coeffs[3][2]) ;
    printf("L_{2,-2}  %9.6f %9.6f %9.6f\n",
           coeffs[4][0],coeffs[4][1],coeffs[4][2]) ;
    printf("L_{2,-1}  %9.6f %9.6f %9.6f\n",
           coeffs[5][0],coeffs[5][1],coeffs[5][2]) ;
    printf("L_{2,0}   %9.6f %9.6f %9.6f\n",
           coeffs[6][0],coeffs[6][1],coeffs[6][2]) ;
    printf("L_{2,1}   %9.6f %9.6f %9.6f\n",
           coeffs[7][0],coeffs[7][1],coeffs[7][2]) ;
    printf("L_{2,2}   %9.6f %9.6f %9.6f\n",
           coeffs[8][0],coeffs[8][1],coeffs[8][2]) ;
}

void printMatricesToGlslDeclaration(const SphericalHarmonicsCoeffs& shc) {
    printf("\nconst mat4 hred = mat4(\n") ;
    for (int i = 0 ; i < 4 ; i++) {
        printf("    ");
        for (int j = 0 ; j < 4 ; j++) {
            if (i == 3 && j == 3) {
                printf("%9.6f ", shc.hred[i][j]) ;
            }
            else {
                printf("%9.6f, ", shc.hred[i][j]) ;
            }
        }
        printf("\n") ;
    }
    printf(") ;\n");

    printf("\nconst mat4 hgreen = mat4(\n") ;
    for (int i = 0 ; i < 4 ; i++) {
        printf("    ");
        for (int j = 0 ; j < 4 ; j++) {
            if (i == 3 && j == 3) {
                printf("%9.6f ", shc.hgreen[i][j]) ;
            }
            else {
                printf("%9.6f, ", shc.hgreen[i][j]) ;
            }
        }
        printf("\n") ;
    }
    printf(") ;\n");

    printf("\nconst mat4 hblue = mat4(\n") ;
    for (int i = 0 ; i < 4 ; i++) {
        printf("    ");
        for (int j = 0 ; j < 4 ; j++) {
            if (i == 3 && j == 3) {
                printf("%9.6f ", shc.hblue[i][j]) ;
            }
            else {
                printf("%9.6f, ", shc.hblue[i][j]) ;
            }
        }
        printf("\n") ;
    }
    printf(") ;\n");
}

class Proxy {
    int w;
    int h;

    vec4* p;

public:

    Proxy(int w, int h, vec4* p) : w(w), h(h), p(p) {}

    vec4& operator() (int i, int j) {
        i = i - 1;
        j = j - 1;

        return p[i * w + j];
    }
};

#define approx(val, target, delta) ((val <= target + delta) && (val >= target - delta))

bool importSingleLeadrTexture(const char *filename, Texture& leadr) {
    FILE* fp = NULL;

    fp = fopen(filename, "rb");

    if (!fp) {
        std::cerr << "Error opening file " << filename << '\n';

        return false;
    }

//    printf("Loading leadr texture : %s \n", filename);

//    float ten[10] = {0};

//    fread((void*) &ten, sizeof(float), 10, fp);

//    for (int i = 0; i < 10; ++i) {
//        std::cout << ten[i] << '\n';
//    }

//    exit(0);

    int width, height;

    fread((void*) &width, sizeof(int), 1, fp);
    fread((void*) &height, sizeof(int), 1, fp);

    size_t size = width * height;

    float* pixelData = new float[size*4];

    fread((void*) pixelData, sizeof(float), size*4, fp);

    Proxy proxy(width, height, (vec4*) pixelData);

    std::cout << "Width : " << width << ", Height : " << height << '\n';

    std::cout << '\n';



    if (std::string(filename) == "tex1") {
        for (int i = 1; i <= 512; ++i)
        for (int j = 1; j <= 512; ++j)
        {
//            std::cout <<  << '\n';

            assert(approx(proxy(i, j).w  - proxy(i, j).x*proxy(i, j).x, 0, 0.00001));
//            std::cout << proxy(1, i).z - proxy(1, i).x*proxy(1, i).x << '\n';
        }
    }

//    exit(0);

    fclose(fp);

    leadr.loadFromBlob(width, height, GL_RGBA32F, GL_RGBA, GL_FLOAT, pixelData);

    delete pixelData;

    return true;
}


bool importLeadrTextures(const char *filename1, const char *filename2, Texture &leadr1, Texture &leadr2) {
    importSingleLeadrTexture(filename1, leadr1);
    importSingleLeadrTexture(filename2, leadr2);

    return true;
}
