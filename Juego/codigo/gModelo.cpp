#include <allegro.h>
#include <alleggl.h>

#include <cmath>

#include "gModelo.h"
#include "gApi.h"
#include "gObjeto3D.h"
#include "eExcepcionLocalizada.h"



namespace Graficos
{   
InterleavedElemenent Modelo::elementoArray[50000];
    
Modelo::Modelo(string nombreFichero, Imagen *textura, float r, float g, float b):
    deFichero(false)
{
    try
    {
        transparente = false ;
        
        colorBando[0] = r ;
        colorBando[1] = g ;
        colorBando[2] = b ;
        
        boundingBoxMin = Util::Vector3D(-1,-1,-1);
        boundingBoxMax = Util::Vector3D(1,1,1);
        if ( nombreFichero == "" )
        {
            // Caso especial, creamos un cubo para testing
            callList = Api::ReservarCallList();
    
            glNewList(callList, GL_COMPILE);
                glDisable(GL_TEXTURE_2D);
                glBegin(GL_QUADS);                
                    glColor4f(1,1,1,1);
    
                    glNormal3f( 0, -1,  0);
                    glVertex3f(-1,  0, -1);
                    glVertex3f(-1,  0,  1);
                    glVertex3f( 1,  0,  1);
                    glVertex3f( 1,  0, -1);
                    
                    glNormal3f( 0,  1,  0);
                    glVertex3f(-1,  3, -1);
                    glVertex3f(-1,  3,  1);
                    glVertex3f( 1,  3,  1);
                    glVertex3f( 1,  3, -1);
    
                    glNormal3f( 1,  0,  0);
                    glVertex3f( 1,  0, -1);
                    glVertex3f( 1,  0,  1);
                    glVertex3f( 1,  3,  1);
                    glVertex3f( 1,  3, -1);
                    
                    glNormal3f(-1,  0,  0);
                    glVertex3f(-1,  0, -1);
                    glVertex3f(-1,  0,  1);
                    glVertex3f(-1,  3,  1);
                    glVertex3f(-1,  3, -1);
                    
                    glNormal3f( 0,  0,  1);
                    glVertex3f(-1,  0,  1);
                    glVertex3f(-1,  3,  1);
                    glVertex3f( 1,  3,  1);
                    glVertex3f( 1,  0,  1);
                    
                    glNormal3f( 0,  0, -1);
                    glVertex3f(-1,  0, -1);
                    glVertex3f(-1,  3, -1);
                    glVertex3f( 1,  3, -1);
                    glVertex3f( 1,  0, -1);
                glEnd();
                glColor4f(1,1,1,1);
                //glEnable(GL_TEXTURE_2D);
            glEndList();
            
            // Generamos el boundingBox
            boundingBoxMin = Util::Vector3D(-1, 0,-1);
            boundingBoxMax = Util::Vector3D( 1, 3, 1);
            
            file = 0 ;
        }
        else
        {
            glPushMatrix();
            callList = -1;
    
            file=NULL;
            file=lib3ds_file_load(nombreFichero.c_str());
            if (!file)
            {
                throw Error::ExcepcionLocalizada("Graficos::Modelo::Modelo(string, string, float, float, float)");
            }
            lib3ds_file_eval(file,0);
            
            /*
            textura = NULL ;
            if ( ficheroTextura != "" )
            {
                textura = new Imagen(ficheroTextura);
            }
            */
            Modelo::textura = textura ;
            
            // Generamos el boundingBox
            boundingBoxMin = Util::Vector3D(-1, 0,-1);
            boundingBoxMax = Util::Vector3D( 1, 3, 1);
            glPopMatrix();
            
            
            {
            elementoInicio = 0 ;
            elementoCuenta = 0 ;
            
            glPushMatrix();

                glLoadIdentity();

                glRotatef(90, 0, 1, 0);
                glRotatef(-90, 1, 0, 0);
                glScalef(0.005, 0.005, 0.005); 
                
                                
                Lib3dsNode *c,*t;
                Lib3dsMatrix M;
                
                Lib3dsNode *p;
                for (p=file->nodes; p!=0; p=p->next) 
                {
                    renderNode(p);
                }
            glPopMatrix();
            
            callList = Api::ReservarCallListAlta();
            deFichero = true ; 

            glInterleavedArrays(GL_T2F_C4F_N3F_V3F, 0, elementoArray); 
            //glInterleavedArrays(GL_T2F_N3F_V3F, 0, elementoArray); 

            glNewList(callList, GL_COMPILE);
                glColor4f(1,1,1,1);
                if ( textura )
                {
                    glBindTexture(GL_TEXTURE_2D, textura->GetTextura());
                }
                else
                {
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
                glDrawArrays(GL_TRIANGLES, elementoInicio, elementoCuenta);            
            glEndList();

            // Calculamos los bounding boxs
            boundingBoxMin = Util::Vector3D(elementoArray[0].vertex[0], elementoArray[0].vertex[1], elementoArray[0].vertex[2]);
            boundingBoxMax = boundingBoxMin ;
            for ( int i = 1 ; i < elementoCuenta ; i++ )
            {
                if ( elementoArray[i].vertex[0] < boundingBoxMin.GetX() )
                {
                    boundingBoxMin.SetX(elementoArray[i].vertex[0]);
                }
                if ( elementoArray[i].vertex[0] > boundingBoxMax.GetX() )
                {
                    boundingBoxMax.SetX(elementoArray[i].vertex[0]);
                }

                if ( elementoArray[i].vertex[1] < boundingBoxMin.GetY() )
                {
                    boundingBoxMin.SetY(elementoArray[i].vertex[1]);
                }
                if ( elementoArray[i].vertex[1] > boundingBoxMax.GetY() )
                {
                    boundingBoxMax.SetY(elementoArray[i].vertex[1]);
                }

                if ( elementoArray[i].vertex[2] < boundingBoxMin.GetZ() )
                {
                    boundingBoxMin.SetZ(elementoArray[i].vertex[2]);
                }
                if ( elementoArray[i].vertex[2] > boundingBoxMax.GetZ() )
                {
                    boundingBoxMax.SetZ(elementoArray[i].vertex[2]);
                }
            }
            
            lib3ds_file_free(file);
            file = NULL ;

            //glNewList(deFichero, GL_COMPILE);
            //    glDrawArrays(GL_TRIANGLES, elementoInicio, elementoCuenta);            
            //glEndList();           

            /*
            //glDisable(GL_TEXTURE_2D);
            callList = Api::ReservarCallListAlta();
            deFichero = true ;
            //glNewList(callList, GL_COMPILE);
            //glEndList();
            
            //callList = glGenLists(1);
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,0);
            glDisable(GL_LIGHTING);
            glDisable(GL_BLEND);
            glDisable(GL_NORMALIZE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_ALPHA_TEST);
            glColor4f(1,1,1,1);
            
            glPushMatrix();
            
            glRotatef(90, 0, 1, 0);
            glRotatef(-90, 1, 0, 0);
            glScalef(0.005, 0.005, 0.005);   
            
                     

            Lib3dsNode *c,*t;
            Lib3dsMatrix M;
            
            glNewList(callList, GL_COMPILE);
            glBegin(GL_TRIANGLES);
            Lib3dsNode *p;
            for (p=file->nodes; p!=0; p=p->next) 
            {
                renderNode(p);
            }
            glEnd();
            glEndList();
            
            glPopMatrix();
            
            glEnable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            glColor4f(1,1,1,1);  
            */                 
                
                
            }
        }    
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Modelo::Modelo(string)").Combinar(ex);
    }
}    
    
Modelo::Modelo(int callList):
    deFichero(false)
{
    Modelo::callList = callList ;
    file = 0 ;
}

Modelo::~Modelo()
{
    try
    {
        Api::LiberarCallList(callList);
        callList = -1 ;
        if ( file )
        {
            lib3ds_file_free(file);
        }
        /*
        if ( textura )
        {
            delete textura ;
        }
        */
    }
    catch ( Error::Excepcion &ex )
    {
        Error::ExcepcionLocalizada("Graficos::Modelo::~Modelo()").Combinar(ex);
    }
}

void
Modelo::SetBoundingBox(Util::Vector3D bmin, Util::Vector3D bmax)
{
    boundingBoxMin = bmin ;
    boundingBoxMax = bmax ;
}

void
Modelo::GetBoundingBox(Util::Vector3D &bmin, Util::Vector3D &bmax)
{
    bmin = boundingBoxMin ;
    bmax = boundingBoxMax ;   
}

void
Modelo::Pintar()
{
    try
    {
        //glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D,0);
        //glEnable(GL_LIGHTING);
        
        
        glPushMatrix();
        
        if ( callList != -1 )
        {
            
            //glPushMatrix();
            //glPushAttrib(GL_ALL_ATTRIB_BITS);
            
            if (deFichero)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,0);
                if ( transparente )
                {
                    glEnable(GL_BLEND);
                }
                else
                {
                    glDisable(GL_BLEND);
                }
                glDisable(GL_NORMALIZE);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDisable(GL_ALPHA_TEST);
                glColor4f(1,1,1,1);                
                
                glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
                
                //glRotatef(90, 0, 1, 0);
                //glRotatef(-90, 1, 0, 0);
                //glScalef(0.005, 0.005, 0.005); 
                
                glColor4f(1,1,1,1);  
                //glDisable(GL_LIGHTING);
                
            }
            
            //int tiempoCallList = GetTickCount();            
            if ( !deFichero)
            {
                glCallList(callList);
            }
            else
            {
                glCallList(callList);
                //glDrawArrays(GL_TRIANGLES, elementoInicio, elementoCuenta); 
            }
            //cout << " glCallList(): " << GetTickCount() - tiempoCallList << endl ;
                        
            if ( deFichero )
            {
                
                glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);    
                
                GLfloat ambient[]={0.2,0.2,0.2,1};
                GLfloat diffuse[]={1,1,1,1} ;
                GLfloat specular[]={0,0,0,0} ;
                
                float s;
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
            
                
                glDisable(GL_BLEND);
                glColor4f(1,1,1,1);
                glEnable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
                glBindTexture(GL_TEXTURE_2D,0);     
                           
            }
            
            //glPopAttrib();
            //glPopMatrix();

        }
        else
        { 


                    
        }
        
        glPopMatrix();
        
        GLfloat ambient[]={0.2,0.2,0.2,1};
        GLfloat diffuse[]={1,1,1,1} ;
        GLfloat specular[]={0,0,0,0} ;
        
        float s;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
    
        
        glDisable(GL_BLEND);
        glColor4f(1,1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,0);
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Modelo::Pintar()").Combinar(ex);
    }
}



void
Modelo::renderNode(Lib3dsNode *node)
{
    
    ASSERT(file);

    {
        Lib3dsNode *p;
        for (p=node->childs; p!=0; p=p->next) 
        {
            glPushMatrix();
            renderNode(p);
            glPopMatrix();
        }
    }
    
    if (node->type==LIB3DS_OBJECT_NODE) 
    {
        if (strcmp(node->name,"$$$DUMMY")==0) 
        {
            return;
        }
        
        
        //if (!node->user.d) 
        //{
            Lib3dsMesh *mesh=lib3ds_file_mesh_by_name(file, node->name);
            ASSERT(mesh);
            if (!mesh) 
            {
                return;
            }

            //node->user.d=Api::ReservarCallListAlta();
            //glNewList(node->user.d, GL_COMPILE);
            
            //glPushMatrix();
            Lib3dsObjectData *d;
        
            d=&node->data.object;            
            glMultMatrixf(&node->matrix[0][0]);
            glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);

            {

                unsigned p;
                Lib3dsVector *normalL=(Lib3dsVector *)malloc(3*sizeof(Lib3dsVector)*mesh->faces);
 
                {
                    Lib3dsMatrix M;
                    lib3ds_matrix_copy(M, mesh->matrix);
                    lib3ds_matrix_inv(M);
                    glMultMatrixf(&M[0][0]);
                }
                lib3ds_mesh_calculate_normals(mesh, normalL);
    
                //node->user.d=Api::ReservarCallListAlta();
                //glNewList(node->user.d, GL_COMPILE);
                
                for (p=0; p<mesh->faces; ++p) 
                {
                    
                    Lib3dsFace *f=&mesh->faceL[p];
                    Lib3dsMaterial *mat=0;
                    
                    if (f->material[0]) 
                    {
                        mat=lib3ds_file_material_by_name(file, f->material);
                    }
                    

                    
    
                    
                    if (mat) 
                    {
                                       
                        static GLfloat a[4]={0.2,0.2,0.2,1};
                        GLfloat diffuse[4] ;
                        GLfloat specular[4] ;
                        diffuse[0] = mat->diffuse[0] ;
                        diffuse[1] = mat->diffuse[1] ;
                        diffuse[2] = mat->diffuse[2] ;
                        diffuse[3] = mat->diffuse[3] ;
                        
                        specular[0] = mat->specular[0] / 255.0 ;
                        specular[1] = mat->specular[1] / 255.0 ;
                        specular[2] = mat->specular[2] / 255.0 ;
                        specular[3] = mat->specular[3] / 255.0 ;                        
                        
                        glColor4f(diffuse[0],diffuse[1],diffuse[2],diffuse[3]);
                        //glColor4f(1,1,1,1);
                        Lib3dsRgba sp={0.0, 0.0, 0.0, 1.0};
                        float s;
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                        s = pow(2, 10.0*mat->shininess);
                        if (s>128.0) 
                        {
                            s=128.0;
                        }
                        s=0.0;
                        //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, s);
                        
                    }
                    else 
                    {
                        
                        Lib3dsRgba a={0.2, 0.2, 0.2, 1.0};
                        Lib3dsRgba d={0.8, 0.8, 0.8, 1.0};
                        Lib3dsRgba s={0.0, 0.0, 0.0, 1.0};
                        glColor4f(1,1,1,1);
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
                        //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
                        
                    }
                    
    
                    
                    {
                      	//glBegin(GL_TRIANGLES);
                        int i;
                        
                        
                            
                            //glNormal3fv(f->normal);
                            
                            for (i=0; i<3; ++i) 
                            {                                
                                //glNormal3fv(normalL[3*p+i]);
                                //glVertex3fv(mesh->pointL[f->points[i]].pos);
                                
                                float mx = mesh->pointL[f->points[i]].pos[0] ;
                                float my = mesh->pointL[f->points[i]].pos[1] ;
                                float mz = mesh->pointL[f->points[i]].pos[2] ;
                                
                                float mv[16] ;
                                glGetFloatv(GL_MODELVIEW_MATRIX, mv);
                                
                                float x = mv[0]*mx + mv[4]*my + mv[8]*mz + mv[12]; 
                                float y = mv[1]*mx + mv[5]*my + mv[9]*mz + mv[13];
                                float z = mv[2]*mx + mv[6]*my + mv[10]*mz + mv[14];
                                
                                float color[4] ;
                                glGetFloatv(GL_CURRENT_COLOR, color);
                                
                                // Textura
                                
                                elementoArray[elementoInicio+elementoCuenta].texture[0] = mesh->texelL[f->points[i]][0] ;
                                elementoArray[elementoInicio+elementoCuenta].texture[1] = mesh->texelL[f->points[i]][1] ;
                                
                                
                                /*
                                elementoArray[elementoInicio+elementoCuenta].texture[0] = 0.0 ;
                                elementoArray[elementoInicio+elementoCuenta].texture[1] = 0.0 ;
                                */
                                
                                // Color
                                if ((color[0] == 1.0 &&
                                     color[1] == 0.0 &&
                                     color[2] == 1.0) )
                                {
                                    elementoArray[elementoInicio+elementoCuenta].color[0] = colorBando[0] ;
                                    elementoArray[elementoInicio+elementoCuenta].color[1] = colorBando[1] ;
                                    elementoArray[elementoInicio+elementoCuenta].color[2] = colorBando[2] ; 
                                    elementoArray[elementoInicio+elementoCuenta].color[3] = 1.0 ;                                                                                               
                                }
                                else
                                {
                                    elementoArray[elementoInicio+elementoCuenta].color[0] = 1.0 ;
                                    elementoArray[elementoInicio+elementoCuenta].color[1] = 1.0 ;
                                    elementoArray[elementoInicio+elementoCuenta].color[2] = 1.0 ; 
                                    elementoArray[elementoInicio+elementoCuenta].color[3] = 1.0 ;
                                }                                    
                                
                                // Normal
                                elementoArray[elementoInicio+elementoCuenta].normal[0] = normalL[3*p+i][0] ;
                                elementoArray[elementoInicio+elementoCuenta].normal[1] = normalL[3*p+i][1] ;
                                elementoArray[elementoInicio+elementoCuenta].normal[2] = normalL[3*p+i][2] ;                                                                
                                
                                // Vertice
                                elementoArray[elementoInicio+elementoCuenta].vertex[0] = x ;
                                elementoArray[elementoInicio+elementoCuenta].vertex[1] = y ;
                                elementoArray[elementoInicio+elementoCuenta].vertex[2] = z ;
                                elementoCuenta++ ;

                                //cout << elementoCuenta << endl ;
                                
                                                                
                                /*
                                if ( mesh->pointL[f->points[i]].pos[0] < boundingBoxMin.GetX() )
                                {
                                    boundingBoxMin.SetX(mesh->pointL[f->points[i]].pos[0]);
                                }
                                if ( mesh->pointL[f->points[i]].pos[1] < boundingBoxMin.GetY() )
                                {
                                    boundingBoxMin.SetY(mesh->pointL[f->points[i]].pos[1]);
                                }
                                if ( mesh->pointL[f->points[i]].pos[2] < boundingBoxMin.GetZ() )
                                {
                                    boundingBoxMin.SetZ(mesh->pointL[f->points[i]].pos[2]);
                                }
                                
                                if ( mesh->pointL[f->points[i]].pos[0] > boundingBoxMax.GetX() )
                                {
                                    boundingBoxMax.SetX(mesh->pointL[f->points[i]].pos[0]);
                                }
                                if ( mesh->pointL[f->points[i]].pos[1] > boundingBoxMax.GetY() )
                                {
                                    boundingBoxMax.SetY(mesh->pointL[f->points[i]].pos[1]);
                                }
                                if ( mesh->pointL[f->points[i]].pos[2] > boundingBoxMax.GetZ() )
                                {
                                    boundingBoxMax.SetZ(mesh->pointL[f->points[i]].pos[2]);
                                }
                                */
                            }
                            
                        //glEnd();
                       	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
                    }
                    
                }
    
                free(normalL);
                
            }
            //glEndList();
        //}

        if (node->user.d) 
        {
            /*
            Lib3dsObjectData *d;
        
            glPushMatrix();
            d=&node->data.object;
            glMultMatrixf(&node->matrix[0][0]);
            glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);
            glCallList(node->user.d);
            glPopMatrix();
            */
            
        }
    }
}

void
Modelo::PintarBoundingBox()
{
    glDisable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);

        // -YZ
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        
        // +YZ
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        
        // X-Z
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        
        // X+Z
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        
        // XY-
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMin.GetZ());
        
        // XY+
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMin.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMax.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
        glVertex3f(boundingBoxMin.GetX(), boundingBoxMax.GetY(), boundingBoxMax.GetZ());
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

Modelo *
Modelo::CrearQuad(Imagen *imagen, float r, float g, float b, float a)
{
    try
    {
        Modelo *modelo = new Modelo("");
        
        
        Api::LiberarCallList(modelo->callList);
        modelo->callList = Api::ReservarCallList();
    
        glNewList(modelo->callList, GL_COMPILE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, imagen->GetTextura());
            glDisable(GL_LIGHTING);
            
            glColor4f(r,g,b,a);
    
            glBegin(GL_QUADS);
                glNormal3f( 0,  1,  0);
    
                glTexCoord2f(0, 0);
                glVertex3f(-1,  0, -1);
    
                glTexCoord2f(0, 1);
                glVertex3f(-1,  0,  1);
    
                glTexCoord2f(1, 1);
                glVertex3f( 1,  0,  1);
    
                glTexCoord2f(1, 0);
                glVertex3f( 1,  0, -1);
            glEnd();
    
            glColor4f(1,1,1,1);
            glEnable(GL_DEPTH_TEST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);
        glEndList();
        
        modelo->file = 0 ; 
        modelo->boundingBoxMin = Util::Vector3D(-1, 0,-1) ;
        modelo->boundingBoxMax = Util::Vector3D( 1, 1, 1) ;
        
        
        
        return modelo ;  
    }
    catch ( Error::Excepcion &ex )
    {
        throw Error::ExcepcionLocalizada("Graficos::Modelo::CrearQuad(Imagen *, float, float, float, float)").Combinar(ex);
    }
}

float
Modelo::GetAltura()
{
    return boundingBoxMax.GetY();
}

float
Modelo::GetRadio()
{
    float a, b, c, d, radio ;
    a =  boundingBoxMax.GetX() ;
    b = -boundingBoxMin.GetX() ;
    c =  boundingBoxMax.GetZ() ;
    d = -boundingBoxMin.GetZ() ;
    radio = a>b?a:b ;
    radio = radio>c?radio:c;
    radio = radio>d?radio:d;
    return radio ;
}

}
