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
    glUseProgram(0);
    ::glEnable(GL_LIGHTING);
    dfm2::opengl::myGlColorDiffuse( dfm2::CColor::Gray(0.8) );
    ::glColor3d(1,0,0);
    //    delfem2::opengl::DrawMeshTri3D_Edge(aXYZ_Contact.data(), aXYZ_Contact.size()/3,
    //                                        aTri_Contact.data(), aTri_Contact.size()/3);
    ::glEnable(GL_LIGHTING);
    delfem2::opengl::DrawMeshTri3D_FaceNorm(aXYZ_Contact.data(),
                                            aTri_Contact.data(), aTri_Contact.size()/3);//body
    ::glDisable(GL_LIGHTING);
    ::glColor3d(0,0,0);
    //    delfem2::opengl::DrawMeshDynTri3D_Edge(aXYZ, aETri);
    delfem2::opengl::DrawMeshDynTri3D_Edge(aXYZ, aETri);//edge of cloth
    ::glEnable(GL_LIGHTING);
    dfm2::opengl::myGlColorDiffuse( dfm2::CColor::Red() );
    delfem2::opengl::DrawMeshDynTri_FaceNorm(aETri, aXYZ.data());//cloth
}

class Cloth:public Object
{
public:
    Cloth():shader("../shader/basic/shader.vs","../shader/basic/shader.gs", "../shader/basic/shader.fs")
    {
        Scale(2);
        Translate(glm::vec3(0,2,2));
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

        InitBodyMesh();
        InitClothMesh();

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

        //::glEnable(GL_NORMALIZE);

        //Draw(aETri, aXYZ,projector.aXYZ_Body,projector.aTri_Body);
        RenderBody(cam,light);
        RenderCloth(cam,light);

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


    GLuint BodyVAO;
    GLuint BodyVBO;
    GLuint BodyEBO;
    GLuint ClothVAO;
    GLuint ClothVBO;
    GLuint ClothEBO;

    Shader shader;

    void InitBodyMesh()
    {
        glGenVertexArrays(1,&BodyVAO);
        glGenBuffers(1,&BodyVBO);
        glGenBuffers(1,&BodyEBO);
        glBindVertexArray(BodyVAO);

        glBindBuffer(GL_ARRAY_BUFFER,BodyVBO);

        glBufferData(GL_ARRAY_BUFFER, projector.aXYZ_Body.size()*sizeof(double), projector.aXYZ_Body.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BodyEBO);
        projector.aTri_Body.data();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, projector.aTri_Body.size()*sizeof(unsigned int), projector.aTri_Body.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    }
    void InitClothMesh()
    {
        glGenVertexArrays(1,&ClothVAO);
        glGenBuffers(1,&ClothVBO);
        glGenBuffers(1,&ClothEBO);
        glBindVertexArray(ClothVAO);

        glBindBuffer(GL_ARRAY_BUFFER,ClothVBO);

        glBufferData(GL_ARRAY_BUFFER, aXYZ.size()*sizeof(double), aXYZ.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ClothEBO);

        unsigned int* index=new unsigned int[aETri.size()*3];
        for(int i=0;i<aETri.size();i++)
        {
            for(int j=0;j<3;j++)
            {
                index[i*3+j]=aETri[i].v[j];
            }
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, aETri.size()*3*sizeof(unsigned int), index, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (GLvoid*)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

        glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
        delete [] index;
    }

    void RenderBody(Camera* cam, Light* light)
    {
        shader.use();
        glBindVertexArray(BodyVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BodyEBO);
        shader.setM4("model",GetModelMatrix());
        cam->SendToGPU(&shader);
        shader.set3Float("objectColor",0.5,0.5,0.5);
        shader.set3Float("lightPos",cam->Position);


        glDrawElements(GL_TRIANGLES, (GLsizei)projector.aTri_Body.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void RenderCloth(Camera* cam, Light* light)
    {
        shader.use();
        glBindVertexArray(ClothVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ClothEBO);


        glBindBuffer(GL_ARRAY_BUFFER,ClothVBO);
        glBufferData(GL_ARRAY_BUFFER, aXYZ.size()*sizeof(double), aXYZ.data(), GL_DYNAMIC_DRAW);

        shader.setM4("model",GetModelMatrix());
        cam->SendToGPU(&shader);
        shader.set3Float("objectColor",0.9,0.1,0.1);
        shader.set3Float("lightPos",cam->Position);


        glDrawElements(GL_TRIANGLES, (GLsizei)aETri.size()*3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};

#endif //PHYSICALENGINE_CLOTH_H
