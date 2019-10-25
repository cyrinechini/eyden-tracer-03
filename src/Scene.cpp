#include "Scene.h"

#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include <fstream>

void CScene::ParseOBJ(const std::string& fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;
    std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
    
    Vec3f v[3000];
    double x,y,z;
    std::ifstream file(fileName);
    std::string data;
    int i = 0;
    std::string s;
    int v1_nr, v2_nr, v3_nr;
    
    if (! (file.is_open()) ) {
        std::cout << "Error when opening file!" << std::endl ;
        exit(EXIT_FAILURE);
    }
    else {
        while (getline(file, data)) {
            std::istringstream iss(data);
            iss >> s;
            if (s == "v") {
                iss >> s;
                x = std::stod(s);
                iss >> s;
                y = std::stod(s);
                iss >> s;
                z = std::stod(s);
                v[i] = Vec3f(x * 100.0f,y * 100.0f, z * 100.0f);
                i++;
            }
            else if (s == "f") {
               
                iss >> s;
                v1_nr = std::stoi(s);
            
                iss >> s;
                v2_nr = std::stoi(s);
                
                iss >> s;
                v3_nr = std::stoi(s);
                
                Add(std::make_shared<CPrimTriangle>(v[v1_nr - 1], v[v2_nr - 1], v[v3_nr - 1], pShader));
            }
        }
    }

    file.close();
	std::cout << "Finished Parsing" << std::endl;
}


