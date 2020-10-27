// readable smallpt, a Path Tracer by Kevin Beason, 2008.
// Adjusted // for my particular readability sensitivities by Roger Allen, 2016
// Added C++11 multithreading & removed openmp.
// Adolfo Muñoz, 2020
//    - Added new scenes by Kevin Beason
//    - Removed debugging printfs
//    - Added command line parameters

// Make:
// smallpt_thd: smallpt_thd.cpp
//	g++ -Wall -std=c++11 -O3 smallpt_thd.cpp -pthread -o smallpt_thd

// Usage: time ./smallpt 100
// N  real
// 1  151
// 2   81
// 3
// 4   43
// 5   40
// 6   40
// 7
// 8   32

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

// Vec is a structure to store position (x,y,z) and color (r,g,b)
struct Vec {
    double x, y, z;
    Vec(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; }
    Vec operator+(const Vec &b) const {
        return Vec(x+b.x,y+b.y,z+b.z);
    }
    Vec operator-(const Vec &b) const {
        return Vec(x-b.x,y-b.y,z-b.z);
    }
    Vec operator*(double b) const {
        return Vec(x*b,y*b,z*b);
    }
    Vec mult(const Vec &b) const {
        return Vec(x*b.x,y*b.y,z*b.z);
    }
    Vec& norm() {
        return *this = *this * (1/sqrt(x*x+y*y+z*z));
    }
    double dot(const Vec &b) const {
        return x*b.x+y*b.y+z*b.z;
    }
    // % is cross product
    // rallen ??? why no const here?
    Vec operator%(Vec&b){
        return Vec(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
    }
};

struct Ray {
    Vec o, d;
    Ray(Vec o_, Vec d_) : o(o_), d(d_) {}
};

// material types, used in radiance()
enum Refl_t { DIFF, SPEC, REFR };

struct Sphere {
    double rad;       // radius
    Vec p, e, c;      // position, emission, color
    Refl_t refl;      // reflection type (DIFFuse, SPECular, REFRactive)
    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
        rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}
    double intersect(const Ray &r) const {
        // returns distance, 0 if nohit
        Vec op = p-r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
        double t, eps=1e-4, b=op.dot(r.d), det=b*b-op.dot(op)+rad*rad;
        if (det<0) {
            return 0;
        }
        else {
            det=sqrt(det);
        }
        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
    }
};

//Scene: radius, position, emission, color, material
std::vector<Sphere> spheres = {
    Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF),//Left
    Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF),//Rght
    Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.75,.75,.75),DIFF),//Back
    Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF),//Frnt
    Sphere(1e5, Vec(50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF),//Botm
    Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF),//Top
    Sphere(16.5,Vec(27,16.5,47),       Vec(),Vec(1,1,1)*.999, SPEC),//Mirr
    Sphere(16.5,Vec(73,16.5,78),       Vec(),Vec(1,1,1)*.999, REFR),//Glas
    Sphere(600, Vec(50,681.6-.27,81.6),Vec(12,12,12),  Vec(), DIFF) //Lite
};

void set_scene(const std::string& name) {
    //Extra scenes
    //Kevin Beason
    //12/04/2008

    if (name == "sky") {
        // Idea stolen from Picogen http://picogen.org/ by phresnel/greenhybrid
        Vec Cen(50,40.8,-860);
        spheres = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0

           Sphere(1600, Vec(1,0,2)*3000, Vec(1,.9,.8)*1.2e1*1.56*2,Vec(), DIFF), // sun
           Sphere(1560, Vec(1,0,2)*3500,Vec(1,.5,.05)*4.8e1*1.56*2, Vec(),  DIFF), // horizon sun2
        //   Sphere(10000,Cen+Vec(0,0,-200), Vec(0.0627, 0.188, 0.569)*6e-2*8, Vec(.7,.7,1)*.25,  DIFF), // sky
           Sphere(10000,Cen+Vec(0,0,-200), Vec(0.00063842, 0.02001478, 0.28923243)*6e-2*8, Vec(.7,.7,1)*.25,  DIFF), // sky

          Sphere(100000, Vec(50, -100000, 0),  Vec(),Vec(.3,.3,.3),DIFF), // grnd
          Sphere(110000, Vec(50, -110048.5, 0),  Vec(.9,.5,.05)*4,Vec(),DIFF),// horizon brightener
          Sphere(4e4, Vec(50, -4e4-30, -3000),  Vec(),Vec(.2,.2,.2),DIFF),// mountains
        //  Sphere(3.99e4, Vec(50, -3.99e4+20.045, -3000),  Vec(),Vec(.7,.7,.7),DIFF),// mountains snow

           Sphere(26.5,Vec(22,26.5,42),   Vec(),Vec(1,1,1)*.596, SPEC), // white Mirr
           Sphere(13,Vec(75,13,82),   Vec(),Vec(.96,.96,.96)*.96, REFR),// Glas
          Sphere(22,Vec(87,22,24),   Vec(),Vec(.6,.6,.6)*.696, REFR)    // Glas2
        };
    }
    else if (name == "nightsky") {
       spheres = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0
          //     rad       pos                   emis           col     refl
        //  Sphere(1e3,   Vec(1,1,-2)*1e4,    Vec(1,1,1)*5e2,     Vec(), DIFF), // moon
        //  Sphere(3e2,   Vec(.6,.2,-2)*1e4,    Vec(1,1,1)*5e3,     Vec(), DIFF), //
        //  moon

          Sphere(2.5e3,   Vec(.82,.92,-2)*1e4,    Vec(1,1,1)*.8e2,     Vec(), DIFF), // moon

        //  Sphere(2.5e4, Vec(50, 0, 0),     Vec(1,1,1)*1e-3,    Vec(.2,.2,1)*0.0075, DIFF), // sky
        //  Sphere(2.5e4, Vec(50, 0, 0),  Vec(0.114, 0.133, 0.212)*1e-2,  Vec(.216,.384,1)*0.0007, DIFF), // sky

          Sphere(2.5e4, Vec(50, 0, 0),  Vec(0.114, 0.133, 0.212)*1e-2,  Vec(.216,.384,1)*0.003, DIFF), // sky

          Sphere(5e0,   Vec(-.2,0.16,-1)*1e4, Vec(1.00, 0.843, 0.698)*1e2,   Vec(), DIFF),  // star
          Sphere(5e0,   Vec(0,  0.18,-1)*1e4, Vec(1.00, 0.851, 0.710)*1e2,  Vec(), DIFF),  // star
          Sphere(5e0,   Vec(.3, 0.15,-1)*1e4, Vec(0.671, 0.780, 1.00)*1e2,   Vec(), DIFF),  // star
          Sphere(3.5e4,   Vec(600,-3.5e4+1, 300), Vec(),   Vec(.6,.8,1)*.01,  REFR),   //pool
          Sphere(5e4,   Vec(-500,-5e4+0, 0),   Vec(),      Vec(1,1,1)*.35,  DIFF),    //hill
          Sphere(16.5,  Vec(27,0,47),         Vec(),              Vec(1,1,1)*.33, DIFF), //hut
          Sphere(7,     Vec(27+8*sqrt(2),0,47+8*sqrt(2)),Vec(),  Vec(1,1,1)*.33,  DIFF), //door
          Sphere(500,   Vec(-1e3,-300,-3e3), Vec(),  Vec(1,1,1)*.351,    DIFF),  //mnt
          Sphere(830,   Vec(0,   -500,-3e3), Vec(),  Vec(1,1,1)*.354,    DIFF),  //mnt
          Sphere(490,  Vec(1e3,  -300,-3e3), Vec(),  Vec(1,1,1)*.352,    DIFF),  //mnt
        };
    }
    else if (name == "island") {
        // Inspired by cover of "Time Planet Earth: An Illustrated History"
        Vec Cen(50,-20,-860);
        spheres = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0
          //     rad       pos                   emis           col     refl

          Sphere(160,  Cen+Vec(0, 600, -500),Vec(1,1,1)*2e2, Vec(),  DIFF), // sun
          Sphere(800, Cen+Vec(0,-880,-9120),Vec(1,1,1)*2e1, Vec(),  DIFF), // horizon
          Sphere(10000,Cen+Vec(0,0,-200), Vec(0.0627, 0.188, 0.569)*1e0, Vec(1,1,1)*.4,  DIFF), // sky

        //  Sphere(1000, Cen+Vec(0,-1080,-8020),Vec(1,1,1)*2e1, Vec(),  DIFF), // horizon
        //  Sphere(10000,Cen+Vec(0,0,-200), Vec(0.0627, 0.188, 0.569)*1e0, Vec(1,1,1)*.3,  DIFF), // sky

        //  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0, 0.588, 0.8),  REFR), // water
        //  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.106, 0.725, 0.949),  REFR), // water
        //  Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.110, 0.988, 0.945),  REFR), // water
          Sphere(800, Cen+Vec(0,-720,-200),Vec(),  Vec(0.110, 0.898, 1.00)*.996,  REFR), // water
          Sphere(790, Cen+Vec(0,-720,-200),Vec(),  Vec(.4,.3,.04)*.6,    DIFF), // earth
          Sphere(325, Cen+Vec(0,-255,-50), Vec(),  Vec(.4,.3,.04)*.8,       DIFF), // island
          Sphere(275, Cen+Vec(0,-205,-33), Vec(),  Vec(.02,.3,.02)*.75,      DIFF), // grass
        };
    }
    else if (name == "vista") {
        Vec Cen(50,-20,-860);
        spheres = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0
          //     rad       pos                   emis           col     refl

          Sphere(8000, Cen+Vec(0,-8000,-900),Vec(1,.4,.1)*5e-1, Vec(),  DIFF), // sun
          Sphere(1e4,  Cen+Vec(), Vec(0.631, 0.753, 1.00)*3e-1, Vec(1,1,1)*.5,  DIFF), // sky

          Sphere(150,  Cen+Vec(-350,0, -100),Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt
          Sphere(200,  Cen+Vec(-210,0,-100), Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt
          Sphere(145,  Cen+Vec(-210,85,-100),Vec(),  Vec(1,1,1)*.8,  DIFF), // snow
          Sphere(150,  Cen+Vec(-50,0,-100),  Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt
          Sphere(150,  Cen+Vec(100,0,-100),  Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt
          Sphere(125,  Cen+Vec(250,0,-100),  Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt
          Sphere(150,  Cen+Vec(375,0,-100),  Vec(),  Vec(1,1,1)*.3,  DIFF), // mnt

          Sphere(2500, Cen+Vec(0,-2400,-500),Vec(),  Vec(1,1,1)*.1,  DIFF), // mnt base

          Sphere(8000, Cen+Vec(0,-8000,200), Vec(),  Vec(.2,.2,1),    REFR), // water
          Sphere(8000, Cen+Vec(0,-8000,1100),Vec(),  Vec(0,.3,0),     DIFF), // grass
          Sphere(8   , Cen+Vec(-75, -5, 850),Vec(),  Vec(0,.3,0),     DIFF), // bush
          Sphere(30,   Cen+Vec(0,   23, 825),Vec(),  Vec(1,1,1)*.996, REFR), // ball

          Sphere(30,  Cen+Vec(200,280,-400),  Vec(),  Vec(1,1,1)*.8,  DIFF),   // clouds
          Sphere(37,  Cen+Vec(237,280,-400),  Vec(),  Vec(1,1,1)*.8,  DIFF),   // clouds
          Sphere(28,  Cen+Vec(267,280,-400),  Vec(),  Vec(1,1,1)*.8,  DIFF),   // clouds

          Sphere(40,  Cen+Vec(150,280,-1000),  Vec(),  Vec(1,1,1)*.8,  DIFF),  // clouds
          Sphere(37,  Cen+Vec(187,280,-1000),  Vec(),  Vec(1,1,1)*.8,  DIFF),  // clouds

          Sphere(40,  Cen+Vec(600,280,-1100),  Vec(),  Vec(1,1,1)*.8,  DIFF),  // clouds
          Sphere(37,  Cen+Vec(637,280,-1100),  Vec(),  Vec(1,1,1)*.8,  DIFF),  // clouds

          Sphere(37,  Cen+Vec(-800,280,-1400),  Vec(),  Vec(1,1,1)*.8,  DIFF), // clouds
          Sphere(37,  Cen+Vec(0,280,-1600),  Vec(),  Vec(1,1,1)*.8,  DIFF),    // clouds
          Sphere(37,  Cen+Vec(537,280,-1800),  Vec(),  Vec(1,1,1)*.8,  DIFF),  // clouds

        };
    }
    else if (name == "overlap") {
        double D=50;
        double R=40;
        spheres = {//Scene: radius, position, emission, color, material
          Sphere(150, Vec(50+75,28,62), Vec(1,1,1)*0e-3, Vec(1,.9,.8)*.93, REFR),
          Sphere(28,  Vec(50+5,-28,62), Vec(1,1,1)*1e1, Vec(1,1,1)*0, DIFF),
          Sphere(300, Vec(50,28,62), Vec(1,1,1)*0e-3, Vec(1,1,1)*.93, SPEC)
        };
    }
    else if (name == "wada") {
        double R=60;
        //double R=120;
        double T=30*M_PI/180.;
        double D=R/cos(T);
        double Z=60;
        spheres = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0
          Sphere(1e5, Vec(50, 100, 0),      Vec(1,1,1)*3e0, Vec(), DIFF), // sky
          Sphere(1e5, Vec(50, -1e5-D-R, 0), Vec(),     Vec(.1,.1,.1),DIFF),           //grnd

          Sphere(R, Vec(50,40.8,62)+Vec( cos(T),sin(T),0)*D, Vec(), Vec(1,.3,.3)*.999, SPEC), //red
          Sphere(R, Vec(50,40.8,62)+Vec(-cos(T),sin(T),0)*D, Vec(), Vec(.3,1,.3)*.999, SPEC), //grn
          Sphere(R, Vec(50,40.8,62)+Vec(0,-1,0)*D,         Vec(), Vec(.3,.3,1)*.999, SPEC), //blue
          Sphere(R, Vec(50,40.8,62)+Vec(0,0,-1)*D,       Vec(), Vec(.53,.53,.53)*.999, SPEC), //back
          Sphere(R, Vec(50,40.8,62)+Vec(0,0,1)*D,      Vec(), Vec(1,1,1)*.999, REFR), //front

        //   Sphere(R, Vec(50,35,Z)+Vec( cos(T),sin(T),0)*D, Vec(1,1,1)*1e-1, Vec(1,1,1)*.999, SPEC), //red
        //   Sphere(R, Vec(50,35,Z)+Vec(-cos(T),sin(T),0)*D, Vec(1,1,1)*1e-1, Vec(1,1,1)*.999, SPEC), //grn
        //   Sphere(R, Vec(50,35,Z)+Vec(0,-1,0)*D,           Vec(1,1,1)*1e-1, Vec(1,1,1)*.999, SPEC), //blue
        //   Sphere(R, Vec(50,35,Z)+Vec(0,0,-1)*D*1.6,       Vec(1,1,1)*0e-1, Vec(0.275, 0.612, 0.949)*.999, SPEC), //back
        //  Sphere(R, Vec(50,40.8,62)+Vec(0,0,1)*D*.2877,          Vec(1,1,1)*0e-1, Vec(1,1,1)*.999, REFR), //front
        };
    }
    else if (name == "wada2") {
        //double R=60;
        double R=120;     // radius
        double T=30*M_PI/180.;
        double D=R/cos(T);     //distance
        // double D=60;     //distance
        // double R=D*sqrt(2);
        double Z=62;
        Vec C=Vec(0.275, 0.612, 0.949);
        spheres = {//Scene: radius, position, emission, color, material

          Sphere(R, Vec(50,28,Z)+Vec( cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //red
          Sphere(R, Vec(50,28,Z)+Vec(-cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //grn
          Sphere(R, Vec(50,28,Z)+Vec(0,-1,0)*D,              C*6e-2,Vec(1,1,1)*.996, SPEC), //blue
          Sphere(R, Vec(50,28,Z)+Vec(0,0,-1)*R*2*sqrt(2./3.),C*0e-2,Vec(1,1,1)*.996, SPEC), //back
        //  Sphere(1e5, Vec(50,28,Z)+Vec(0,0,1e5+170),   Vec(1,1,1)*0,Vec(1,1,1)*.996, SPEC), //front
        //  Sphere(2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3., Vec(50,28,Z)+Vec(0,0,-R*2*sqrt(2./3.)/3.),   Vec(1,1,1)*0,Vec(1,1,1)*.3333, SPEC), //front
          Sphere(2*2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3., Vec(50,28,Z)+Vec(0,0,-R*2*sqrt(2./3.)/3.),   Vec(1,1,1)*0,Vec(1,1,1)*.5, SPEC), //front
        };
    }
    else if (name == "forest") {
        Vec tc(0.0588, 0.361, 0.0941);
        Vec sc = Vec(1,1,1)*.7;
        Sphere spheres[] = {//Scene: radius, position, emission, color, material
          // center 50 40.8 62
          // floor 0
          // back  0
        //  Sphere(1e5, Vec(50, 1e5+100, 0),  Vec(1,1,1)*1,Vec(),DIFF), //lite
        //  Sphere(1e5, Vec(50, -1e5, 0),  Vec(),Vec(.3,.3,.1),DIFF), //grnd
        //  Sphere(1e5, Vec(50, 1e5+100, 0),  Vec(0.761, 0.875, 1.00)*1.3,Vec(),DIFF),
        //  //lite
          Sphere(1e5, Vec(50, 1e5+130, 0),  Vec(1,1,1)*1.3,Vec(),DIFF), //lite
          Sphere(1e2, Vec(50, -1e2+2, 47),  Vec(),Vec(1,1,1)*.7,DIFF), //grnd

          Sphere(1e4, Vec(50, -30, 300)+Vec(-sin(50*M_PI/180),0,cos(50*M_PI/180))*1e4, Vec(), Vec(1,1,1)*.99,SPEC),// mirr L
          Sphere(1e4, Vec(50, -30, 300)+Vec(sin(50*M_PI/180),0,cos(50*M_PI/180))*1e4,  Vec(), Vec(1,1,1)*.99,SPEC),// mirr R
          Sphere(1e4, Vec(50, -30, -50)+Vec(-sin(30*M_PI/180),0,-cos(30*M_PI/180))*1e4,Vec(), Vec(1,1,1)*.99,SPEC),// mirr FL
          Sphere(1e4, Vec(50, -30, -50)+Vec(sin(30*M_PI/180),0,-cos(30*M_PI/180))*1e4, Vec(), Vec(1,1,1)*.99,SPEC),// mirr


          Sphere(4, Vec(50,6*.6,47),   Vec(),Vec(.13,.066,.033), DIFF),//"tree"
          Sphere(16,Vec(50,6*2+16*.6,47),   Vec(), tc,  DIFF),//"tree"
          Sphere(11,Vec(50,6*2+16*.6*2+11*.6,47),   Vec(), tc,  DIFF),//"tree"
          Sphere(7, Vec(50,6*2+16*.6*2+11*.6*2+7*.6,47),   Vec(), tc,  DIFF),//"tree"

          Sphere(15.5,Vec(50,1.8+6*2+16*.6,47),   Vec(), sc,  DIFF),//"tree"
          Sphere(10.5,Vec(50,1.8+6*2+16*.6*2+11*.6,47),   Vec(), sc,  DIFF),//"tree"
          Sphere(6.5, Vec(50,1.8+6*2+16*.6*2+11*.6*2+7*.6,47),   Vec(), sc,  DIFF),//"tree"
        };
    }
}

inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }

inline int toInt(double x){ return int(pow(clamp(x),1/2.2)*255+.5); }

inline bool intersect(const Ray &r, double &t, int &id) {
    double d, inf=t=1e20;
    for(int i=int(spheres.size());i--;) {
        if((d=spheres[i].intersect(r))&&d<t){
            t=d;
            id=i;
        }
    }
    return t<inf;
}

std::atomic<int> max_depth{0};

Vec radiance(const Ray &r, int depth, unsigned short *Xi){
    double t;                               // distance to intersection
    int id=0;                               // id of intersected object
    if (!intersect(r, t, id)) {
        return Vec();  // if miss, return black
    }
    const Sphere &obj = spheres[id];        // the hit object
    Vec x=r.o+r.d*t, n=(x-obj.p).norm(), nl=n.dot(r.d)<0?n:n*-1, f=obj.c;
    double p = f.x>f.y && f.x>f.z ? f.x : f.y>f.z ? f.y : f.z; // max refl
    if (++depth>5) {
        if (erand48(Xi)<p){
            f=f*(1/p);
        }
        else{
            return obj.e; //R.R.
        }
    }
#if 1
    // RALLEN this isn't a good CAS, but is "good enough"
    if(depth > max_depth) {
        max_depth = depth;
//        printf("max_depth = %d\n",max_depth.load());
    }
#endif
#if 0
    if(depth > 500) {
        return obj.e; // RALLEN early exit
    }
#endif
    if (obj.refl == DIFF) {
        // Ideal DIFFUSE reflection
        double r1=2*M_PI*erand48(Xi), r2=erand48(Xi), r2s=sqrt(r2);
        Vec w=nl, u=((fabs(w.x)>.1?Vec(0,1):Vec(1))%w).norm(), v=w%u;
        Vec d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).norm();
        return obj.e + f.mult(radiance(Ray(x,d),depth,Xi));
    } else if (obj.refl == SPEC) {
        // Ideal SPECULAR reflection
        return obj.e + f.mult(radiance(Ray(x,r.d-n*2*n.dot(r.d)),depth,Xi));
    }
    Ray reflRay(x, r.d-n*2*n.dot(r.d));     // Ideal dielectric REFRACTION
    bool into = n.dot(nl)>0;                // Ray from outside going in?
    double nc=1, nt=1.5, nnt=into?nc/nt:nt/nc, ddn=r.d.dot(nl), cos2t;
    if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0) {    // Total internal reflection
        return obj.e + f.mult(radiance(reflRay,depth,Xi));
    }
    Vec tdir = (r.d*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
    double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
    double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
    return obj.e + f.mult(depth>2 ? (erand48(Xi)<P ?   // Russian roulette
                                     radiance(reflRay,depth,Xi)*RP:radiance(Ray(x,tdir),depth,Xi)*TP) :
                          radiance(reflRay,depth,Xi)*Re+radiance(Ray(x,tdir),depth,Xi)*Tr);
}

void render(int thread_num, int num_threads,
            int w, int h, int samps,
            Ray cam,
            Vec cx, Vec cy, Vec r, Vec *c
    ) {
    int y_height = h/num_threads;
    int y0 = thread_num*y_height;
    int y1 = y0 + y_height;
    if(thread_num == num_threads - 1) {
        y1 = h; // last thread might have a bit extra due to rounding
    }
    for (int y=y0; y<y1; y++) {                       // Loop over image rows
        //fprintf(stderr,"%d Rendering (%d spp) %5.2f%%\n",thread_num,samps*4,100.*(y-y0)/(y1-y0));
        //fprintf(stderr,"%d y=%d\n",thread_num,y); fflush(stderr);
        for (unsigned short x=0, Xi[3]={0,0,static_cast<unsigned short>(y*y*y)}; x<w; x++) {   // Loop cols
            //fprintf(stderr,"%d x=%d\n",thread_num,x); fflush(stderr);
            for (int sy=0, i=(h-y-1)*w+x; sy<2; sy++) {     // 2x2 subpixel rows
                //fprintf(stderr,"%d sy=%d\n",thread_num,sy); fflush(stderr);
                for (int sx=0; sx<2; sx++, r=Vec()) {        // 2x2 subpixel cols
                    //fprintf(stderr,"%d sx=%d\n",thread_num,sy); fflush(stderr);
                    for (int s=0; s<(samps/4); s++) {
                        double r1=2*erand48(Xi), dx=r1<1 ? sqrt(r1)-1: 1-sqrt(2-r1);
                        double r2=2*erand48(Xi), dy=r2<1 ? sqrt(r2)-1: 1-sqrt(2-r2);
                        Vec d = cx*( ( (sx+.5 + dx)/2 + x)/w - .5) +
                                                       cy*( ( (sy+.5 + dy)/2 + y)/h - .5) + cam.d;
                        r = r + radiance(Ray(cam.o+d*140,d.norm()),0,Xi)*(1./samps);
                    } // Camera rays are pushed ^^^^^ forward to start in interior
                    c[i] = c[i] + Vec(clamp(r.x),clamp(r.y),clamp(r.z))*.25;
                }
            }
        }
    }
    fprintf(stderr,"%d done.\n",thread_num);
}

int main(int argc, char *argv[]){
    int w=1024, h=768, samps = 1; // # samples

    unsigned int num_threads = std::thread::hardware_concurrency();
    std::string output = "image.ppm";

    for (int i = 0; i < (argc-1); ++i) {
        if (std::string(argv[i])=="-scene") set_scene(argv[++i]);
        else if (std::string(argv[i])=="-threads") num_threads = atoi(argv[++i]);
        else if (std::string(argv[i])=="-samples") samps = atoi(argv[++i]);
        else if (std::string(argv[i])=="-width") w = atoi(argv[++i]);
        else if (std::string(argv[i])=="-height") h = atoi(argv[++i]);
        else if (std::string(argv[i])=="-output") output = argv[++i];
    }
    Ray cam(Vec(50,52,295.6), Vec(0,-0.042612,-1).norm()); // cam pos, dir
    Vec cx=Vec(w*.5135/h), cy=(cx%cam.d).norm()*.5135, r, *c=new Vec[w*h];
    printf("num_threads = %d\n",num_threads);
   // printf("scene has %d objects",int(spheres.size()));


    std::vector<std::thread> threads;

    auto start = std::chrono::steady_clock::now();
    // Launch a group of threads
    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(render,i,num_threads,w,h,samps,cam,cx,cy,r,c));
    }
    // Join the threads with the main thread
    for(auto &t : threads) {
        t.join();
    }
    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = stop-start;
    std::cout << "Execution time: " << elapsed_seconds.count() << "s\n";

    fprintf(stderr,"\n");



    FILE *f = fopen(output.c_str(), "w");         // Write image to PPM file.
    fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
    for (int i=0; i<w*h; i++)
        fprintf(f,"%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
}
