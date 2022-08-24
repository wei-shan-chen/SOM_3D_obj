
#include "OBJmodel.h"

OBJmodel::OBJmodel(){

}
OBJmodel::~OBJmodel(){

}
bool OBJmodel::LoadOBJfile(const char *objFileName){

    FILE *file = NULL;
    errno_t err;
    pointNum = 0;
    file = fopen(objFileName, "r"); // err = 0 success,err != 0 fail
    if (file == NULL)
    {
        std::cout << "Failed to open objfile" << std::endl;
        return false;
    }

    const int SIZE = 2048;
    char buffer[SIZE];
    char type[80];
    int lineno = 0; // record line number


    // read file
    fgets(buffer, SIZE, file);// # Blender ....
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file);// # www.blender.org
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file);// mtllib halfCylinder.mtl
    // std::cout <<buffer<<std::endl;
    fgets(buffer, SIZE, file);// o halfCylinder_big
    // std::cout <<buffer<<std::endl;
    sscanf(buffer, "%s", type);

    if(strcmp(type, "o") != 0){
        std::cout << "Failed to read o fileName"<<std::endl;
        return false;
    }
    lineno+=4;
    vertex_tri.clear();
    normal_tri.clear();
    texcor_tri.clear();
    vertex_index.clear();
    normal_index.clear();
    texcor_index.clear();
    m_MeshTri.clear();
    while(fgets(buffer, SIZE, file) != NULL) {
        // read
        int res = sscanf(buffer, "%s", type);
        // std::cout << type<< " " <<std::endl;

        if(strcmp(type,"v") == 0){
            glm::fvec3 vertex;
            sscanf(buffer, "v %f %f %f",(&vertex.x), (&vertex.y), (&vertex.z));
            // std::cout <<buffer<<std::endl;
            // std::cout << vertex.x << ", " << vertex.y << ", "<< vertex.z<< std::endl;
            vertex_tri.push_back(vertex);

            if(pointNum == 0){
                m_MaxPos.x = m_MinPos.x = vertex.x;
                m_MaxPos.y = m_MinPos.y = vertex.y;
                m_MaxPos.z = m_MinPos.z = vertex.z;
            }
             // maximum
            if (m_MaxPos.x < vertex.x)
                m_MaxPos.x = vertex.x;
            if (m_MaxPos.y < vertex.y)
                m_MaxPos.y = vertex.y;
            if (m_MaxPos.z < vertex.z)
                m_MaxPos.z = vertex.z;

            // minmum
            if (m_MinPos.x > vertex.x)
                m_MinPos.x = vertex.x;
            if (m_MinPos.y > vertex.y)
                m_MinPos.y = vertex.y;
            if (m_MinPos.z > vertex.z)
                m_MinPos.z = vertex.z;
            pointNum++;

        }else if(strcmp(type, "vt") == 0){
            glm::fvec2 texcor;
            sscanf(buffer, "vt %f %f", &texcor.x, &texcor.y);
            // std::cout << texcor.x << ", " << texcor.y << std::endl;
            texcor_tri.push_back(texcor);

        }else if(strcmp(type, "vn") == 0){
            glm::fvec3 normal;
            sscanf(buffer, "vn %f %f %f", (&normal.x), (&normal.y), (&normal.z));
            // std::cout << normal.x << ", " << normal.y << ", "<< normal.z<< std::endl;
            normal_tri.push_back(normal);

        }else if(strcmp(type, "f") == 0){
            glm::ivec3 ver;
            glm::ivec3 tex;
            glm::ivec3 nor;
            int tmp;
            char p[10];
            sscanf(buffer, "f %d/%s",&tmp,p);
            // std::cout << buffer<<std::endl;
            // std::cout <<"1"<<p<<"1    "<<strncmp(p,"/",1)<<"1"<<std::endl;
            if(strncmp(p,"/",1) == 0){
           
                sscanf(buffer, "f %d//%d %d//%d %d//%d",
                &ver.x, &nor.x, &ver.y, &nor.y, &ver.z, &nor.z);
                // std::cout << ver.x << "//" << nor.x<<"  " << ver.y << "//"<<nor.y<<"  "<< ver.z<<"//"<<nor.z<< std::endl;
        
            }
            
            vertex_index.push_back(ver);
            texcor_index.push_back(tex);
            normal_index.push_back(nor);
            triNum++;
        }

    }
    if (feof(file)) {
        std::cout << "End of file reached!" << std::endl;
    } else if (ferror(file)) {
        std::cout << "Encountered an error while reading the file!" << std::endl;
    }
    
    fclose(file);
    std::cout << "max : " << m_MaxPos.x << ", " << m_MaxPos.y << ", "<< m_MaxPos.z <<std::endl;
    std::cout << "min : " << m_MinPos.x << ", " << m_MinPos.y << ", "<< m_MinPos.z <<std::endl;
    for(int i = 0; i < triNum; i++){

        m_MeshTri.push_back(
            Vertex{vertex_tri[vertex_index[i].x-1].x, 
                   vertex_tri[vertex_index[i].x-1].y, 
                   vertex_tri[vertex_index[i].x-1].z});
        m_MeshTri.push_back(
            Vertex{vertex_tri[vertex_index[i].y-1].x, 
                   vertex_tri[vertex_index[i].y-1].y, 
                   vertex_tri[vertex_index[i].y-1].z});
        m_MeshTri.push_back(
            Vertex{vertex_tri[vertex_index[i].z-1].x, 
                   vertex_tri[vertex_index[i].z-1].y, 
                   vertex_tri[vertex_index[i].z-1].z});
    }
    triNum*=3;
    std::cout<<"triNum : " <<triNum<<std::endl;
    return true;
}


