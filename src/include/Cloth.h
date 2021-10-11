//
// Created by wuzaiqiang on 21-9-27.
//

#ifndef PHYSICALENGINE_CLOTH_H
#define PHYSICALENGINE_CLOTH_H

#include "shader.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Object.h"
#include "Light.h"

#include <iostream>
#include <vector>
#if defined(_WIN32) // windows
#  define NOMINMAX   // to remove min,max macro
#  include <windows.h>
#endif
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "delfem2/cnpy/smpl_cnpy.h"
#include "delfem2/geo3_v23m34q.h"
#include "delfem2/garment.h"
#include "delfem2/cad2_dtri2.h"
#include "delfem2/cad2_io_svg.h"
#include "delfem2/rig_geo3.h"
#include "inputs_garment.h"
#include "delfem2/mshmisc.h"
#include "delfem2/points.h"
#include "delfem2/dtri.h"
#include "delfem2/color.h"
#include "delfem2/glfw/viewer3.h"
#include "delfem2/glfw/util.h"
#include "delfem2/opengl/old/cad2dtriv2.h"
#include "delfem2/opengl/old/caddtri_v3.h"
#include "delfem2/opengl/old/mshuni.h"
#include "delfem2/opengl/old/funcs.h"
#include "delfem2/opengl/old/color.h"


// ----------------------------

#ifndef M_PI
#  define M_PI 3.14159265359
#endif

namespace dfm2=delfem2;

// physics parameter
const double dt = 0.01;
const double gravity[3] = {0.0, -0.1, 0.0};
const double bend_stiff_ratio = 0.1;


void Draw(
        const std::vector<dfm2::CDynTri>& aETri,
        const std::vector<double>& aXYZ,
        std::vector<double>& aXYZ_Contact,
        std::vector<unsigned int>& aTri_Contact)
{
    //    myGlutDisplay(aETri,aVec2);
    /*
    ::glDisable(GL_LIGHTING);
    for( auto& rt : aRT23 ){
      ::glPointSize(10);
      ::glColor3d(0,0,0);
      ::glBegin(GL_POINTS);
      dfm2::CVec3d v = rt.org3;
      ::glVertex3dv(v.p);
      ::glEnd();
    }
     */
    ::glEnable(GL_LIGHTING);
    dfm2::opengl::myGlColorDiffuse( dfm2::CColor::Gray(0.8) );
    ::glColor3d(1,0,0);
    //    delfem2::opengl::DrawMeshTri3D_Edge(aXYZ_Contact.data(), aXYZ_Contact.size()/3,
    //                                        aTri_Contact.data(), aTri_Contact.size()/3);
    ::glEnable(GL_LIGHTING);
    delfem2::opengl::DrawMeshTri3D_FaceNorm(aXYZ_Contact.data(),
                                            aTri_Contact.data(), aTri_Contact.size()/3);
    ::glDisable(GL_LIGHTING);
    ::glColor3d(0,0,0);
    //    delfem2::opengl::DrawMeshDynTri3D_Edge(aXYZ, aETri);
    delfem2::opengl::DrawMeshDynTri3D_Edge(aXYZ, aETri);
    ::glEnable(GL_LIGHTING);
    dfm2::opengl::myGlColorDiffuse( dfm2::CColor::Red() );
    delfem2::opengl::DrawMeshDynTri_FaceNorm(aETri, aXYZ.data());
}

class Cloth:public Object
{
public:
    Cloth()
    {
        dfm2::cnpy::LoadSmpl_Bone(
                projector.aXYZ_Body,
                aSkinningWeight,
                projector.aTri_Body,
                aIndBoneParent,
                aJntRgrs0,
                std::string("/home/wuzaiqiang/workspace/delfem2/test_inputs")+"/smpl_model_f.npz");
        dfm2::Points3_WeighttranspPosition(
                aJntPos0,
                aJntRgrs0, projector.aXYZ_Body);
        dfm2::InitBones_JointPosition(
                aBone,
                aBone.size(), aIndBoneParent.data(), aJntPos0.data());
        dfm2::UpdateBoneRotTrans(aBone);
        projector.Init();

        dfm2::Inputs_SmplTshirt(
                name_cad_in_test_input,
                scale_adjust,
                aIESeam,
                mesher_edge_length,
                aRT23);

        std::string path_svg = std::string("/home/wuzaiqiang/workspace/delfem2/test_inputs")+"/"+name_cad_in_test_input;
        std::cout << "open svg: " << path_svg << std::endl;
        dfm2::ReadSVG_Cad2D(
                cad, path_svg, 0.001*scale_adjust);
        dfm2::CMesher_Cad2D mesher;
        dfm2::MeshingPattern(
                aETri,aVec2,aXYZ,aLine,mesher,
                aRT23,cad,aIESeam,mesher_edge_length);

    }
    void Render(Camera* cam, Light* light, GLuint depthMap)
    {
        std::vector<double> aXYZt = aXYZ;
        std::vector<double> aUVW(aXYZ.size(), 0.0);
        std::vector<int> aBCFlag(aXYZ.size()/3, 0.0);
        dfm2::StepTime_PbdClothSim(
                aXYZ, aXYZt, aUVW, aBCFlag,
                aETri, aVec2, aLine,
                projector,
                dt, gravity, bend_stiff_ratio);
        damper.Damp(aUVW);
        // ------------
        //viewer.DrawBegin_oldGL();
        //    dfm2::opengl::Draw_CCad2D(cad);
        ::glEnable(GL_NORMALIZE);
        Draw(aETri, aXYZ,
             projector.aXYZ_Body,
             projector.aTri_Body);

    }
    void Step()
    {

    }
private:
    dfm2::CProjectorMesh projector;
    std::vector<dfm2::CRigBone> aBone;
    std::vector<unsigned int> aIndBoneParent;
    std::vector<double> aJntRgrs0;
    std::vector<double> aSkinningWeight;
    std::vector<double> aJntPos0;

    std::vector<dfm2::CDynTri> aETri;
    std::vector<dfm2::CVec2d> aVec2;
    std::vector<double> aXYZ; // deformed vertex positions
    std::vector<unsigned int> aLine;

    delfem2::CCad2D cad;
    std::string name_cad_in_test_input;
    double scale_adjust = 0.0;
    std::vector<unsigned int> aIESeam;
    double mesher_edge_length;
    std::vector<dfm2::CRigidTrans_2DTo3D> aRT23;

    dfm2::CKineticDamper damper;

};

#endif //PHYSICALENGINE_CLOTH_H